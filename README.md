# ESP32 Electricity Price Ticker for Home Assistant (ENTSO-E API)

[![ESP32](https://img.shields.io/badge/ESP32-Ready-blue)](https://www.espressif.com/en/products/devkits)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-green)](https://www.home-assistant.io/)
[![ENTS0-E API](https://img.shields.io/badge/ENTS0--E%20API-Free-orange)](https://transparency.entsoe.eu/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP-IDF](https://img.shields.io/badge/ESP-IDF-Framework-blue)](https://docs.espressif.com/projects/esp-idf/en/latest/)
[![GitHub stars](https://img.shields.io/github/stars/Legolas-2025/esp32-electricity-price-ticker?style=social)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/fork)

Inspired by the [hass-entso-e project](https://github.com/JaccoR/hass-entso-e). For detailed documentation, FAQ and troubleshooting, visit our [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/Home).

> **Current Stable Version: v4.2.0**
> v4.2.0 is a **major architectural release** that moves all scheduling logic to external Home Assistant automations for maximum flexibility and control.
> **CRITICAL REQUIREMENT**: v4.2.0 requires two external Home Assistant automations from the `v4.2.0/crucial_ha_automations/` folder. Without these automations, your prices will never update.
> Key improvements include: ESP-IDF framework for stability, NVS persistent storage for data retention across power cycles, optimized HTTP client, and the two helper files `entsoe_storage_v2.h` and `entsoe_http_idf.h`.

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Hardware and Software Requirements](#2-hardware-and-software-requirements)
3. [New in v4.2.0](#3-new-in-v420)
4. [External Home Assistant Automations (REQUIRED)](#4-external-home-assistant-automations-required)
5. [Credentials Setup](#5-credentials-setup)
6. [Installation Guide](#6-installation-guide)
7. [List of Available Sensors](#7-list-of-available-sensors)
8. [Features](#8-features)
9. [Troubleshooting](#9-troubleshooting)
10. [FAQ](#10-faq)

## 1. Project Overview

This ESP32-based electricity price ticker fetches real-time electricity prices from the ENTSO-E API and provides comprehensive sensors for Home Assistant. The system covers 35+ European countries and their bidding zones, and exposes rich information about current, next, and daily electricity prices.

Version **v3.1.1** introduced a major feature upgrade that enables the device to fetch and display electricity prices for the next day, in addition to the current day's prices. This enables users to compare today vs. tomorrow and schedule heavy loads (EV charging, laundry, heating) on the cheaper day.

Version **v3.5.0** attempted to address stability issues with a safe scheduling model, but **v4.2.0** represents the definitive solution through migration to the ESP-IDF framework and external automation control.

Version **v4.2.0** represents a major architectural change from previous versions:

- **Definitive reboot fix**: The ESP-IDF framework eliminates the random reboots that occurred with Arduino framework, even those that v3.5.0 attempted to address through scheduling changes.
- **NVS Persistent Storage**: New NVS implementation stores price data in flash memory, ensuring data survives power cycles and reducing API calls on startup.
- **Optimized HTTP Client**: New `entsoe_http_idf.h` provides better connection handling and error recovery using native ESP-IDF networking.
- **External Automation Control**: All scheduling logic has been moved to external Home Assistant automations, providing maximum flexibility for customizing update timing, retry logic, and notifications without recompiling firmware.
- **Two Required Helper Files**: v4.2.0 requires `entsoe_storage_v2.h` and `entsoe_http_idf.h` placed directly alongside the main YAML file.

The solution has been designed with reliability as the primary concern. The architecture ensures that today's price data remains available and accurate throughout the entire day, even during next-day update operations and even if the device reboots during the afternoon. With v4.2.0 and ESP-IDF, such reboots should be extremely rare.

## 2. Hardware and Software Requirements

### Hardware Requirements

The following hardware components are required to build the electricity price ticker:

- **ESP32 Development Board**: Any ESP32 variant with WiFi connectivity will work. The project has been tested extensively on the ESP32dev board, though ESP32-C3 variants are also supported. The board must have sufficient flash and RAM for ESPHome and the XML parsing logic.
- **USB Cable**: A USB cable capable of data transfer (not just charging) for programming the ESP32 board.
- **Power Supply**: A stable 5V USB power supply capable of providing at least 500mA. Unstable power can cause reliability issues and unexpected reboots. A quality power adapter is recommended for permanent installations.
- **WiFi Network**: The device requires a 2.4GHz WiFi network with internet connectivity to reach the ENTSO-E API. Some 5GHz-only networks are not compatible with ESP32 devices. The network must provide stable connectivity for reliable updates.
- **Home Assistant Instance**: Home Assistant must be running on the same network as the ESP32 device, with the ESPHome integration installed. The integration is included in Home Assistant by default and only needs to be configured.

### Software Requirements

The following software components must be installed and configured:

- **ESPHome 2025.12.0 or newer**: This project requires ESPHome version 2025.12.0 or newer due to specific component features and C++ standard library dependencies used in the firmware. Newer versions are supported and recommended.
- **ESP-IDF Framework**: v4.2.0 requires the ESP-IDF framework (not Arduino). When compiling in ESPHome, you must select ESP-IDF when prompted.
- **Home Assistant**: Any recent version of Home Assistant with the ESPHome integration enabled. The integration is included by default but must be configured through the Devices & Services page.
- **ENTSO-E API Account**: You must register for a free account on the ENTSO-E Transparency Platform to obtain an API token. Visit https://transparency.entsoe.eu/ and click the registration link. Note that token activation can take several hours after registration.
- **Web Browser**: A modern web browser for accessing the ESPHome dashboard or editing configuration files.

### Required Files

The v4.2.0 release requires the following files to function properly:

1. **entso-e-prices.yaml** - Main ESPHome configuration file (renamed from entsoe-hass-compatible.yaml)
2. **secrets.yaml** - User-provided credentials file
3. **entsoe_storage_v2.h** - NVS storage helper for persistent data
4. **entsoe_http_idf.h** - HTTP client helper for ESP-IDF
5. **entsoe_midnight_automation.yaml** - REQUIRED midnight automation from v4.2.0/crucial_ha_automations/
6. **entsoe_next_day_automation.yaml** - REQUIRED next-day automation from v4.2.0/crucial_ha_automations/

## 3. New in v4.2.0

### Framework Migration: Arduino to ESP-IDF

The most significant change in v4.2.0 is the migration from the Arduino framework to the ESP-IDF (Espressif IoT Development Framework). This migration addresses the core stability issues that affected the Arduino-based releases.

**Benefits of ESP-IDF:**

- **Improved Stability**: The ESP-IDF framework provides better memory management and task scheduling, eliminating the random reboots that occurred with Arduino.
- **Native ESP32 Optimization**: ESP-IDF is the official framework from Espressif, optimized specifically for ESP32 hardware.
- **Better Networking**: Native ESP-IDF networking stack provides more reliable HTTP client operation.
- **Long-term Support**: ESP-IDF is the primary framework for professional ESP32 development, ensuring future compatibility.

**Important:** When you first compile v4.2.0 in ESPHome, you will be prompted to select a framework. You **must** select ESP-IDF, not Arduino. If you previously used Arduino framework, you will need to perform a clean build.

### NVS Persistent Storage

Version v4.2.0 introduces NVS (Non-Volatile Storage) implementation for persistent price data storage. This feature ensures that price data survives power cycles and reduces the need for API calls during device startup.

**How NVS Storage Works:**

- Price data fetched from ENTSO-E API is automatically stored in NVS flash memory
- On device boot, cached price data is restored immediately
- Full API fetch still occurs at scheduled times, but cached data provides instant availability
- Data integrity is maintained through validation checks

**Implementation Details:**

The NVS storage is implemented in `entsoe_storage_v2.h` and automatically integrated into the main configuration. No manual configuration is required—the system handles storage automatically.

**Benefits:**

- Instant sensor availability after power cycle
- Reduced API load during startup
- Data resilience across power outages
- Faster recovery from forced reboots

### Reboot Issue Resolution

**Important Note:** Although v3.5.0 was released with the claim of fixing spontaneous reboots, further investigation revealed that the underlying issue required a framework migration to fully resolve. The v4.2.0 release with ESP-IDF definitively eliminates the random reboots that affected v3.5.0 on some hardware configurations.

**What was fixed:**

- Random reboots during midnight price updates (00:00)
- Random reboots during next-day data fetches (14:00)
- Instability during concurrent API operations
- Watchdog timer triggers under load

**If you experienced reboots with v3.5.0:**

- Upgrade to v4.2.0 and select ESP-IDF framework
- Perform a clean build (delete old build artifacts)
- Monitor stability over 24-48 hours
- The improvement should be immediately noticeable

### Helper Files Requirement

v4.2.0 requires two C++ helper files to be placed in the same directory as your main configuration:

1. **entsoe_storage_v2.h**
   - Implements NVS storage operations
   - Handles price data persistence
   - Manages flash memory access
   - Automatically included by main YAML

2. **entsoe_http_idf.h**
   - Optimized HTTP client for ESP-IDF
   - ENTSO-E API communication
   - XML parsing utilities
   - Error handling and retry logic

**Important: Where to Place Helper Files**

Place both `.h` helper files **directly alongside** `entso-e-prices.yaml`. ESPHome automatically includes all `.h` and `.cpp` files from the configuration directory.

**Directory Structure:**

```
your-esphome-project/
├── entso-e-prices.yaml              # Main configuration (from v4.2.0/)
├── entsoe_storage_v2.h              # NVS storage helper (from v4.2.0/)
├── entsoe_http_idf.h                # HTTP client helper (from v4.2.0/)
└── secrets.yaml                     # Your credentials (from v4.2.0/secrets_template.yaml)
```

**Do NOT put helper files in a `src/` subfolder** - place them directly alongside the main YAML file.

### Updated Secrets Structure

v4.2.0 introduces an updated secrets structure with additional configuration options:

**New secrets.yaml structure:**

```yaml
wifi_ssid: "YourWiFiNetworkName"
wifi_password: "YourWiFiPassword"
entsoe_api_encryption_key: "YOUR_HOME_ASSISTANT_API_KEY"
entsoe_entsoe_api_token: "YOUR_ENTSOE_API_TOKEN"
entsoe_country_area_code: "10YSI-ELES-----"  # For Slovenia
entsoe_timezone: "Europe/Ljubljana"
entsoe_fallback_ap_ssid: "ENTSOE-Fallback"
entsoe_fallback_ap_password: "fallbackpassword"
```

The secrets are now consistently prefixed with `entsoe_` for clarity and organized in the main YAML configuration.

## 4. External Home Assistant Automations (REQUIRED)

> **⚠️ CRITICAL: EXTERNAL AUTOMATIONS ARE REQUIRED FOR V4.2.0**
>
> Unlike v3.5.0 which had internal scheduling, **v4.2.0 requires two external Home Assistant automations** to trigger price updates. The main `entso-e-prices.yaml` file no longer updates on its own—it acts as a passive sensor platform awaiting external triggers.
>
> **Without importing both automation files from `v4.2.0/crucial_ha_automations/`, your prices will never update.**

### Why External Automations?

Moving scheduling to Home Assistant automations provides several advantages:

1. **Flexibility**: Modify update timing without recompiling firmware. Change the 13:50 start time to whenever suits your needs.
2. **Retry Customization**: Adjust retry intervals and conditions easily through the HA automation editor.
3. **Notification Control**: Customize when and how you receive alerts about update failures.
4. **Better Integration**: Use Home Assistant's full automation capabilities including conditions, delays, and variables.
5. **Easier Debugging**: View automation runs in HA's logbook and history.
6. **No Firmware Changes**: Change behavior through configuration, not code.

### Automation Files Location

Download both automation files from the `v4.2.0/crucial_ha_automations/` folder in the repository:

- `entsoe_midnight_automation.yaml` - Handles midnight NVS promote, fallback, and HTTP retries
- `entsoe_next_day_automation.yaml` - Handles 13:50 start and retry logic until 20:50

### Configuration Required

After importing the automations, you must configure the mobile notification service:

1. Open both automation files in a text editor
2. Search for `notify.mobile_app_sm_s911b`
3. Replace with your own mobile notification entity (e.g., `notify.mobile_app_your_phone_name`)
4. Save and reload automations in Home Assistant

### Automation 1: Midnight Update (entsoe_midnight_automation.yaml)

**Purpose:** Handles the midnight transition and today's price update

**Trigger:**
- At 00:00:00 (midnight exactly)

**Actions:**

1. **Primary Action - NVS Promote**: Attempts to promote tomorrow's price curve from NVS storage (tomorrow96 → today96) using the ESPHome action `esphome.entso_e_prices_promote_and_load_today_from_nvs`

2. **Fallback 1 - NVS Load**: If NVS promote fails, clears both today and tomorrow prices, then loads today's prices from today96 NVS storage using `esphome.entso_e_prices_load_today_from_nvs`

3. **Fallback 2 - HTTP Fetch**: If NVS load also fails, performs HTTP fetch with up to 3 retries (1-minute intervals):
   - Presses the force update button: `button.entso_e_prices_entso_e_force_update`
   - Waits 2 minutes between retries
   - Checks status sensors after each attempt

4. **Notifications**:
   - **Success**: Creates persistent notification confirming successful update
   - **Failure**: Creates persistent notification and sends mobile alert with full status details

**Status Sensors Monitored:**
- `sensor.entso_e_prices_price_update_status` - SUCCESS or FAILED
- `sensor.entso_e_prices_current_price_status` - Valid or Missing
- `sensor.entso_e_prices_entsoe_today_nvs_status` - NVS operation status
- `sensor.entso_e_prices_entsoe_last_update_source` - Source: NVS_promote, NVS_today96, or HTTP_today

### Automation 2: Next-Day Update (entsoe_next_day_automation.yaml)

**Purpose:** Handles fetching tomorrow's prices starting at 13:50 with intelligent retry logic

**Trigger:**
- At 13:50:00 (13 minutes before the standard 14:00 publish time)

**Actions:**

1. **Primary Action**: Immediately presses the force next day update button: `button.entso_e_prices_entso_e_force_next_day_update`

2. **Wait Period**: Waits 2 minutes for the initial update to complete

3. **Retry Loop**: If update not successful, retries every 30 minutes until 20:50:
   - Checks both `next_day_price_update_status` and `next_day_current_price_status`
   - Stops immediately upon success
   - Continues retrying if still failed (up to 20:50 cutoff)

4. **Notifications**:
   - **Success**: Creates persistent notification confirming update occurred between 13:50 and 20:50
   - **Failure**: Creates persistent notification and sends mobile alert with full status details

**Status Sensors Monitored:**
- `sensor.entso_e_prices_next_day_price_update_status` - SUCCESS or FAILED
- `sensor.entso_e_prices_next_day_current_price_status` - Valid or Waiting
- `sensor.entso_e_prices_next_day_last_price_update_time` - Timestamp of last update

### Testing Your Automations

After importing and configuring the automations:

1. **Verify Import**: Check Settings → Automations in Home Assistant - both automations should appear
2. **Check Configuration**: Verify you replaced `notify.mobile_app_sm_s911b` with your notification entity
3. **Enable Automation**: Toggle both automations to "On" state
4. **Test Manually**:
   - Run the midnight automation manually (it will wait until midnight to actually work, but you can trace the logic)
   - Press the next-day force update button to verify it triggers the automation check
5. **Monitor Logs**: Check Home Assistant logbook for automation runs
6. **Check Notifications**: Verify persistent notifications appear in the HA notifications panel

## 5. Credentials Setup

This project uses distinct types of credentials that serve different purposes. Understanding the difference is essential for proper configuration.

### ENTSO-E API Token

The ENTSO-E API token is required to authenticate your requests to the ENTSO-E Transparency Platform. This token is unique to your ENTSO-E account and authorizes access to electricity market data for your chosen bidding zone.

To obtain your ENTSO-E API token, visit the ENTSO-E Transparency Platform at https://transparency.entsoe.eu/. Click on the registration link and complete the form with your details. After verifying your email, log in to your account and generate a new API token from the "My Account" or "API Tokens" section. Copy this token carefully, as it will be used in your secrets.yaml configuration.

### ESPHome API Encryption Key

The ESPHome API encryption key is used to secure communication between the ESP32 device and your Home Assistant instance. This is a 32-character hexadecimal string that must match on both the device and in Home Assistant.

You can generate an encryption key using the ESPHome dashboard when creating a new project, which will prompt you automatically. Alternatively, you can generate one manually by running the command `python -c 'import secrets; print(secrets.token_hex(16))'` in a terminal.

### WiFi Credentials

WiFi credentials are stored separately in a `secrets.yaml` file for security and convenience. This file should contain your WiFi network name (SSID) and password, as well as other sensitive configuration values. The main YAML configuration references these values using the `!secret` directive.

Create a file named `secrets.yaml` in the same directory as your main YAML configuration file with the content shown in the New in v4.2.0 section above.

### Fallback AP Credentials

v4.2.0 includes fallback AP (Access Point) configuration for emergency access. If the device cannot connect to your WiFi network, it will create a fallback hotspot that you can connect to for troubleshooting.

Set these credentials in secrets.yaml:

```yaml
entsoe_fallback_ap_ssid: "ENTSOE-Fallback"
entsoe_fallback_ap_password: "fallbackpassword"
```

### Credentials Summary

| Credential | Purpose | Storage Location | Example |
|------------|---------|------------------|---------|
| WiFi SSID | Network identification | secrets.yaml | "MyWiFiNetwork" |
| WiFi Password | Network authentication | secrets.yaml | "MyWiFiPassword" |
| ESPHome API Key | HA-ESP32 communication encryption | secrets.yaml | "32charhexstring" |
| ENTSO-E API Token | ENTSO-E API authentication | secrets.yaml | "abc123..." |
| Country Area Code | Bidding zone selection | secrets.yaml | "10YDE-RENEW----U" |
| Timezone | Schedule timing | secrets.yaml | "Europe/Berlin" |
| Fallback AP SSID | Emergency access point name | secrets.yaml | "ENTSOE-Fallback" |
| Fallback AP Password | Emergency access point password | secrets.yaml | "fallbackpassword" |

## 6. Installation Guide

> **⚠️ IMPORTANT: v4.2.0 is Different from v3.5.0**
>
> Unlike v3.5.0 where some values were hardcoded in the YAML file and required manual editing, **v4.2.0 puts ALL credentials in secrets.yaml**. The main YAML file (`entso-e-prices.yaml`) uses `!secret` directives for all configurable values.
>
> **No editing of entso-e-prices.yaml is required** - just download it, create secrets.yaml with your values, and compile!
>
> **CRITICAL**: v4.2.0 requires two external Home Assistant automations from `v4.2.0/crucial_ha_automations/`. Without these, prices will never update.

### Step 1: Get Your ENTSO-E API Token

Visit the ENTSO-E Transparency Platform at https://transparency.entsoe.eu/ and register for a free account. After email verification, log in and generate a security token from your account settings. Copy this token; you will paste it into your secrets.yaml configuration later.

### Step 2: Find Your Country Area Code

Select your country's area code from the table below. Each European country has one or more bidding zones represented by unique area codes. If you are unsure which code to use, check your electricity bill or contact your utility company.

| Country | Area Code |
|---------|-----------|
| Austria | 10YAT-APG------L |
| Belgium | 10YBE----------2 |
| Bulgaria | 10YCA-BULGARIA-R |
| Croatia | 10YHR-HEP------M |
| Czech Republic | 10YCZ-CEPS-----N |
| Denmark | 10YDK-1--------A |
| Estonia | 10YEE-1--------U |
| Finland | 10YFI-1--------U |
| France | 10YFR-RTE------B |
| Germany | 10YDE-RENEW----U |
| Great Britain | 10YGB----------A |
| Greece | 10YGR-HTSO-----Y |
| Hungary | 10YHU-MAVIR----U |
| Ireland | 10YIE-1--------U |
| Italy | 10YIT-GRTN-----B |
| Latvia | 10YLV-1--------S |
| Lithuania | 10YLT-1001A0008Y |
| Luxembourg | 10YLU-CEGEDEL-NQ |
| Netherlands | 10YNL----------L |
| Norway | 10YNO-1--------2 |
| Poland | 10YPL-AREA-----S |
| Portugal | 10YPT-REN------W |
| Romania | 10YRO-TEL------P |
| Serbia | 10YRS-SRB------- |
| Slovakia | 10YSK-SEPS-----K |
| Slovenia | 10YSI-ELES-----O |
| Spain | 10YES-REE------M |
| Sweden | 10YSE-1--------K |
| Switzerland | 10YCH-SWISSGRIDZ |

### Step 3: Determine Your Timezone

Use a standard IANA timezone string for your location. This determines the correct time for scheduled price updates and ensures that hourly prices align with your local time. Common timezone strings include:

- `"Europe/Berlin"`
- `"Europe/Paris"`
- `"Europe/London"`
- `"Europe/Stockholm"`

### Step 4: Generate ESPHome API Encryption Key

Generate a 32-character hexadecimal string for the ESPHome API encryption key. You can use the ESPHome dashboard when creating the project, which will prompt you to generate a key automatically. Alternatively, use a command line as described in the Credentials section above.

### Step 5: Download v4.2.0 Files

Download the following files from the `v4.2.0/` folder in the repository:

1. **entso-e-prices.yaml** - Main ESPHome configuration file
2. **secrets_template.yaml** - Template for credentials file
3. **entsoe_storage_v2.h** - NVS storage helper file
4. **entsoe_http_idf.h** - HTTP client helper file

**Important: Where to Place Helper Files**

Place both `.h` helper files **directly alongside** `entso-e-prices.yaml`. ESPHome automatically includes all `.h` and `.cpp` files from the configuration directory.

**Correct Directory Structure:**

```
your-esphome-project/
├── entso-e-prices.yaml              # Main configuration file (from v4.2.0/)
├── entsoe_storage_v2.h              # NVS storage helper (from v4.2.0/)
├── entsoe_http_idf.h                # HTTP client helper (from v4.2.0/)
└── secrets.yaml                     # Your credentials (from v4.2.0/secrets_template.yaml)
```

**Do NOT put helper files in a `src/` subfolder** - place them directly alongside the main YAML file.

### Step 6: Download and Import Required Automations

> **⚠️ THIS STEP IS CRITICAL - AUTOMATIONS ARE REQUIRED**
>
> Download both automation files from the `v4.2.0/crucial_ha_automations/` folder:
>
> 1. **entsoe_midnight_automation.yaml** - Handles midnight NVS promote, fallback, and HTTP retries
> 2. **entsoe_next_day_automation.yaml** - Handles 13:50 start and retry logic until 20:50

**Importing Automations into Home Assistant:**

1. Go to **Settings → Automations** in Home Assistant
2. Click the **⋮ menu** (three dots) in the top right
3. Select **Import from file**
4. Select `entsoe_midnight_automation.yaml`
5. Repeat for `entsoe_next_day_automation.yaml`

**Configure Mobile Notifications:**

1. Open both automation files in a text editor
2. Search for `notify.mobile_app_sm_s911b`
3. Replace with your own mobile notification entity
   - To find your entity: Go to **Settings → Devices & Services → Notifications**
   - Your mobile device will have an entity like `notify.mobile_app_your_phone_name`
4. Save the files and reload automations in Home Assistant

### Step 7: Create secrets.yaml

Create a file named `secrets.yaml` in your project directory with your configuration values:

```yaml
wifi_ssid: "YourWiFiNetworkName"
wifi_password: "YourWiFiPassword"
entsoe_api_encryption_key: "YOUR_HOME_ASSISTANT_API_KEY"
entsoe_entsoe_api_token: "YOUR_ENTSOE_API_TOKEN"
entsoe_country_area_code: "10YSI-ELES-----O"  # Your country code
entsoe_timezone: "Europe/YourCity"
entsoe_fallback_ap_ssid: "ENTSOE-Fallback"
entsoe_fallback_ap_password: "fallbackpassword"
```

### Step 8: Select ESP-IDF Framework (Critical Step)

When you first compile the project in ESPHome dashboard:

1. **ESPHome will prompt** you to choose a framework
2. **Select "ESP-IDF"** (NOT Arduino)
3. **Click "Install"** to begin compilation

**Important:** v4.2.0 requires ESP-IDF framework. If you previously used Arduino framework with v3.5.0, you must select ESP-IDF for v4.2.0. The YAML file is configured for ESP-IDF and will not work correctly with Arduino.

### Step 9: Flash the ESP32

Connect your ESP32 board to your computer via USB. Open the ESPHome dashboard, navigate to the configuration file, and click "Install" to compile and upload the firmware. The first compilation may take several minutes as ESP-IDF components are downloaded.

Once compiled successfully, the firmware will be uploaded to the ESP32. After uploading, the device will reboot and attempt to connect to your WiFi network.

**Note:** For a clean build from a previous version, delete the `.esphome` build directory before compiling.

### Step 10: Add to Home Assistant

Once the firmware is uploaded and the device boots, Home Assistant should automatically discover the ESP32 device if the ESPHome integration is installed and both devices are on the same network. Navigate to **Settings → Devices & Services → ESPHome** and follow the prompts to add the device.

### Step 11: Verify Automations Are Working

After setup, verify that your automations are functioning correctly:

1. **Check Automation State**: Go to Settings → Automations - both should be enabled
2. **Check Next-Day Status**: After 13:50, verify `sensor.entso_e_prices_next_day_price_update_status` shows "SUCCESS"
3. **Check Midnight Behavior**: At midnight, verify today's prices are updated
4. **Test Notifications**: Check that persistent notifications appear in HA
5. **Verify Mobile Alerts**: Check your phone for any failure notifications

## 7. List of Available Sensors

### Today's Price Sensors

The following sensors provide access to the current day's electricity prices. All today's price sensors are updated at midnight when fresh daily data is fetched, with summary statistics recalculated accordingly.

- **json_hourly_prices_kwh** is the primary sensor for today's prices. It publishes a JSON array containing 24 hourly prices in the format `[{"hour":0,"price":0.123},{"hour":1,"price":0.118},...]`. This can be used for advanced templating or custom visualizations in Home Assistant dashboards.

- **hourly_avg_price_kwh** provides the average price for the current hour, calculated from all 15-minute intervals within that hour. This sensor is useful for hour-by-hour price comparisons without requiring manual aggregation.

- **current_hourly_price_kwh** displays the average electricity price for the current hour, enabling quick decisions about whether to run appliances now or wait for a cheaper hour. This sensor updates every 15 minutes.

- **next_hourly_price_kwh** displays the average electricity price for the next hour, enabling planning ahead for the immediate future. This sensor helps determine if prices are trending up or down and can drive "wait for cheaper next hour" automations.

- **daily_avg_price_kwh** provides a single numeric value representing the average price across all 24 hours of the day. This sensor is useful for quick daily comparisons and display purposes on dashboards.

- **daily_min_price_kwh** displays the lowest hourly price for the current day. This information is valuable for scheduling energy-intensive tasks like laundry, dishwashing, or EV charging during the cheapest hour.

- **daily_max_price_kwh** displays the highest hourly price for the current day. Knowing the peak prices helps avoid running appliances during expensive periods. The time of this maximum price can be read from the corresponding time sensors.

- **min_hourly_price_kwh** shows the lowest price among all hourly averages for the current day. This provides a cleaner view of the cheapest hour without considering the 15-minute granularity.

- **max_hourly_price_kwh** shows the highest price among all hourly averages for the current day. This helps identify the most expensive hour to avoid.

- **current_max_hourly_price_percentage** expresses the current hourly price as a percentage of the maximum hourly price for the day. A value of 100% indicates the current hour is the most expensive, while values below 100% indicate cheaper periods relative to the daily peak.

### Today's Time Sensors

- **time_of_max_price** displays the time when the maximum price occurs for the current day, in HH:MM format. This helps identify when to avoid running high-consumption devices. Updated daily at midnight after prices are fetched.

- **time_of_min_price** displays the time when the minimum price occurs for the current day, in HH:MM format. This helps identify the optimal time window for running high-consumption devices. Updated daily at midnight.

- **time_of_max_hourly_price** displays the time when the maximum hourly average price occurs for the current day. Updated daily at midnight.

- **time_of_min_hourly_price** displays the time when the minimum hourly average price occurs for the current day. Updated daily at midnight.

### Today's Status Sensors

- **price_update_status** is a binary-like text sensor that indicates whether the last update was successful. The state will be "SUCCESS" for successful updates and "FAILED/WAITING" for failed or pending updates. This sensor can be used in automations to send alerts if the ENTSO-E data is not being updated.

- **price_update_attempts** (implemented as `Daily Price Update Attempts`) tracks the number of API fetch attempts since midnight. A high value combined with a failed status indicates persistent API connectivity issues or problems with the ENTSO-E platform.

- **price_update_status_message** provides human-readable status information. Common states include "Success (96/96)", "Updating...", "System Boot", "HTTP Error: XXXX", or "No data points parsed". This provides detailed insight into what the updater is doing.

- **current_price_status** is a text sensor indicating data validity for the current 15-minute period. It shows "Valid" when a numeric price exists and "Missing" when the value is NAN, enabling direct automation on data integrity.

### Today's Current Price Sensors

- **current_electricity_price** displays the real-time price for the current 15-minute interval. This is the most granular price data available and updates every 15 minutes. Use this sensor for precise control of time-sensitive loads.

- **next_electricity_price** displays the price for the next 15-minute interval. This forward-looking sensor helps with planning decisions for the immediate future.

### Next Day Price Sensors (v3.1.1+)

The v3.1.1 release adds complete next-day price forecasting capabilities. These sensors become active after 14:00 when the ENTSO-E platform publishes the day-ahead market results. Next-day data is automatically cleared at 23:59:55 to prevent stale values.

- **json_next_day_hourly_prices_kwh** contains the JSON-formatted hourly prices for the next day. The format matches today's JSON sensor, enabling identical automation patterns. This is the primary sensor for next-day price distributions.

- **next_day_hourly_avg_price_kwh** provides the average price for the next day, enabling quick comparisons between today and tomorrow. This summary statistic helps decide which day is cheaper overall.

- **next_day_min_price_kwh** displays the lowest hourly price for the next day. This helps identify the cheapest hour tomorrow for scheduling energy-intensive tasks.

- **next_day_max_price_kwh** displays the highest hourly price for the next day. This helps avoid expensive hours tomorrow.

- **next_day_min_hourly_price_kwh** and **next_day_max_hourly_price_kwh** (stat-style next-day sensors) expose the hourly-averaged minimum and maximum prices for the next day.

### Next Day Status Sensors (v3.1.1+)

- **next_day_price_update_status** indicates whether the last next-day update was successful. The state will be "SUCCESS" for successful updates and "FAILED/WAITING" for failed or pending updates.

- **next_day_price_update_attempts** tracks the number of next-day API fetch attempts since midnight. A high value indicates that the ENTSO-E API may be delayed in publishing next-day data.

- **next_day_price_update_status_message** provides human-readable status information for next-day updates. Common states include "Waiting for 14:00...", "Success (96/96)", "Updating...", "Boot recovery...", and error messages.

- **next_day_current_price_status** is a text sensor indicating data validity for the current time slot in tomorrow's dataset, similar to `current_price_status` for today.

- **next_day_last_price_update_time** displays the timestamp of the last successful next-day price update.

### Control Entities

- **Force Today's Price** is a button that triggers an immediate API fetch for today's prices. This button schedules a safe update via the same worker/guarded path used by automatic triggers.

- **Entso-E Force Next Day Update** is a button for triggering next-day price updates manually. This button is protected by a time gate that only allows execution between 14:00 and 23:00 (effective window for ENTSO-E day-ahead data). It also uses the safe scheduling and worker-based execution path.

## 8. Features

### Real-Time Price Monitoring

The system provides real-time electricity prices at 15-minute intervals, enabling precise timing of energy consumption to coincide with low-price periods. The current and next electricity price sensors offer immediate insight into short-term trends.

### Daily Statistics

Automatic calculation of daily minimum, maximum, and average prices provides at-a-glance information for energy planning. The time-of-highest and time-of-lowest price sensors help identify the optimal times for running consumption-heavy devices.

### Next Day Forecasting (v3.1.1+)

The major feature in v3.1.1 enables the device to fetch and display prices for the next day. This enables informed decisions about whether to run high-consumption appliances today or tomorrow based on price levels.

### Automatic VAT and Fees

The configuration includes provider fee and VAT calculations that are applied to the base ENTSO-E price. This ensures the displayed prices reflect the actual cost per kWh that you would pay, rather than only wholesale market prices.

### Smart Retry Logic

The system implements intelligent retry logic that automatically retries failed API fetches according to the automation schedules. This ensures data availability even during temporary API outages or network issues.

### Status Monitoring

Comprehensive status sensors track the health of the price fetching system. The update status sensor indicates success or failure, the update attempts sensor shows how many attempts have been made, and the detailed status message provides context about errors or progress.

### Data Isolation (v3.1.1+)

Today's and next-day data pipelines are completely isolated to prevent interference. Today's data remains stable throughout the day even during next-day update operations. Next-day data is cleared at 23:59:55 to prevent stale values from leaking into the next day.

### Watchdog Protection (v3.1.1+)

A 20-second HTTP timeout prevents the ESP32 watchdog timer from rebooting the device during slow API responses. Both today and tomorrow scripts use this timeout. With ESP-IDF framework, this protection is further enhanced.

### Boot Recovery (v3.1.1+)

The system handles device restarts gracefully. If the device reboots within the valid next-day data window, it automatically attempts to fetch next-day prices. This ensures that device restarts during the afternoon do not permanently remove next-day forecasts.

### Safe Scheduling & Worker-Based Execution (v3.5.0)

All versions since v3.5.0 use a safe execution model for scheduled and manual updates:

- Time-based triggers only schedule work via flags, instead of executing heavy logic directly
- Worker ticks start updates when safe and skip if an update is already in progress
- Guards ensure no overlapping updates for today's and tomorrow's pipelines
- This architecture removes spontaneous reboots during scheduled updates

### NVS Persistent Storage (v4.2.0)

Version v4.2.0 introduces NVS storage for persistent price data:

- Automatic storage of fetched price data in flash memory
- Data retrieval on boot for instant sensor availability
- Validation of stored data integrity
- Reduced API calls during startup
- Data preservation across power cycles

### ESP-IDF Framework (v4.2.0)

Version v4.2.0 uses ESP-IDF framework for improved stability:

- Native ESP32 optimization from Espressif
- Better memory management and task scheduling
- Enhanced networking stack
- Definitive solution to random reboot issues
- Long-term support and compatibility

### External Automation Control (v4.2.0)

Version v4.2.0 moves all scheduling to Home Assistant automations:

- Maximum flexibility for customizing update timing
- Easy modification of retry intervals and conditions
- Full integration with HA automation capabilities
- No firmware changes required for behavior modifications
- Better debugging through HA logbook

## 9. Troubleshooting

### No Price Data

If sensors show 0.0000 or "unknown", verify the following. First, check that the ENTSO-E API token is correct in secrets.yaml and that the token has been activated (new tokens may take 24 hours). Second, confirm that your area code and timezone are correct. Third, review ESPHome logs for HTTP errors or parsing issues.

**Also check automation status:**
- Verify both automations are imported and enabled in Home Assistant
- Check that mobile notification entity was replaced (not `notify.mobile_app_sm_s911b`)
- Verify automations ran at 00:00 and 13:50
- Check Home Assistant logbook for automation execution history

### Prices Not Updating (CRITICAL - Check Automations)

If prices remain stale or show "unknown":

1. **Check Automation Status**:
   - Go to Settings → Automations in Home Assistant
   - Verify both `ENTSO-E v420: Midnight` and `ENTSO-E v420: Next-Day` automations exist and are enabled

2. **Check Automation Traces**:
   - Click on each automation
   - Check "Traces" to see if it ran successfully
   - Look for errors in the trace timeline

3. **Check Button Entities**:
   - Verify `button.entso_e_prices_entso_e_force_update` exists
   - Verify `button.entso_e_prices_entso_e_force_next_day_update` exists
   - Try pressing the buttons manually to test

4. **Check Status Sensors**:
   - `sensor.entso_e_prices_price_update_status` should show SUCCESS
   - `sensor.entso_e_prices_next_day_price_update_status` should show SUCCESS after 13:50

5. **Check Notifications**:
   - Look for persistent notifications in Home Assistant
   - Check if mobile alerts were received
   - Search logbook for "ENTSO-E" entries

### Compilation Errors About Missing Headers

If you see errors about missing `entsoe_storage_v2.h` or `entsoe_http_idf.h`:

- Verify both helper files are in the same directory as `entso-e-prices.yaml`
- Check file names match exactly (case-sensitive)
- Ensure the files are NOT in a `src/` subfolder
- Confirm ESP-IDF framework is selected (not Arduino)

### Midnight Update Failed

If no new prices appear after midnight, check the following. First, review the `Daily Price Update Attempts` sensor to see if update attempts are being made. Second, check ESPHome logs for error messages. Third, verify that the device's time (via Home Assistant) is correct.

**Automation-specific checks:**
- Verify midnight automation ran at 00:00 (check traces)
- Check if NVS promote succeeded or fell back to HTTP
- Verify mobile notification was received (or check persistent notifications)

### Next Day Data Not Available After 14:00

If next-day sensors remain in "Waiting for 14:00..." status after 14:00, check the following. First, verify the ESP32 system time is accurate by checking the Home Assistant time sync status. Second, check ESPHome logs for API or parsing errors. Third, confirm that ENTSO-E has published next-day data (there can be delays).

**Automation-specific checks:**
- Verify next-day automation started at 13:50 (check traces)
- Check if retries are occurring (should see multiple button presses in logbook)
- Verify automation stops after SUCCESS status is achieved

### Inaccurate Average Price

If daily averages seem incorrect, verify the following. First, check the provider fee and VAT settings in the script section of the YAML file. Second, review logs for any calculation error messages. Third, confirm that all 96 time slots are populated (you can inspect the JSON output sensors).

### Home Assistant Can't Connect

If Home Assistant fails to connect to the ESP32, verify the following. First, confirm that the API encryption key matches between secrets.yaml and Home Assistant device settings. Second, ensure both devices are on the same network and can reach each other. Third, check for firewall rules or network isolation.

### Spontaneous Reboots (Fixed in v4.2.0)

If you previously experienced random reboots:

- **Upgrade to v4.2.0 and select ESP-IDF framework** - This is the definitive fix
- Perform a clean build (delete old build artifacts)
- Confirm in logs that updates are being triggered by automations
- With v4.2.0 and ESP-IDF, these reboots should no longer occur

**Important:** If reboots occur with v4.2.0 ESP-IDF, capture the serial log around the reboot and open a GitHub issue. This is unexpected behavior that should be investigated.

### NVS Storage Issues

If NVS storage does not seem to be working:

- Check logs for "NVS" related messages during boot
- Verify `entsoe_storage_v2.h` is properly included
- Ensure ESP-IDF framework is selected
- Check that flash memory is not corrupted

### Automation Not Triggering

If automations do not seem to be running:

1. Verify automation is enabled (toggle in HA settings)
2. Check automation triggers are correct (13:50:00 and 00:00:00)
3. Check conditions are being evaluated correctly
4. Verify entity IDs match exactly
5. Check HA logbook for automation execution
6. Verify timezone settings in both ESPHome and HA

### Enable Debug Logging

To enable detailed logging for troubleshooting, add the following to your YAML configuration:

```yaml
logger:
  level: DEBUG
  logs:
    esphttpclient: DEBUG
    http_request: DEBUG
    entsoe: DEBUG
```

This will provide detailed information about API requests and responses, helping identify authentication issues, network problems, or API errors.

## 10. FAQ

### General Questions

- **Q: Is this project free?**
  A: Yes, the software is MIT licensed and freely available. The ENTSO-E API is also free with registration, though commercial use may have different terms.

- **Q: Which countries are supported?**
  A: All 35+ European countries covered by ENTSO-E are supported. See the area code table in Section 4 for the complete list.

- **Q: How often are prices updated?**
  A: Prices are updated by automations: at midnight for today's data and starting at 13:50 for next-day data. The midnight automation handles the transition; the next-day automation retries until successful (up to 20:50).

- **Q: Do I need a separate ESP32 for each country?**
  A: Yes, each device monitors one area code. If you need to monitor multiple bidding zones, you will need separate ESP32 devices.

- **Q: What ESPHome version is required?**
  A: This project requires ESPHome 2025.12.0 or newer. Using an older version may result in compilation errors or missing features.

- **Q: Why ESP-IDF instead of Arduino?**
  A: ESP-IDF provides better stability, memory management, and native ESP32 optimization. It definitively resolves the random reboot issues that affected Arduino-based releases, including v3.5.0.

### Automation Questions

- **Q: Why does v4.2.0 use external automations?**
  A: External automations provide maximum flexibility. You can modify update timing, retry intervals, and notifications without recompiling firmware. They integrate better with Home Assistant and are easier to debug.

- **Q: What happens if I do not import the automations?**
  A: **Prices will never update.** The main YAML file no longer contains internal scheduling—it relies entirely on external automations to trigger updates. Both automations are required.

- **Q: Can I modify the automation timing?**
  A: Yes! That's the benefit of external automations. Open the automation in HA's visual editor or YAML view and change the trigger time from 13:50 to your preferred time.

- **Q: How do I change retry intervals?**
  A: Open the next-day automation and modify the 30-minute delay in the repeat loop. You can customize this to retry more or less frequently.

- **Q: Do I need to modify the notification entity?**
  A: Yes, you **must** replace `notify.mobile_app_sm_s911b` with your own mobile notification entity, otherwise mobile alerts will fail silently.

### Configuration Questions

- **Q: What is the difference between secrets.yaml and the main YAML?**
  A: secrets.yaml stores all sensitive credentials including WiFi, API keys, tokens, and configuration values. The main YAML file uses `!secret` directives to reference these values. This keeps sensitive data separate from the main configuration.

- **Q: Why were secrets moved to secrets.yaml in v4.2.0?**
  A: Centralizing secrets in secrets.yaml improves security and makes configuration management easier. It also aligns with ESPHome best practices for sensitive data handling.

- **Q: How do I find my country area code?**
  A: See the table in Section 5 of this document, or visit the ENTSO-E transparency platform documentation for the complete list of bidding zone codes.

- **Q: What timezone format should I use?**
  A: Use standard IANA timezone format like "Europe/Berlin", "Europe/Paris", "Europe/London", or "Europe/Stockholm".

- **Q: What are the helper files for?**
  A: `entsoe_storage_v2.h` handles NVS persistent storage, and `entsoe_http_idf.h` provides optimized HTTP communication for ESP-IDF. Both are required for v4.2.0 to function correctly.

### Migration Questions

- **Q: Can I upgrade from v3.5.0 directly?**
  A: Yes, v4.2.0 is a direct replacement for v3.5.0. All existing sensor IDs and functionality are preserved.

- **Q: Will my automations break when upgrading to v4.2.0?**
  A: Your existing sensor-based automations will continue to work. However, you **must** import the new external automations from `v4.2.0/crucial_ha_automations/` for price updates to work.

- **Q: Do I need new secrets?**
  A: Yes, v4.2.0 uses a new secrets.yaml structure. Copy your values to the new format shown in Section 5. The new format includes the same WiFi credentials plus additional settings for ESP-IDF and fallback AP.

- **Q: What changed in v4.2.0?**
  A: v4.2.0 includes major architectural changes:
  - Framework migration from Arduino to ESP-IDF
  - NVS persistent storage implementation
  - Updated HTTP client (entsoe_http_idf.h)
  - External automation control (REQUIRED)
  - New secrets.yaml structure
  - Two required helper files
  - Fallback AP configuration
  - Definitive fix for random reboots

- **Q: I had reboots with v3.5.0. Will v4.2.0 fix them?**
  A: Yes, v4.2.0 with ESP-IDF definitively resolves the reboot issues. If you experienced random reboots with v3.5.0, upgrading to v4.2.0 and selecting ESP-IDF framework should eliminate these reboots entirely.

- **Q: Do I need to delete old build files before upgrading?**
  A: Yes, it is recommended to perform a clean build when migrating from v3.5.0 to v4.2.0. Delete the `.esphome` build directory before compiling the new version.

- **Q: Where do I put the helper files?**
  A: Create a directory for your ESPHome project and place both `.h` files in the **same folder** as `entso-e-prices.yaml`, NOT in a `src/` subfolder.

- **Q: What if I cannot select ESP-IDF in ESPHome?**
  A: Ensure you are using ESPHome 2025.12.0 or newer. Older versions may not support ESP-IDF framework selection. Update ESPHome if necessary.
