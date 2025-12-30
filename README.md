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

This ESP32-based electricity price ticker fetches real-time electricity prices from the ENTSO-E API and provides comprehensive sensors for Home Assistant. The system covers 35+ European countries and offers real-time 15-minute price intervals, hourly average prices, daily statistics, automatic VAT and fee calculations, and smart retry logic for reliability.

Version v3.1.1 introduces a major feature upgrade that enables the device to fetch and display electricity prices for the next day, in addition to the current day's prices. This enables users to compare today's and tomorrow's prices in real-time, making it possible to optimize energy consumption based on price predictions and decide whether to run high-consumption appliances today or tomorrow based on which day offers better prices.

The solution has been designed with reliability as the primary concern. The architecture ensures that today's price data remains available and accurate throughout the entire day, even during next-day price update operations. This isolation between data pipelines prevents the common issue of all sensors becoming unavailable during API fetch operations. A sophisticated boot recovery mechanism handles device restarts gracefully, and an HTTP timeout of 20 seconds prevents the ESP32 watchdog timer from rebooting the device during slow API responses.


## 2. Hardware and Software Requirements

### Hardware Requirements

The following hardware components are required to build the electricity price ticker:

- **ESP32 Development Board**: Any ESP32 variant with WiFi connectivity will work. The project has been tested extensively on the ESP32dev board, though ESP32-C3 variants are also supported. The board must have sufficient flash memory (4MB minimum recommended) to store the firmware and runtime data.
- **USB Cable**: A USB cable capable of data transfer (not just charging) for programming the ESP32 board.
- **Power Supply**: A stable 5V USB power supply capable of providing at least 500mA. Unstable power can cause reliability issues and unexpected reboots. A quality power adapter is recommended for permanent installations.
- **WiFi Network**: The device requires a 2.4GHz WiFi network with internet connectivity to reach the ENTSO-E API. Some 5GHz-only networks are not compatible with ESP32 devices. The network must provide reliable connectivity without frequent drops.
- **Home Assistant Instance**: Home Assistant must be running on the same network as the ESP32 device, with the ESPHome integration installed. The integration is included in Home Assistant by default and can be added through the Settings → Devices & Services menu.

### Software Requirements

The following software components must be installed and configured:

- **ESPHome 2025.12.0 or newer**: This project requires ESPHome version 2025.12.0 or newer due to specific component features and C++ standard library dependencies used in the firmware. Newer versions may provide additional features and bug fixes. ESPHome can be installed as a Home Assistant add-on through the Supervisor menu (now Settings → Add-ons in newer Home Assistant versions), or it can be run standalone on any system with Python 3.8+ installed using the command `pip install esphome`.
- **Home Assistant**: Any recent version of Home Assistant with the ESPHome integration enabled. The integration is included by default but must be configured through the Devices & Services page.
- **ENTSO-E API Account**: You must register for a free account on the ENTSO-E Transparency Platform to obtain an API token. Visit https://transparency.entsoe.eu/ and click the registration link. Note that API tokens may take up to 24 hours to become active after registration.
- **Web Browser**: A modern web browser for accessing the ESPHome dashboard or editing configuration files.


## 3. Credentials Setup

This project uses two distinct types of credentials that serve different purposes. Understanding the difference is essential for proper configuration.

### ENTSO-E API Token

The ENTSO-E API token is required to authenticate your requests to the ENTSO-E Transparency Platform. This token is unique to your ENTSO-E account and authorizes access to electricity market data for your specified bidding zone. The token is configured directly in the main YAML configuration file as a global variable.

To obtain your ENTSO-E API token, visit the ENTSO-E Transparency Platform at https://transparency.entsoe.eu/. Click on the registration link and complete the form with your details. After verifying your email address, log in to the platform and navigate to the section for generating security tokens. Create a new token, copy it immediately (tokens may not be viewable after leaving the page), and paste it into your YAML configuration file.

### ESPHome API Encryption Key

The ESPHome API encryption key is used to secure communication between the ESP32 device and your Home Assistant instance. This is a 32-character hexadecimal string that must match on both the device and Home Assistant side for the connection to be established. The key is configured in the API section of the YAML file.

You can generate an encryption key using the ESPHome dashboard when creating a new project, which will prompt you automatically. Alternatively, you can generate one manually by running the command `python3 -c "import os; print(os.urandom(24).hex())"` in a terminal, or by using any online 32-character hex string generator.

### WiFi Credentials

WiFi credentials are stored separately in a `secrets.yaml` file for security and convenience. This file should only contain your WiFi network name (SSID) and password. The main YAML configuration references these credentials using the `!secret` syntax, allowing you to share your configuration file without exposing your network credentials.

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

Visit the ENTSO-E Transparency Platform at https://transparency.entsoe.eu/ and register for a free account. After email verification, log in and generate a security token from your account settings. Copy this token as you will need to paste it into the YAML configuration file. Note that new tokens may take up to 24 hours to become active on the ENTSO-E system.

### Step 2: Find Your Country Area Code

Select your country's area code from the table below. Each European country has one or more bidding zones represented by unique area codes. If you are unsure which code to use, check your electricity bill or contact your energy provider.

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

Use a standard IANA timezone string for your location. This determines the correct time for scheduled price updates and ensures that hourly prices align with your local time. Common timezone strings include "Europe/Berlin" for Central European Time, "Europe/Paris" for Paris and surrounding areas, "Europe/London" for the United Kingdom, and "Europe/Stockholm" for Nordic countries.

### Step 4: Generate ESPHome API Encryption Key

Generate a 32-character hexadecimal string for the ESPHome API encryption key. You can use the ESPHome dashboard when creating the project, which will prompt you to generate a key automatically. Alternatively, run `python3 -c "import os; print(os.urandom(24).hex())"` in a terminal, or use any online hex generator.

### Step 5: Edit the YAML File

Download the latest `entsoe-hass-compatible.yaml` configuration file from the GitHub releases page. Open the file in a text editor and locate the following sections that require modification:

In the `api` section (around line 29), replace `YOUR_API_ENCRYPTION_KEY_HERE` with your generated 32-character encryption key. In the `time` section (around line 247), replace `YOUR_TIMEZONE_HERE` with your timezone string. In the `globals` section (around lines 341-346), replace `YOUR_ENTSOE_API_TOKEN_HERE` with your ENTSO-E API token and `YOUR_COUNTRY_AREA_CODE_HERE` with your area code from the table above.

### Step 6: Create secrets.yaml

Create a file named `secrets.yaml` in the same directory as your YAML configuration file. Add your WiFi credentials to this file:

```yaml
wifi_ssid: "YourWiFiNetworkName"
wifi_password: "YourWiFiPassword"
```

### Step 7: Flash the ESP32

Connect your ESP32 board to your computer via USB. Open the ESPHome dashboard or command line interface, navigate to the configuration file, and click "Compile" to build the firmware. The first compilation may take several minutes as ESPHome downloads the required components. After successful compilation, click "Upload" to flash the firmware to your ESP32 board. If you encounter upload errors, try holding the BOOT button on your ESP32 board during the upload process to enter bootloader mode.

### Step 8: Add to Home Assistant

Once the firmware is uploaded and the device boots, Home Assistant should automatically discover the ESP32 device if the ESPHome integration is installed and both devices are on the same network. Navigate to Settings → Devices & Services to find the new device. Click "Configure" and submit to complete the integration. The device will appear in Home Assistant with all configured sensors and controls.

For detailed Home Assistant automation examples, visit our [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/Home) and navigate to the Automations section.


## 5. Configuration

### Main Configuration Overview

The YAML configuration file contains all device configuration organized into logical sections. The `esphome` section defines the device name and friendly name. The `esp32` section specifies the board type and framework. The `wifi` section configures network connectivity using credentials from secrets.yaml. The `api` section enables Home Assistant integration with encryption. The `time` section synchronizes the device clock with Home Assistant. The `globals` section stores the ENTSO-E API token and area code. The `sensor`, `text_sensor`, `button`, and related sections define the entities exposed to Home Assistant.

### Adjusting Taxes and Fees

The configuration includes constants for provider fees and VAT that are applied to the base electricity price. These values can be modified in the script section of the YAML file to match your specific electricity plan. The default values assume a 12% provider fee and 22% VAT, which may not match your actual rates. Modify the `PROV_FEE` and `VAT_R` constants to reflect your actual electricity pricing structure. The formula multiplies the base price by (1 + provider fee) and then by (1 + VAT rate) to calculate the final displayed price.

### Customizing Update Intervals

The default configuration updates sensor values every 15 minutes during normal operation. This interval balances data freshness with API call frequency. If you need more frequent updates, you can modify the `interval` sections in the YAML file, though this may increase API load and network traffic. The midnight update occurs at 00:00:30 to ensure the ENTSO-E API has published the new day's prices before attempting to fetch them.

### Changing Area Code

If you move to a different country or want to monitor a different bidding zone, simply edit the `area_code` value in the globals section of the YAML file and recompile. Each ESP32 device can only monitor one area code at a time. If you need to monitor multiple areas, you will need separate ESP32 devices for each.


## 6. List of Available Sensors

### Today's Price Sensors

The following sensors provide access to the current day's electricity prices. All today's price sensors are updated at midnight when fresh daily data is fetched, with summary statistics recalculated at that time.

- **json_hourly_prices_kwh** is the primary sensor for today's prices. It publishes a JSON array containing 24 hourly prices in the format `[{"hour":0,"price":0.123},{"hour":1,"price":0.118},...]`. This format is designed for easy parsing in Home Assistant automations and scripts. The sensor updates every 15 minutes to ensure data freshness reflects the current pricing period.

- **hourly_avg_price_kwh** provides the average price for the current hour, calculated from all 15-minute intervals within that hour. This sensor is useful for hour-by-hour price comparisons without requiring JSON parsing. The value is updated at the start of each new hour.

- **current_hourly_price_kwh** displays the average electricity price for the current hour, enabling quick decisions about whether to run appliances now or wait for a cheaper hour. This sensor updates every 15 minutes as new price intervals become available.

- **next_hourly_price_kwh** displays the average electricity price for the next hour, enabling planning ahead for the immediate future. This sensor helps determine if prices are trending up or down and informs decisions about delaying energy-intensive tasks.

- **daily_avg_price_kwh** provides a single numeric value representing the average price across all 24 hours of the day. This sensor is useful for quick daily comparisons and display purposes on dashboards. The value is updated only at midnight when fresh daily data is fetched.

- **daily_min_price_kwh** displays the lowest hourly price for the current day. This information is valuable for scheduling energy-intensive tasks like laundry, dishwashing, or EV charging during the cheapest hours. The time of this minimum price can be found in the corresponding time sensor.

- **daily_max_price_kwh** displays the highest hourly price for the current day. Knowing the peak prices helps avoid running appliances during expensive periods. The time of this maximum price can be found in the corresponding time sensor.

- **min_hourly_price_kwh** shows the lowest price among all hourly averages for the current day. This provides a cleaner view of the cheapest hour without considering the 15-minute granularity.

- **max_hourly_price_kwh** shows the highest price among all hourly averages for the current day. This helps identify the most expensive hour to avoid.

- **current_max_hourly_price_percentage** expresses the current hourly price as a percentage of the maximum hourly price for the day. A value of 100% indicates the current hour is the most expensive, while 0% would indicate the cheapest. This normalized value is useful for gradient displays and quick visual assessment.

### Today's Time Sensors

- **time_of_max_price** displays the time when the maximum price occurs for the current day, in HH:MM format. This helps identify when to avoid running high-consumption devices. Updated daily at midnight.

- **time_of_min_price** displays the time when the minimum price occurs for the current day, in HH:MM format. This helps identify the optimal time window for running high-consumption devices. Updated daily at midnight.

- **time_of_max_hourly_price** displays the time when the maximum hourly average price occurs for the current day. Updated daily at midnight.

- **time_of_min_hourly_price** displays the time when the minimum hourly average price occurs for the current day. Updated daily at midnight.

### Today's Status Sensors

- **price_update_status** is a binary sensor that indicates whether the last update was successful. The state will be "on" for successful updates and "off" for failed updates. This sensor can be used in automations to send notifications when price data becomes stale.

- **price_update_attempts** tracks the number of API fetch attempts since midnight. A high value combined with a failed status indicates persistent API connectivity issues or problems with the ENTSO-E API service.

- **price_update_status_message** provides human-readable status information. Common states include "Update successful", "Updating...", "Waiting for midnight...", and various error messages indicating the type of failure encountered. Useful for debugging and monitoring.

### Today's Current Price Sensors

- **current_electricity_price** displays the real-time price for the current 15-minute interval. This is the most granular price data available and updates every 15 minutes. Use this sensor for precise real-time pricing decisions.

- **next_electricity_price** displays the price for the next 15-minute interval. This forward-looking sensor helps with planning decisions for the immediate future.

### Next Day Price Sensors (v3.1.1)

The v3.1.1 release adds complete next-day price forecasting capabilities. These sensors become active after 14:00 when the ENTSO-E platform publishes the day-ahead market results. Next-day data is automatically cleared at 23:59:55 to prevent stale data from appearing after midnight.

- **json_next_day_hourly_prices_kwh** contains the JSON-formatted hourly prices for the next day. The format matches today's JSON sensor, enabling identical automation patterns. This is the primary sensor for next-day price planning.

- **next_day_hourly_avg_price_kwh** provides the average price for the next day, enabling quick comparisons between today and tomorrow. This summary statistic helps decide which day is cheaper overall.

- **next_day_min_price_kwh** displays the lowest hourly price for the next day. This helps identify the cheapest hour tomorrow for scheduling energy-intensive tasks.

- **next_day_max_price_kwh** displays the highest hourly price for the next day. This helps avoid expensive hours tomorrow.

### Next Day Status Sensors (v3.1.1)

- **next_day_price_update_status** is a binary sensor indicating whether the last next-day update was successful. The state will be "on" for successful updates and "off" for failed updates.

- **next_day_price_update_attempts** tracks the number of next-day API fetch attempts since midnight. A high value indicates that the ENTSO-E API may be delayed in publishing next-day data.

- **next_day_price_update_status_message** provides human-readable status information for next-day updates. Common states include "Waiting for 14:00...", "Update successful", "Updating...", and error messages.

### Control Entities

- **Force Today's Price Update** is a button that triggers an immediate API fetch for today's prices. This is useful if you suspect the data may be stale and want to force a refresh outside the normal midnight schedule.

- **Entso-E Force Next Day Update** is a button for triggering next-day price updates manually. This button is protected by a time gate that only allows execution between 14:00 and 23:50, preventing attempts when next-day data is not yet available on the ENTSO-E platform. Pressing the button outside this window results in a logged message with no action taken.


## 7. Automation Schedule

The ESP32 device operates on an automated schedule for fetching and updating price data. Understanding this schedule helps with troubleshooting and planning automations in Home Assistant.

- **Every 15 minutes**: Sensor values are updated to reflect the current pricing period. This ensures that real-time price sensors always show current information without requiring a full API fetch. The update occurs at the :02 second mark of each 15-minute interval to prevent timing conflicts with other scheduled operations.

- **00:00:30 AM**: Primary price update for today's data. This is the main fetch that retrieves the new day's prices from the ENTSO-E API. The 30-second delay ensures the API has published the new day's data before the request is made.

- **Every 5 minutes (retry)**: If the previous update failed, the system automatically retries. This smart retry logic continues until successful, ensuring data availability even during temporary API outages. The retry attempts are tracked in the daily update attempts sensor.

- **14:00:30**: Primary price update for next-day data (v3.1.1+). This fetch retrieves the day-ahead prices once the ENTSO-E platform has published them, typically after 13:15. The data enables tomorrow's price planning.

- **23:59:55**: Next-day data clearing (v3.1.1+). All next-day sensors are cleared at this time to prevent stale data from appearing after midnight. Today's data remains intact.

- **On boot**: Recovery update after 50 seconds. If the device reboots during the valid next-day data window (14:00-23:00), the system automatically attempts to fetch next-day prices. If outside this window, the boot recovery does nothing, avoiding failed API attempts.


## 8. Features

### Real-Time Price Monitoring

The system provides real-time electricity prices at 15-minute intervals, enabling precise timing of energy consumption to coincide with low-price periods. The current and next electricity price sensors update every 15 minutes to reflect the most recent market conditions.

### Daily Statistics

Automatic calculation of daily minimum, maximum, and average prices provides at-a-glance information for energy planning. The time-of-highest and time-of-lowest price sensors help identify the optimal windows for energy-intensive activities.

### Next Day Forecasting (v3.1.1)

The major feature in v3.1.1 enables the device to fetch and display prices for the next day. This enables informed decisions about whether to run high-consumption appliances today or tomorrow based on price comparisons. Next-day data becomes available after 13:15 CET/CEST when the ENTSO-E platform publishes day-ahead market results.

### Automatic VAT and Fees

The configuration includes provider fee and VAT calculations that are applied to the base ENTSO-E price. This ensures the displayed prices reflect the actual cost per kWh that you would pay, rather than just the market price. These constants can be adjusted in the script section to match your actual electricity plan rates.

### Smart Retry Logic

The system implements intelligent retry logic that automatically retries failed API fetches every 5 minutes until successful. This ensures data availability even during temporary API outages or network connectivity issues. The retry attempts are tracked for monitoring purposes.

### Status Monitoring

Comprehensive status sensors track the health of the price fetching system. The update status sensor indicates success or failure, the update attempts sensor shows how many attempts have been made, and the status message sensor provides human-readable information for debugging.

### Data Isolation (v3.1.1)

Today's and next-day data pipelines are completely isolated to prevent interference. Today's data remains stable throughout the day even during next-day update operations. Next-day data is cleared at 23:59:55 to prevent stale data from appearing after midnight. This architecture ensures that critical today's data is never affected by next-day operations.

### Watchdog Protection (v3.1.1)

A 20-second HTTP timeout prevents the ESP32 watchdog timer from rebooting the device during slow API responses. This significantly improves stability and prevents the all-sensors-unavailable state that occurs during device reboots.

### Boot Recovery (v3.1.1)

The system handles device restarts gracefully. If the device reboots within the valid next-day data window, it automatically attempts to fetch next-day prices. This ensures that device restarts during the day do not result in permanently missing next-day data.


## 9. Troubleshooting

### No Price Data

If sensors show 0.0000 or "unknown", verify the following. First, check that the ENTSO-E API token is correct in the YAML file (line 343) and that the token has been activated (new tokens may take 24 hours). Second, verify that the country area code matches your location (line 346). Third, ensure the ESP32 has internet connectivity and can reach the ENTSO-E API. Fourth, check ESPHome logs for API error messages. Fifth, if using v3.1.1, check the `Current Price Status` sensor for validity information.

### Midnight Update Failed

If no new prices appear after midnight, check the following. First, review the `Daily Price Update Attempts` sensor to see if update attempts are being made. Second, check ESPHome logs for error messages during the 00:00:30 update window. Third, verify that Home Assistant time synchronization is working, as the ESP32 relies on HA for accurate time. Fourth, ensure the ENTSO-E API token has full access permissions.

### Inaccurate Average Price

If daily averages seem incorrect, verify the following. First, check the provider fee and VAT settings in the script section of the YAML file. Second, review logs for any calculation error messages. Third, verify that the ENTSO-E API returned complete data for all 24 hours.

### Home Assistant Can't Connect

If Home Assistant fails to connect to the ESP32, verify the following. First, confirm that the API encryption key matches between the YAML file and Home Assistant device settings. Second, ensure both ESP32 and Home Assistant are on the same network subnet. Third, check firewall settings that might block device discovery. Fourth, try using the device IP address instead of the hostname.

### Next Day Data Not Available After 14:00

If next-day sensors remain in "Waiting for 14:00..." status after 14:00, check the following. First, verify the ESP32 system time is accurate by checking the Home Assistant time sync status. Second, confirm that the ENTSO-E API token has access to day-ahead price data. Third, check ESPHome logs for any API error messages during the 14:00 fetch attempt.

### Enable Debug Logging

To enable detailed logging for troubleshooting, add the following to your YAML configuration:

```yaml
logger:
  level: DEBUG
  logs:
    esphttpclient: DEBUG
    http_request: DEBUG
```

This will provide detailed information about API requests and responses, helping identify authentication issues, network problems, or API errors.

### Common Log Messages

The following log messages indicate normal operation. `[INFO] Successfully updated all prices` indicates data was fetched successfully. `[INFO] Forward-fill applied` (v2.3.5+) indicates the system filled gaps in ENTSO-E compressed data. `[WARN] Failed (Attempt X)` indicates a retry is in progress, which is normal behavior. The following log messages indicate errors. `[ERROR] HTTP 401` indicates API authentication failed, check your token. `[ERROR] HTTP 429` indicates rate limit exceeded, reduce update frequency.


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
A: Yes, v3.1.1 is a direct replacement. All existing sensors and automations continue to work unchanged.

- **Q: Will my automations break?**
A: No, all existing today's price sensors work exactly as before. The new next-day sensors are additional entities.

- **Q: Do I need new secrets?**
A: No, secrets.yaml only contains WiFi credentials which haven't changed.

- **Q: What changed in v3.1.1?**
A: v3.1.1 added next-day price forecasting, complete data pipeline isolation, watchdog protection, boot recovery logic, and new next-day sensors. See the changelog for complete details.

For more frequently asked questions and advanced troubleshooting, visit our [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/Home).
