#pragma once

#include <string>
#include "esphome/core/log.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"  // for esp_crt_bundle_attach

namespace entsoe_http_idf {

static const char *TAG = "entsoe_http_idf";

// HTTPS GET using esp_http_client + ESP-IDF certificate bundle.
// On success: returns true and fills out_xml. On failure: returns false and sets out_status.
inline bool fetch(const std::string &url, std::string &out_xml, std::string &out_status) {
  esp_http_client_config_t cfg = {};
  cfg.url = url.c_str();
  cfg.timeout_ms = 20000;
  cfg.method = HTTP_METHOD_GET;

  // Use ESP-IDF's built-in certificate bundle for TLS server verification.
  cfg.crt_bundle_attach = esp_crt_bundle_attach;
  // Follow redirects (default is already to follow, but be explicit).
  cfg.disable_auto_redirect = false;

  esp_http_client_handle_t client = esp_http_client_init(&cfg);
  if (client == nullptr) {
    ESP_LOGW(TAG, "esp_http_client_init failed");
    out_status = "HTTP init failed";
    return false;
  }

  esp_err_t err = esp_http_client_open(client, 0);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "esp_http_client_open failed: %s", esp_err_to_name(err));
    out_status = "HTTP open failed";
    esp_http_client_cleanup(client);
    return false;
  }

  // Fetch headers first; some servers/IDF versions require this.
  int64_t content_length = esp_http_client_fetch_headers(client);
  if (content_length < 0) {
    ESP_LOGW(TAG, "esp_http_client_fetch_headers failed: %lld", (long long) content_length);
  } else {
    ESP_LOGI(TAG, "Content length: %lld", (long long) content_length);
  }

  int status_code = esp_http_client_get_status_code(client);
  ESP_LOGI(TAG, "HTTP status %d for url: %s", status_code, url.c_str());

  std::string xml;
  if (status_code == 200) {
    char buf[512];
    while (true) {
      int read_len = esp_http_client_read(client, buf, sizeof(buf));
      if (read_len < 0) {
        ESP_LOGW(TAG, "esp_http_client_read error");
        break;
      }
      if (read_len == 0) {
        // End of stream
        break;
      }
      xml.append(buf, read_len);
    }
  }

  esp_http_client_close(client);
  esp_http_client_cleanup(client);

  if (status_code == 200 && !xml.empty()) {
    out_xml.swap(xml);
    out_status = "HTTP 200 OK";
    return true;
  }

  char m[128];
  snprintf(m, sizeof(m), "HTTP Error: %d or empty body (len=%d)", status_code, (int)xml.size());
  out_status = m;
  return false;
}

}  // namespace entsoe_http_idf