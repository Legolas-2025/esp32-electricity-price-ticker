#pragma once

#include <vector>
#include <cstdint>
#include <ctime>
#include <cmath>

#include "esphome/core/log.h"
#include "nvs_flash.h"
#include "nvs.h"

namespace entsoe_storage_v2 {

static const char *TAG = "entsoe_storage_v2";
static const char *NVS_NS = "entsoe";
static const char *KEY_TODAY = "today96";
static const char *KEY_TOMORROW = "tomorrow96";

inline bool ensure_nvs_init() {
  static bool initialized = false;
  if (initialized) return true;

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_LOGW(TAG, "NVS init error %s, erasing flash...", esp_err_to_name(err));
    err = nvs_flash_erase();
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "NVS erase failed: %s", esp_err_to_name(err));
      return false;
    }
    err = nvs_flash_init();
  }

  if (err != ESP_OK) {
    ESP_LOGE(TAG, "NVS init failed: %s", esp_err_to_name(err));
    return false;
  }

  initialized = true;
  return true;
}

static inline bool store_curve(const char *key, uint32_t yyyymmdd, const std::vector<float> &prices_96) {
  if (prices_96.size() < 96) {
    ESP_LOGW(TAG, "store_curve(%s): prices_96.size() < 96 (%u)", key, (unsigned)prices_96.size());
    return false;
  }

  if (!ensure_nvs_init()) {
    ESP_LOGW(TAG, "store_curve(%s): NVS init failed", key);
    return false;
  }

  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_NS, NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "store_curve(%s): failed to open NVS namespace '%s' for write: %s",
             key, NVS_NS, esp_err_to_name(err));
    return false;
  }

  const size_t BUF_SIZE = 4 + 96 * sizeof(float);
  uint8_t buf[BUF_SIZE];

  buf[0] = (yyyymmdd >> 24) & 0xFF;
  buf[1] = (yyyymmdd >> 16) & 0xFF;
  buf[2] = (yyyymmdd >> 8)  & 0xFF;
  buf[3] = (yyyymmdd)       & 0xFF;

  float *p = reinterpret_cast<float *>(buf + 4);
  for (int i = 0; i < 96; i++) {
    p[i] = prices_96[i];
  }

  err = nvs_set_blob(handle, key, buf, BUF_SIZE);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "store_curve(%s): nvs_set_blob failed: %s", key, esp_err_to_name(err));
    nvs_close(handle);
    return false;
  }

  err = nvs_commit(handle);
  nvs_close(handle);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "store_curve(%s): nvs_commit failed: %s", key, esp_err_to_name(err));
    return false;
  }

  ESP_LOGI(TAG, "store_curve(%s): stored date %u and 96 prices", key, yyyymmdd);
  return true;
}

static inline bool load_curve_strict(const char *key, uint32_t expected_yyyymmdd, std::vector<float> &out_prices_96) {
  if (!ensure_nvs_init()) {
    ESP_LOGW(TAG, "load_curve_strict(%s): NVS init failed", key);
    return false;
  }

  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_NS, NVS_READONLY, &handle);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "load_curve_strict(%s): failed to open NVS namespace '%s' for read: %s",
             key, NVS_NS, esp_err_to_name(err));
    return false;
  }

  const size_t BUF_SIZE = 4 + 96 * sizeof(float);
  size_t len = 0;
  err = nvs_get_blob(handle, key, nullptr, &len);
  if (err == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGW(TAG, "load_curve_strict(%s): key '%s' not found", key, key);
    nvs_close(handle);
    return false;
  }
  if (err != ESP_OK || len != BUF_SIZE) {
    ESP_LOGW(TAG, "load_curve_strict(%s): nvs_get_blob length failed or size mismatch: %s, len=%u",
             key, esp_err_to_name(err), (unsigned)len);
    nvs_close(handle);
    return false;
  }

  uint8_t buf[BUF_SIZE];
  err = nvs_get_blob(handle, key, buf, &len);
  nvs_close(handle);
  if (err != ESP_OK || len != BUF_SIZE) {
    ESP_LOGW(TAG, "load_curve_strict(%s): nvs_get_blob failed (%s), len=%u (expected %u)",
             key, esp_err_to_name(err), (unsigned)len, (unsigned)BUF_SIZE);
    return false;
  }

  uint32_t stored_yyyymmdd =
    ((uint32_t)buf[0] << 24) |
    ((uint32_t)buf[1] << 16) |
    ((uint32_t)buf[2] << 8)  |
    ((uint32_t)buf[3]);

  if (stored_yyyymmdd != expected_yyyymmdd) {
    ESP_LOGW(TAG, "load_curve_strict(%s): stored date %u != expected %u",
             key, stored_yyyymmdd, expected_yyyymmdd);
    return false;
  }

  const float *p = reinterpret_cast<const float *>(buf + 4);
  out_prices_96.assign(96, NAN);
  for (int i = 0; i < 96; i++) {
    out_prices_96[i] = p[i];
  }

  ESP_LOGI(TAG, "load_curve_strict(%s): loaded date %u and 96 prices", key, stored_yyyymmdd);
  return true;
}

// Public helpers

inline bool store_today(uint32_t yyyymmdd, const std::vector<float> &prices_96) {
  return store_curve(KEY_TODAY, yyyymmdd, prices_96);
}

inline bool store_tomorrow(uint32_t yyyymmdd, const std::vector<float> &prices_96) {
  return store_curve(KEY_TOMORROW, yyyymmdd, prices_96);
}

inline bool load_today_strict(uint32_t today_yyyymmdd, std::vector<float> &out_prices_96) {
  return load_curve_strict(KEY_TODAY, today_yyyymmdd, out_prices_96);
}

inline bool load_tomorrow_strict(uint32_t tomorrow_yyyymmdd, std::vector<float> &out_prices_96) {
  return load_curve_strict(KEY_TOMORROW, tomorrow_yyyymmdd, out_prices_96);
}

// Optional: promote tomorrow96 -> today96 when day rolls over (if dates match)
inline bool promote_tomorrow_to_today(uint32_t today_yyyymmdd) {
  std::vector<float> prices_96;
  if (!load_curve_strict(KEY_TOMORROW, today_yyyymmdd, prices_96)) {
    ESP_LOGW(TAG, "promote_tomorrow_to_today: unable to load tomorrow96 for today=%u", today_yyyymmdd);
    return false;
  }
  if (!store_curve(KEY_TODAY, today_yyyymmdd, prices_96)) {
    ESP_LOGW(TAG, "promote_tomorrow_to_today: failed to store today96");
    return false;
  }
  ESP_LOGI(TAG, "promote_tomorrow_to_today: promoted tomorrow96 to today96 for date %u", today_yyyymmdd);
  return true;
}

}  // namespace entsoe_storage_v2