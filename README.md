# ESP32 Electricity Price Ticker for HA (ENTSO-E API)

![ESP32 Electricity Price Ticker](header.png)
[![ESP32](https://img.shields.io/badge/ESP32-Ready-blue)](https://www.espressif.com/en/products/devkits)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-green)](https://www.home-assistant.io/)
[![ENTS0-E API](https://img.shields.io/badge/ENTS0--E%20API-Free-orange)](https://transparency.entsoe.eu/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub stars](https://img.shields.io/github/stars/Legolas-2025/esp32-electricity-price-ticker?style=social)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/fork)
[![GitHub forks](https://img.shields.io/github/forks/Legolas-2025/esp32-electricity-price-ticker?style=social)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/fork)
[![GitHub issues](https://img.shields.io/github/issues/Legolas-2025/esp32-electricity-price-ticker)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/Legolas-2025/esp32-electricity-price-ticker)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/pulls)

Inspired by the [hass-entso-e project](https://github.com/JaccoR/hass-entso-e). For detailed documentation, FAQ and troubleshooting, visit our 📖 [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/Home).

> **Current Stable Version: v3.5.0**  
> v3.5.0 is a **major stability release** that keeps all v3.1.1 sensors and features but completely eliminates the spontaneous ESP32 reboots some users saw during scheduled updates (midnight and 14:00).  
> If you experienced random reboots or “all sensors unavailable” around those times, upgrading to **v3.5.0** is strongly recommended.

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Hardware and Software Requirements](#2-hardware-and-software-requirements)
3. [Credentials Setup](#3-credentials-setup)
4. [Installation Guide](#4-installation-guide)
5. [Configuration](#5-configuration)
6. [List of Available Sensors](#6-list-of-available-sensors)
7. [Automation Schedule](#7-automation-schedule)
8. [Features](#8-features)
9. [Troubleshooting](#9-troubleshooting)
10. [FAQ](#10-faq)

## 1. Project Overview

This ESP32-based electricity price ticker fetches real-time electricity prices from the ENTSO-E API and provides comprehensive sensors for Home Assistant. The system covers 35+ European countries and their bidding zones, and exposes rich information about current, next, and daily electricity prices.

Version **v3.1.1** introduced a major feature upgrade that enables the device to fetch and display electricity prices for the next day, in addition to the current day's prices. This enables users to compare today vs. tomorrow and schedule heavy loads (EV charging, laundry, heating) on the cheaper day.

Version **v3.5.0** builds directly on v3.1.1 and focuses on **stability under real-world scheduling**:

- Removes spontaneous ESP32 reboots during scheduled ENTSO-E API calls (midnight & 14:00).
- Preserves all existing sensor IDs and functionality so **no Home Assistant automations need to be changed**.
- Introduces a safe scheduling model with worker ticks and update guards to prevent overlapping updates and excessive blocking in ESPHome callbacks.

The solution has been designed with reliability as the primary concern. The architecture ensures that today's price data remains available and accurate throughout the entire day, even during next-day update operations and even if the device reboots during the afternoon.

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
- **Home Assistant**: Any recent version of Home Assistant with the ESPHome integration enabled. The integration is included by default but must be configured through the Devices & Services page.
- **ENTSO-E API Account**: You must register for a free account on the ENTSO-E Transparency Platform to obtain an API token. Visit https://transparency.entsoe.eu/ and click the registration link. Note that token activation can take several hours after registration.
- **Web Browser**: A modern web browser for accessing the ESPHome dashboard or editing configuration files.

## 3. Credentials Setup

This project uses two distinct types of credentials that serve different purposes. Understanding the difference is essential for proper configuration.

### ENTSO-E API Token

The ENTSO-E API token is required to authenticate your requests to the ENTSO-E Transparency Platform. This token is unique to your ENTSO-E account and authorizes access to electricity market data for your chosen bidding zone.

To obtain your ENTSO-E API token, visit the ENTSO-E Transparency Platform at https://transparency.entsoe.eu/. Click on the registration link and complete the form with your details. After verifying your email, log in to your account and generate a new API token from the "My Account" or "API Tokens" section. Copy this token carefully, as it will be used in your YAML configuration.

### ESPHome API Encryption Key

The ESPHome API encryption key is used to secure communication between the ESP32 device and your Home Assistant instance. This is a 32-character hexadecimal string that must match on both the device and in Home Assistant.

You can generate an encryption key using the ESPHome dashboard when creating a new project, which will prompt you automatically. Alternatively, you can generate one manually by running the command `python -c 'import secrets; print(secrets.token_hex(16))'` in a terminal.

### WiFi Credentials

WiFi credentials are stored separately in a `secrets.yaml` file for security and convenience. This file should only contain your WiFi network name (SSID) and password. The main YAML configuration references these values without exposing them directly.

Create a file named `secrets.yaml` in the same directory as your main YAML configuration file with the following content:

```yaml
wifi_ssid: "YourWiFiNetworkName"
wifi_password: "YourWiFiPassword"
```

### Credentials Summary

| Credential | Purpose | Storage Location | Example |
|------------|---------|------------------|---------|
| WiFi SSID | Network identification | secrets.yaml | "MyWiFiNetwork" |
| WiFi Password | Network authentication | secrets.yaml | "MyWiFiPassword" |
| ENTSO-E API Token | ENTSO-E API authentication | Main YAML file | "abc123..." |
| ESPHome API Key | HA-ESP32 communication encryption | Main YAML file | "32charhexstring" |
| Country Area Code | Bidding zone selection | Main YAML file | "10YDE-RENEW----U" |
| Timezone | Schedule timing | Main YAML file | "Europe/Berlin" |

## 4. Installation Guide

### Step 1: Get Your ENTSO-E API Token

Visit the ENTSO-E Transparency Platform at https://transparency.entsoe.eu/ and register for a free account. After email verification, log in and generate a security token from your account settings. Copy this token; you will paste it into your YAML configuration later.

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

### Step 5: Edit the YAML File

Download the latest `entsoe-hass-compatible.yaml` configuration file from the GitHub releases page (v3.5.0). Open the file in a text editor and locate the following sections that require modification:

- In the `api` section (around the top of the file), replace `YOUR_API_ENCRYPTION_KEY_HERE` with your generated 32-character encryption key.
- In the `time` section, replace `YOUR_TIMEZONE_HERE` with your IANA timezone string.
- In the `globals` section, update:
  - `api_token` initial_value to your ENTSO-E token.
  - `area_code` initial_value to your chosen bidding zone code.

### Step 6: Create secrets.yaml

Create a file named `secrets.yaml` in the same directory as your YAML configuration file. Add your WiFi credentials to this file:

```yaml
wifi_ssid: "YourWiFiNetworkName"
wifi_password: "YourWiFiPassword"
```

### Step 7: Flash the ESP32

Connect your ESP32 board to your computer via USB. Open the ESPHome dashboard or command line interface, navigate to the configuration file, and click "Compile" to build the firmware. The first compilation may take a few minutes.

Once compiled successfully, click "Upload" (or use `esphome run`) to flash the firmware onto the ESP32. After flashing, the device will reboot and attempt to connect to your WiFi network.

### Step 8: Add to Home Assistant

Once the firmware is uploaded and the device boots, Home Assistant should automatically discover the ESP32 device if the ESPHome integration is installed and both devices are on the same network. Navigate to **Settings → Devices & Services → ESPHome** and follow the prompts to add the device.

For detailed Home Assistant automation examples, visit our [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/Home) and navigate to the Automations section.

## 5. Configuration

### Main Configuration Overview

The YAML configuration file contains all device configuration organized into logical sections. The `esphome` section defines the device name and friendly name. The `esp32` section specifies the board type and framework. The `wifi`, `api`, `ota`, and `logger` sections handle connectivity, encryption, and diagnostics.

The core logic is implemented in:

- `sensor:` blocks for today’s and next day’s numeric sensors.
- `text_sensor:` blocks for time-of-day, statuses, and JSON exports.
- `time:` block for scheduling and worker ticks.
- `globals:` for internal data storage and control flags.
- `script:` for ENTSO-E API calls, XML parsing, and price/stat calculations.
- `button:` for manual update triggers.

### Adjusting Taxes and Fees

The configuration includes constants for provider fees and VAT that are applied to the base electricity price. These values can be modified in the script section of the YAML file to match your specific tariff:

```cpp
const double PROV_FEE = 0.12;  // Provider fee as fraction
const double VAT_R = 0.22;     // VAT rate as fraction
const double MULT = (1.0 + PROV_FEE) * (1.0 + VAT_R);
```

Adjust `PROV_FEE` and `VAT_R` to match your local provider’s fee and VAT rate.

### Customizing Update Intervals

The default configuration updates sensor values every 15 minutes during normal operation. This interval balances data freshness with API call frequency. If you need more frequent updates, you can modify the `on_time` schedules, but keep in mind:

- ENTSO-E day-ahead data is published once per day.
- Excessive API calls may be unnecessary and could hit rate limits.

### Changing Area Code

If you move to a different country or want to monitor a different bidding zone, simply edit the `area_code` value in the `globals` section of the YAML file and recompile. Each ESP32 device can only monitor a single bidding zone at a time.

## 6. List of Available Sensors

### Today's Price Sensors

The following sensors provide access to the current day's electricity prices. All today's price sensors are updated at midnight when fresh daily data is fetched, with summary statistics recalculated accordingly.

- **json_hourly_prices_kwh** is the primary sensor for today's prices. It publishes a JSON array containing 24 hourly prices in the format `[{"hour":0,"price":0.123},{"hour":1,"price":0.118},...]`. This can be used for advanced templating or custom visualizations in Home Assistant dashboards.

- **hourly_avg_price_kwh** provides the average price for the current hour, calculated from all 15-minute intervals within that hour. This sensor is useful for hour-by-hour price comparisons without requiring manual aggregation.

- **current_hourly_price_kwh** displays the average electricity price for the current hour, enabling quick decisions about whether to run appliances now or wait for a cheaper hour. This sensor updates every 15 minutes.

- **next_hourly_price_kwh** displays the average electricity price for the next hour, enabling planning ahead for the immediate future. This sensor helps determine if prices are trending up or down and can drive “wait for cheaper next hour” automations.

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

The v3.1.1 release (and v3.5.0) adds complete next-day price forecasting capabilities. These sensors become active after 14:00 when the ENTSO-E platform publishes the day-ahead market results. Next-day data is automatically cleared at 23:59:55 to prevent stale values.

- **json_next_day_hourly_prices_kwh** contains the JSON-formatted hourly prices for the next day. The format matches today's JSON sensor, enabling identical automation patterns. This is the primary sensor for next-day price distributions.

- **next_day_hourly_avg_price_kwh** provides the average price for the next day, enabling quick comparisons between today and tomorrow. This summary statistic helps decide which day is cheaper overall.

- **next_day_min_price_kwh** displays the lowest hourly price for the next day. This helps identify the cheapest hour tomorrow for scheduling energy-intensive tasks.

- **next_day_max_price_kwh** displays the highest hourly price for the next day. This helps avoid expensive hours tomorrow.

- **next_day_min_hourly_price_kwh** and **next_day_max_hourly_price_kwh** (stat-style next-day sensors) expose the hourly-averaged minimum and maximum prices for the next day.

### Next Day Status Sensors (v3.1.1+)

- **next_day_price_update_status** indicates whether the last next-day update was successful. The state will be "SUCCESS" for successful updates and "FAILED/WAITING" for failed or pending updates.

- **next_day_price_update_attempts** tracks the number of next-day API fetch attempts since midnight. A high value indicates that the ENTSO-E API may be delayed in publishing next-day data.

- **next_day_price_update_status_message** provides human-readable status information for next-day updates. Common states include "Waiting for 14:00...", "Success (96/96)", "Updating...", "Boot recovery...", and error messages.

- **next_day_current_price_status** is a text sensor indicating data validity for the current time slot in tomorrow’s dataset, similar to `current_price_status` for today.

### Control Entities

- **Force Today's Price Update** is a button that triggers an immediate API fetch for today's prices. In v3.5.0, this button schedules a safe update via the same worker/guarded path used by automatic triggers.

- **Entso-E Force Next Day Update** is a button for triggering next-day price updates manually. This button is protected by a time gate that only allows execution between 14:00 and 23:00 (effective window for ENTSO-E day-ahead data). In v3.5.0, it also uses the safe scheduling and worker-based execution path.

## 7. Automation Schedule

The ESP32 device operates on an automated schedule for fetching and updating price data. Understanding this schedule helps with troubleshooting and planning automations in Home Assistant.

- **Every 15 minutes**:
  - Today's sensors update at :02 seconds (e.g. 00:00:02, 00:15:02, etc.).
  - Next-day sensors update at :03 seconds (e.g. 00:00:03, 00:15:03, etc.).
  - This one-second staggering avoids timing conflicts.

- **00:00:30 AM**:
  - Primary price update for today's data is **scheduled**:
    - `last_update_success` is reset.
    - `retry_count` is reset.
    - `need_today_update` is set to `true`.
  - A worker tick (every 10 seconds) sees `need_today_update` and starts `smart_price_update` if it is not already running.

- **Every 5 minutes (retry, all day)**:
  - If the previous today’s update failed (`last_update_success == false`), `need_today_update` is set to `true` again.
  - Worker tick picks this up and safely retries using the same guarded path.

- **14:00:30**:
  - Primary price update for next-day data is **scheduled**:
    - `next_day_last_update_success` is reset.
    - `next_day_retry_count` is reset.
    - `next_day_update_status_message` is set to "Updating...".
    - `next_day_boot_recovery_executed` is reset to allow boot recovery if needed.
    - `need_tomorrow_update` is set to `true`.
  - A worker tick (every 10 seconds) checks if:
    - current time is between 14:00 and 23:00, and
    - `need_tomorrow_update` is `true`, and
    - `is_updating_tomorrow` is `false`,
    and then starts `smart_next_day_price_update`.

- **Every 5 minutes (retry, 14:00–23:00)**:
  - If the last next-day update failed, and time is between 14:00 and 23:00, the system schedules another attempt by setting `need_tomorrow_update = true`.

- **23:59:55**:
  - Next-day data clearing (v3.1.1+):
    - All next-day sensors, globals, and JSON outputs are reset.
    - Ensures tomorrow’s data is not shown after midnight.
    - Today’s pipeline is completely unaffected.

- **On boot**:
  - Boot recovery for today's data:
    - At :45 seconds, if `boot_recovery_executed` is `false`, the system schedules an update (`need_today_update = true`) and sets `boot_recovery_executed = true`.
  - Boot recovery for tomorrow's data:
    - At :50 seconds, if `next_day_boot_recovery_executed` is `false` and time is between 14:00 and 23:00, the system:
      - Sets `next_day_update_status_message = "Boot recovery..."`.
      - Sets `need_tomorrow_update = true`.
      - Sets `next_day_boot_recovery_executed = true`.
  - Worker ticks handle actually running the update scripts.

In **v3.5.0**, all heavy work (HTTP + XML parsing) is performed by worker ticks, not directly by `on_time` triggers. This design is what eliminates spontaneous reboots during scheduled updates.

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

The system implements intelligent retry logic that automatically retries failed API fetches every 5 minutes until successful. This ensures data availability even during temporary API outages or network issues.

### Status Monitoring

Comprehensive status sensors track the health of the price fetching system. The update status sensor indicates success or failure, the update attempts sensor shows how many attempts have been made, and the detailed status message provides context about errors or progress.

### Data Isolation (v3.1.1+)

Today's and next-day data pipelines are completely isolated to prevent interference. Today's data remains stable throughout the day even during next-day update operations. Next-day data is cleared at 23:59:55 to prevent stale values from leaking into the next day.

### Watchdog Protection (v3.1.1+)

A 20-second HTTP timeout prevents the ESP32 watchdog timer from rebooting the device during slow API responses. Both today and tomorrow scripts use this timeout. This significantly improves stability and prevents the all-sensors-unavailable state that can occur after watchdog resets.

### Boot Recovery (v3.1.1+)

The system handles device restarts gracefully. If the device reboots within the valid next-day data window, it automatically attempts to fetch next-day prices. This ensures that device restarts during the afternoon do not permanently remove next-day forecasts.

### Safe Scheduling & Worker-Based Execution (v3.5.0)

Version v3.5.0 introduces a safe execution model for scheduled and manual updates:

- Time-based triggers only schedule work via flags, instead of executing heavy logic directly.
- Worker ticks start updates when safe and skip if an update is already in progress.
- Guards ensure no overlapping updates for today’s and tomorrow’s pipelines.
- This architecture removes spontaneous reboots observed around midnight and 14:00 on some setups.

## 9. Troubleshooting

### No Price Data

If sensors show 0.0000 or "unknown", verify the following. First, check that the ENTSO-E API token is correct in the YAML file and that the token has been activated (new tokens may take 24 hours). Second, confirm that your area code and timezone are correct. Third, review ESPHome logs for HTTP errors or parsing issues.

### Midnight Update Failed

If no new prices appear after midnight, check the following. First, review the `Daily Price Update Attempts` sensor to see if update attempts are being made. Second, check ESPHome logs for error messages around 00:00:30. Third, verify that the device’s time (via Home Assistant) is correct.

### Next Day Data Not Available After 14:00

If next-day sensors remain in "Waiting for 14:00..." status after 14:00, check the following. First, verify the ESP32 system time is accurate by checking the Home Assistant time sync status. Second, check ESPHome logs for API or parsing errors around 14:00:30. Third, confirm that ENTSO-E has published next-day data (there can be delays).

### Inaccurate Average Price

If daily averages seem incorrect, verify the following. First, check the provider fee and VAT settings in the script section of the YAML file. Second, review logs for any calculation error messages. Third, confirm that all 96 time slots are populated (you can inspect the JSON output sensors).

### Home Assistant Can't Connect

If Home Assistant fails to connect to the ESP32, verify the following. First, confirm that the API encryption key matches between the YAML file and Home Assistant device settings. Second, ensure both devices are on the same network and can reach each other. Third, check for firewall rules or network isolation.

### Spontaneous Reboots During Updates (Fixed in v3.5.0)

If you previously experienced random reboots around midnight or 14:00:

- Upgrade your YAML to v3.5.0.
- Confirm in logs that:
  - Updates are being "scheduled" instead of executed directly in `on_time`.
  - Worker logs like `"Worker starting Today's Price Update"` appear.
- With v3.5.0, these reboots should no longer occur. If they do, capture the serial log around the reboot and open a GitHub issue.

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
  A: Prices are updated every 15 minutes during normal operation. A full daily refresh occurs at midnight, and next-day data is fetched at 14:00.

- **Q: Do I need a separate ESP32 for each country?**  
  A: Yes, each device monitors one area code. If you need to monitor multiple bidding zones, you will need separate ESP32 devices.

- **Q: What ESPHome version is required?**  
  A: This project requires ESPHome 2025.12.0 or newer. Using an older version may result in compilation errors or missing features.

### Configuration Questions

- **Q: What's the difference between secrets.yaml and the main YAML?**  
  A: secrets.yaml stores WiFi credentials only. All other credentials (API encryption key, ENTSO-E token, area code, timezone) must be edited directly in the main YAML file.

- **Q: Why aren't all credentials in secrets.yaml?**  
  A: This is a design choice to make it easier to copy and paste your complete configuration without managing multiple files. The inline placeholders for API credentials are clearly marked for replacement.

- **Q: How do I find my country area code?**  
  A: See the table in Section 4 of this document, or visit the ENTSO-E transparency platform documentation for the complete list of bidding zone codes.

- **Q: What timezone format should I use?**  
  A: Use standard IANA timezone format like "Europe/Berlin", "Europe/Paris", "Europe/London", or "Europe/Stockholm".

### Migration Questions

- **Q: Can I upgrade from v2.3.5 directly?**  
  A: Yes, v3.x versions (including v3.5.0) are direct replacements. All existing today's price sensors work exactly as before. The new next-day sensors are additional entities introduced in v3.1.1.

- **Q: Will my automations break when upgrading to v3.5.0 from v3.1.1?**  
  A: No. v3.5.0 was designed to keep all sensor IDs and semantics identical to v3.1.1. Only internal scheduling and stability mechanisms changed.

- **Q: Do I need new secrets?**  
  A: No, secrets.yaml only contains WiFi credentials which haven't changed.

- **Q: What changed in v3.5.0?**  
  A: v3.5.0 redesigned how scheduled updates are executed:
  - Updates are scheduled via flags and run in worker ticks.
  - Guards prevent overlapping updates.
  - HTTP timeouts are consistently applied.
  This removes spontaneous reboots during scheduled API calls without touching your existing entities.

For more frequently asked questions and advanced troubleshooting, visit our [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/Home).
