# ⚡ ESP32 Electricity Price Ticker for HA (ENTSO-E API)
![ESP32 Electricity Price Ticker](header.png)

[![ESP32](https://img.shields.io/badge/ESP32-Ready-blue)](https://www.espressif.com/en/products/devkits)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-green)](https://www.home-assistant.io/)
[![ENTS0-E API](https://img.shields.io/badge/ENTS0--E%20API-Free-orange)](https://transparency.entsoe.eu/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub stars](https://img.shields.io/github/stars/Legolas-2025/esp32-electricity-price-ticker?style=social)](https://github.com/Legolas-2025/esp32-electricity-price-ticker)
[![GitHub forks](https://img.shields.io/github/forks/Legolas-2025/esp32-electricity-price-ticker?style=social)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/fork)
[![GitHub issues](https://img.shields.io/github/issues/Legolas-2025/esp32-electricity-price-ticker)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/Legolas-2025/esp32-electricity-price-ticker)](https://github.com/Legolas-2025/esp32-electricity-price-ticker/pulls)

This **ESP32 Electricity Price Ticker** is a real-time electricity price monitoring solution for smart homes. It fetches real-time electricity prices from the ENTSO-E API, adds relevant VAT/fees, and provides comprehensive smart price sensors for Home Assistant automation. It is perfect for optimizing electricity costs and smart home energy management across **35+ European countries**. Inspired by the [hass-entso-e project](https://github.com/JaccoR/hass-entso-e), this ESP32 hardware version offers enhanced reliability and offline recovery capabilities. 

For detailed documentation, FAQ and troubleshooting, visit our 📖 [Wiki Home Page](https://github.com/Legolas-2025/esp32-electricity-price-ticker/wiki/1-Home).

🚀 **v2.0.0 Features:**
- Smart retry logic (3 attempts for failed updates)
- Boot recovery (automatic price fetch after power outages)  
- Real-time status monitoring sensors
- 99% reliability improvement


## 🎯 Quick Start

**[⚡ 5-Minute Setup Guide](QUICKSTART.md)** | **[📖 Full Documentation](README.md)** | **[🏗️ Setup Instructions](#-installation-guide)**

## ✨ Features

### Real-Time Price Monitoring
- **15-minute interval pricing** for precise consumption timing
- **Hourly average prices** for broader usage planning
- **Current and next price predictions** for immediate decisions
- **Daily statistics** including min/max/average prices
- **Price percentage calculations** showing relative cost

### Smart Calculations
- **Automatic VAT and provider fee application**
- **EUR/MWh to EUR/kWh conversion**
- **Time-based price tracking** (when min/max occur)
- **JSON export** of hourly prices for external systems

### Home Assistant Integration
- **Multiple sensor types** (numeric and text sensors)
- **Real-time updates** every 15 minutes
- **Automatic data refresh** at midnight with smart retry logic
- **Manual force update button** for testing
- **Home Assistant time synchronization**

### 🆕 Version 2.0.0: Enhanced Reliability & Status Monitoring
- **Intelligent Retry Logic**: Up to 3 automatic retry attempts for failed updates
- **Data Freshness Check**: Prevents unnecessary API calls if data is recent
- **Status Monitoring Sensors**: Real-time update status and retry tracking
- **Bidirectional Communication**: Home Assistant can verify update status
- **Critical Failure Detection**: Automatic alerts when all retry attempts fail
- **Cross-Platform Verification**: ESPHome validates updates with Home Assistant
- **🆕 Boot Recovery**: Automatic price fetch on device boot for power outage recovery
- **🛠️ Production Ready**: All ESPHome compilation errors resolved for smooth installation

## 📋 Requirements

### Hardware
- ESP32 development board (any model: ESP32-WROOM, ESP32-DevKit, etc.)
- USB cable for programming
- Stable 2.4GHz WiFi connection

### Software
- **ESP Home dashboard** (web interface)
- **Home Assistant** instance
- **ENTS0-E API** account (free)

### ⚠️ Version 2.0.0 Requirements
- **ESPHome 2025.12.0+** (required for API action responses feature)
- Enhanced Home Assistant integration for status monitoring
- **Boot Recovery**: Automatic power outage recovery functionality
- **Production Ready**: All ESPHome compilation errors resolved in v2.0.0

### Network
- WiFi network with 2.4GHz support
- Internet connection for API access

## 🚀 Installation Guide

### Step 1: Get ENTSO-E API Token

1. **Visit the ENTSO-E Transparency Platform**
   - Go to: https://transparency.entsoe.eu/

2. **Register for API Access**
   - Click "How to register?" link
   - Complete the registration form
   - Verify your email address

3. **Obtain Your API Token**
   - Log into your account
   - Navigate to API documentation section
   - Generate a new security token
   - **Save this token** - you'll need it for configuration

> **Note**: The ENTSO-E API is free but requires registration. Tokens are typically approved within 24 hours.

### Step 2: Install ESP Home

**⚠️ Version 2.0.0 users**: Ensure you have ESPHome 2025.12.0 or later.

Choose one of these installation methods:

#### Option A: Docker (Recommended)
```bash
# Create directory for ESP Home configuration
mkdir esphome
cd esphome

# Run ESP Home in Docker (Latest version)
docker run -d \
  --name esphome \
  --privileged \
  -p 6052:6052 \
  -v $(pwd):/config \
  -v /etc/localtime:/etc/localtime:ro \
  ghcr.io/esphome/esphome:latest

# Access dashboard at http://localhost:6052
```

#### Option B: Python Installation
```bash
# Install ESP Home via pip (upgrade to latest)
pip install --upgrade esphome

# Start dashboard
esphome dashboard

# Access dashboard at http://localhost:6052
```

#### Option C: Home Assistant Add-on
1. Open Home Assistant
2. Go to Supervisor > Add-on Store
3. Search for "ESPHome"
4. Install and start the add-on
5. **Ensure add-on is updated to latest version**

### Step 3: Configure Your Project

1. **Create New Project**
   - Open ESP Home dashboard (http://localhost:6052)
   - Click "NEW PROJECT"
   - Enter project name (e.g., "electricity-prices")

2. **Configure YAML**
   - Copy the content from `entsoe-hass-compatible.yaml`
   - Paste into your new project
   - Save the file

3. **Create Secrets File**
   - Create `secrets.yaml` in the same directory
   - Copy content from `secrets_template.yaml`
   - Replace placeholder values with your actual data

### Step 4: Customize Configuration

#### WiFi Configuration
```yaml
wifi:
  ssid: "YourWiFiNetworkName"
  password: "YourWiFiPassword"
```

#### Country Selection
Find your country area code from the list below and update:
```yaml
- id: area_code
  type: std::string
  initial_value: '"YOUR_COUNTRY_AREA_CODE_HERE"'
```

#### Timezone Setting
```yaml
time:
  - platform: homeassistant
    id: ha_time 
    timezone: "YourTimezone"  # e.g., "Europe/Berlin"
```

### Step 5: Flash ESP32

1. **Connect ESP32** to your computer via USB
2. **Click "Install"** in ESP Home dashboard
3. **Select the correct port** (usually /dev/ttyUSB0 on Linux, COM3 on Windows)
4. **Wait for compilation and upload** (2-5 minutes)
5. **Note the device IP address** shown after successful upload

### Step 6: Add to Home Assistant

1. **Open Home Assistant**
2. **Go to Configuration > Integrations**
3. **Click "Add Integration"**
4. **Search for "ESPHome"**
5. **Enter device IP address** (shown in ESP Home dashboard)
6. **Enter API encryption key** from your secrets file
7. **Enable API Actions** (required for v2.0.0):
   - Click the "configure" button on the ESPHome integration
   - Check "Allow the device to perform Home Assistant actions"
   - Click "submit"
8. **Click "Finish"**

### Step 7: Configure Home Assistant Automations (v2.0.0)

Create these automations for optimal status monitoring:

#### Critical Failure Alert
```yaml
automation:
  - alias: "Electricity Price Update Critical Failure"
    trigger:
      - platform: state
        entity_id: sensor.price_update_status
        to: "FAILED"
    condition:
      - condition: template
        value_template: "{{ states('sensor.price_update_retry_count') | int >= 3 }}"
    action:
      - service: notify.persistent_notification
        data:
          title: "⚠️ Electricity Price Update Failed"
          message: |
            Critical: Electricity price updates have failed after 3 attempts.
            Last successful update: {{ states('sensor.last_price_update_time') }}
            Status: {{ states('sensor.price_update_status_message') }}
            
            Please check:
            - ESPHome device connectivity
            - ENTSO-E API availability
            - Home Assistant integration
```

#### Status Dashboard Card
```yaml
type: entities
entities:
  - entity: sensor.price_update_status
    name: Update Status
  - entity: sensor.price_update_retry_count  
    name: Retry Attempts
  - entity: sensor.last_price_update_time
    name: Last Update Time
  - entity: sensor.price_status_message
    name: Status Message
title: Electricity Price Update Status
```

## 🌍 Supported Countries and Area Codes

The ENTSO-E API covers all European electricity market areas. Here are the available country codes:

| Country | Area Code | Currency | Notes |
|---------|-----------|----------|--------|
| **Austria** | `10YAT-APG------L` | EUR | Austrian Power Grid |
| **Belgium** | `10YBE----------2` | EUR | Elia (Belgian TSO) |
| **Bosnia and Herzegovina** | `10YBA-1----------` | BAM | NOS BiH |
| **Bulgaria** | `10YCA-BULGARIA-R` | BGN | ESO EAD |
| **Croatia** | `10YHR-HEP------M` | HRK/HRK | Croatian TSO |
| **Cyprus** | `10YCY-1001A0003J` | EUR | Cyprus TSO |
| **Czech Republic** | `10YCZ-CEPS-----N` | CZK | ČEPS |
| **Denmark** | `10YDK-1--------A` | DKK | Energinet |
| **Estonia** | `10YEE-1--------U` | EUR | Elering |
| **Finland** | `10YFI-1--------U` | EUR | Fingrid |
| **France** | `10YFR-RTE------B` | EUR | RTE |
| **Germany** | `10YDE-RENEW----U` | EUR | 50Hertz, Amprion, TenneT, TransnetBW |
| **Great Britain** | `10YGB----------A` | GBP | National Grid ESO |
| **Greece** | `10YGR-HTSO-----Y` | EUR | IPTO |
| **Hungary** | `10YHU-MAVIR----U` | HUF | MAVIR |
| **Ireland** | `10YIE-1--------U` | EUR | EirGrid |
| **Italy** | `10YIT-GRTN-----B` | EUR | Terna |
| **Latvia** | `10YLV-1--------S` | EUR | Augstsprieguma Tikls |
| **Lithuania** | `10YLT-1001A0008Y` | EUR | Litgrid |
| **Luxembourg** | `10YLU-CEGEDEL-NQ` | EUR | Cegedel |
| **Netherlands** | `10YNL----------L` | EUR | TenneT |
| **North Macedonia** | `10YMK-MEPSO----8` | MKD | MEPSO |
| **Norway** | `10YNO-1--------2` | NOK | Statnett |
| **Poland** | `10YPL-AREA-----S` | PLN | PSE |
| **Portugal** | `10YPT-REN------W` | EUR | REN |
| **Romania** | `10YRO-TEL------P` | RON | Transelectrica |
| **Serbia** | `10YRS-SRB-------` | RSD | EMS |
| **Slovakia** | `10YSK-SEPS-----K` | EUR | SEPS |
| **Slovenia** | `10YSI-ELES-----O` | EUR | ELES (Default example) |
| **Spain** | `10YES-REE------M` | EUR | REE |
| **Sweden** | `10YSE-1--------K` | SEK | Svenska Kraftnät |
| **Switzerland** | `10YCH-SWISSGRIDZ` | CHF | Swissgrid |
| **Turkey** | `10YTR-1--------U` | TRY | TEİAŞ |

> **Important**: Some countries may have limited data availability or different update frequencies. The API provides real-time data for most European markets.

## 📊 Available Sensors

After successful setup, these sensors will be available in Home Assistant:

### Price Sensors (€/kWh)
| Sensor Name | Description | Update Frequency |
|-------------|-------------|------------------|
| `Current Electricity Price` | Real-time price for current 15-min slot | Every 15 minutes |
| `Next Electricity Price` | Price for next 15-min slot | Every 15 minutes |
| `Current Hourly Electricity Price` | Average price for current hour | Every 15 minutes |
| `Next Hourly Electricity Price` | Average price for next hour | Every 15 minutes |
| `Average Electricity Price Today` | Daily average price | Daily at midnight |
| `Highest Electricity Price Today` | Maximum price today | Daily at midnight |
| `Lowest Electricity Price Today` | Minimum price today | Daily at midnight |
| `Highest Hourly Electricity Price Today` | Maximum hourly price today | Daily at midnight |
| `Lowest Hourly Electricity Price Today` | Minimum hourly price today | Daily at midnight |

### Percentage Sensor
| Sensor Name | Description | Range |
|-------------|-------------|-------|
| `Current Max Hourly Price Percentage` | Current price as % of today's max | 0-100% |

### 🆕 Status Monitoring Sensors (v2.0.0)
| Sensor Name | Description | Use Case |
|-------------|-------------|----------|
| `Price Update Status` | SUCCESS/FAILED status | Dashboard monitoring |
| `Price Update Retry Count` | Current retry attempt (0-3) | Troubleshooting |
| `Last Price Update Time` | Timestamp of last update | Historical tracking |
| `Price Update Status Message` | Detailed status description | Error diagnosis |

### Text Sensors
| Sensor Name | Description | Example Output |
|-------------|-------------|----------------|
| `ENTSO-E Hourly Prices EUR/kWh JSON` | All 24 hourly prices in JSON format | `[0.1234,0.1156,0.1098,...]` |
| `Time Of Highest Energy Price Today` | When peak price occurs | `15:30` |
| `Time Of Lowest Energy Price Today` | When minimum price occurs | `02:15` |
| `Time Of Highest Hourly Energy Price Today` | When hourly peak occurs | `16:00` |
| `Time Of Lowest Hourly Energy Price Today` | When hourly minimum occurs | `03:00` |

### Control
| Control Name | Description | Function |
|--------------|-------------|----------|
| `Entso-E Force Update` | Manual trigger for price update | Updates all sensors immediately |

## ⚙️ Configuration Options

### Adjusting Taxes and Fees
The electricity price calculation includes provider fees and VAT. Modify these values in the YAML file:

```yaml
// 1. Provider Fee Percentage (e.g., 0.12 = 12% fee)
// Adjust according to your electricity provider's markup
const float PROVIDER_FEE = 0.12; 

// 2. VAT Percentage (e.g., 0.22 = 22% VAT)
// Adjust according to your country's VAT rate
const float VAT_RATE = 0.22;
```

**Example calculations:**
- Raw price: 0.10 €/kWh
- Provider fee (12%): 0.01 €/kWh
- VAT (22%): 0.024 €/kWh
- **Final price: 0.134 €/kWh**

### Update Schedule

#### Version 1.0.0 (Legacy)
- **Every 15 minutes**: Sensor value updates
- **0:00 AM and 2:00 AM (as a fallback) daily**: New market data fetch

#### Version 2.0.0 (Smart Retry)
- **Every 15 minutes**: Sensor value updates
- **0:00 AM daily**: Smart price update with retry logic
- **Automatic retry**: Up to 3 attempts with 5-minute intervals
- **Data freshness check**: Skip updates if data is less than 1 hour old

To modify timing, edit the time section:

```yaml
time:
  - platform: homeassistant
    id: ha_time
    timezone: "YourTimezone"
    on_time:
      # Update sensors every 30 minutes instead of 15
      - minutes: /30
        seconds: 1
        then:
          # ... sensor updates
      # Modify smart update timing if needed
      - if:
          condition:
            lambda: 'return (id(ha_time).now().hour == 1 && id(ha_time).now().minute == 0);' # 1 AM instead of midnight
          then:
            - script.execute: smart_price_update
```

### 🆕 Smart Retry Configuration (v2.0.0)

Customize retry behavior by modifying these globals:

```yaml
globals:
  # Maximum retry attempts (default: 3)
  - id: max_retries
    type: int
    initial_value: '3'
  
  # Data freshness threshold in seconds (default: 3600 = 1 hour)
  - id: last_successful_update
    type: time_t
    initial_value: '0'
```

## 🤖 Home Assistant Automation Examples

### 1. Turn On Devices During Cheap Hours
```yaml
automation:
  - alias: "Run dishwasher during cheap electricity"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      below: 0.12 # 12 cents per kWh threshold
    condition:
      - condition: time
        after: "20:00:00"
        before: "06:00:00"
    action:
      service: switch.turn_on
      entity_id: switch.dishwasher
```

### 2. Electric Vehicle Charging Optimization
```yaml
automation:
  - alias: "Optimize EV charging based on price"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
    action:
      service: script.ev_charging_control
      data:
        current_price: "{{ states('sensor.current_electricity_price') }}"
```

Create the corresponding script:

```yaml
script:
  ev_charging_control:
    sequence:
      - variables:
          current_price: "{{ current_price | float }}"
      - condition: template
        value_template: "{{ current_price < 0.15 }}" # Only charge if price < 15 cents
      - service: switch.turn_on
        entity_id: switch.ev_charger
```

### 3. Price Spike Alert
```yaml
automation:
  - alias: "High electricity price notification"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      above: 0.25 # 25 cents per kWh
    action:
      service: notify.mobile_app_your_phone
      data:
        title: "⚡ High Electricity Price Alert"
        message: >
          Current price: {{ states('sensor.current_electricity_price') }} €/kWh
          Consider reducing power consumption
        data:
          priority: high
```

### 4. Heat Water During Low-Price Period
```yaml
automation:
  - alias: "Heat water during low price hours"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      below: 0.11 # 11 cents per kWh
    condition:
      - condition: time
        after: "22:00:00"
        before: "06:00:00"
    action:
      service: switch.turn_on
      entity_id: switch.water_heater
```

### 5. Smart Thermostat Integration
```yaml
automation:
  - alias: "Adjust thermostat based on electricity price"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
    action:
      service: climate.set_temperature
      target:
        entity_id: climate.living_room_thermostat
      data:
        temperature: >
          {% set price = states('sensor.current_electricity_price') | float %}
          {% if price < 0.10 %} # Very cheap
            22.0
          {% elif price < 0.15 %} # Moderate
            20.0
          {% else %} # Expensive
            18.0
          {% endif %}
```

### 🆕 6. Smart Retry Status Monitoring (v2.0.0)
```yaml
automation:
  - alias: "Price Update Retry Notification"
    trigger:
      - platform: state
        entity_id: sensor.price_retry_count
    condition:
      - condition: template
        value_template: "{{ trigger.to_state.state | int >= 2 }}"
    action:
      - service: notify.persistent_notification
        data:
          title: "🔄 Price Update Retry Attempt"
          message: |
            Update attempt {{ states('sensor.price_retry_count') }}/3 in progress.
            Current status: {{ states('sensor.price_status_message') }}
```

## 🔧 Troubleshooting

### Common Issues and Solutions

#### 1. Device Not Connecting to WiFi
**Symptoms**: ESP32 doesn't appear on network, no IP address assigned
**Solutions**:
- Verify WiFi credentials in `secrets.yaml`
- Ensure using 2.4GHz network (ESP32 doesn't support 5GHz)
- Check WiFi signal strength at ESP32 location
- Try restarting your router

#### 2. No Electricity Price Data
**Symptoms**: Sensors show 0.0000 or "unknown"
**Solutions**:
- Verify ENTSO-E API token is correct and active
- Check country area code matches your location
- Ensure internet connectivity from ESP32
- Check ESP Home logs for API error messages
- Verify token is approved (may take up to 24 hours)

#### 3. Home Assistant Not Finding Device
**Symptoms**: Integration setup fails, can't connect to ESP32
**Solutions**:
- Ensure ESP32 and Home Assistant are on same network
- Check firewall settings on both devices
- Try using device IP address instead of hostname
- Verify API encryption key matches exactly
- Restart ESP32 and Home Assistant

#### 4. Version 2.0.0: ESPHome Version Error
**Symptoms**: Configuration error about API actions not supported
**Solutions**:
- **Upgrade ESPHome to version 2025.12.0 or later**
- Docker: Use `latest` tag or specific version tag
- Python: Run `pip install --upgrade esphome`
- Home Assistant Add-on: Update to latest version

#### 5. Version 2.0.0: Status Sensors Not Appearing
**Symptoms**: Status monitoring sensors missing in Home Assistant
**Solutions**:
- Ensure API actions are enabled in ESPHome integration
- Check "Allow the device to perform Home Assistant actions" is checked
- Verify ESPHome device is connected and online
- Check ESPHome logs for API action errors

#### 6. Smart Retry Not Working
**Symptoms**: No automatic retries, status always shows failed
**Solutions**:
- Verify Home Assistant API actions are enabled
- Check network connectivity between ESP32 and HA
- Review ESPHome logs for retry logic messages
- Test manual force update button

#### 7. Inaccurate Price Calculations
**Symptoms**: Prices seem wrong compared to official sources
**Solutions**:
- Adjust provider fee percentage in YAML
- Verify VAT rate for your country
- Check if your area uses different currency
- Consider transmission/distribution fees

#### 8. ESP32 Restarts or Crashes
**Symptoms**: Device disconnects frequently, logs show watchdog resets
**Solutions**:
- Check power supply (use quality USB cable)
- Ensure stable WiFi connection
- Reduce logging level to reduce memory usage
- Check for memory leaks in custom code

### Debug Mode
Enable debug logging to troubleshoot issues:

```yaml
logger:
  level: DEBUG # Change from INFO to DEBUG
  logs:
    esphttpclient: DEBUG
    http_request: DEBUG
```

### Log Analysis
Common log messages and their meanings:

**[INFO][entsoe]: Successfully updated all prices and found 96 prices.**
✅ **Good**: Data fetched successfully

**[INFO][entsoe]: Price update SUCCESS after 0 attempts**
✅ **Version 2.0.0**: Smart retry succeeded on first attempt

**[WARN][entsoe]: Failed to parse prices. Found only 45 points.**
⚠️ **Warning**: Incomplete data, may be temporary API issue

**[WARN][entsoe]: Price update FAILED, attempt 1/3**
⚠️ **Version 2.0.0**: Smart retry in progress, will retry automatically

**[WARN][entsoe]: HTTP Request failed. Code: 401**
❌ **Error**: API authentication failed, check token

**[WARN][entsoe]: HTTP Request failed. Code: 429**
⚠️ **Warning**: Rate limit exceeded, too many requests

**[ERROR][entsoe]: ALL UPDATE ATTEMPTS FAILED - ALERT REQUIRED**
🚨 **Version 2.0.0**: Critical failure - check status sensors for details

## 🆕 Version 2.0.0 Migration Guide

### Upgrading from Version 1.0.0

1. **Backup Current Configuration**
   ```bash
   # Copy your current YAML configuration
   cp current_config.yaml current_config_backup.yaml
   ```

2. **Upgrade ESPHome**
   - **Docker**: Update to latest version
   - **Python**: `pip install --upgrade esphome`
   - **HA Add-on**: Update to latest version

3. **Replace Configuration**
   - Use the new `entsoe-hass-compatible.yaml` (v2.0.0)
   - Update your `secrets.yaml` with same values
   - Preserve your WiFi and API settings

4. **Enable API Actions**
   - In Home Assistant, go to ESPHome integration
   - Click "Configure"
   - Check "Allow the device to perform Home Assistant actions"
   - Click "Submit"

5. **Add Status Monitoring**
   - Copy the provided Home Assistant automations
   - Add the status dashboard card
   - Test the smart retry logic

6. **Verify Installation**
   - Check that status sensors appear in Home Assistant
   - Test force update button
   - Monitor logs for smart retry messages

### Breaking Changes in v2.0.0
- **Requires ESPHome 2025.12.0+**
- **Changed update schedule** (single midnight fetch with retries)
- **New status sensors** require HA configuration
- **API actions must be enabled** for full functionality

## 📈 Data Source Information

### ENTSO-E Transparency Platform
The data comes from the ENTSO-E (European Network of Transmission System Operators for Electricity) Transparency Platform, which provides:
- **Real-time electricity prices** for European markets
- **15-minute granularity** for most markets
- **Historical data** for analysis
- **Official market data** from transmission system operators

### Data Update Frequency
- **Prices**: Updated every 15 minutes
- **Market data**: Published with 1-2 hour delay
- **Historical data**: Available for up to 2 years

### Data Quality
- **Accuracy**: Official market prices from TSOs
- **Reliability**: High availability (>99%)
- **Coverage**: 35+ European countries and regions

## 🔒 Security Considerations

### API Token Security
- **Never share** your ENTSO-E API token publicly
- **Store securely** in `secrets.yaml` file
- **Rotate regularly** if using extensively
- **Monitor usage** through ENTSO-E portal

### Network Security
- **Use WPA2/WPA3** WiFi encryption
- **Keep ESP32 firmware** updated
- **Monitor network traffic** for unusual activity
- **Consider VLAN isolation** for IoT devices

### Home Assistant Security
- **Use strong passwords** for all accounts
- **Enable two-factor authentication** when possible
- **Regularly update** Home Assistant
- **Review automation permissions** regularly

### Version 2.0.0 Additional Security
- **API Actions**: Only enable if needed for status monitoring
- **Status Sensors**: Monitor access to update status information
- **Bidirectional Communication**: Review HA automation permissions

## 🚀 Advanced Features

### Custom Price Calculations
You can modify the price calculation logic in the script section to include:
- **Time-of-use tariffs**
- **Peak/off-peak multipliers**
- **Grid stability fees**
- **Renewable energy certificates**

### External Data Integration
The JSON hourly prices sensor can be integrated with:
- **InfluxDB** for historical analysis
- **Grafana** for visualization
- **Python scripts** for custom calculations
- **Node-RED** for complex automation flows

### Multi-Zone Support
Modify the configuration to support multiple areas:

```yaml
# Add multiple area codes
- id: area_code_1
  type: std::string
  initial_value: '"10YDE-RENEW----U"' # Germany
- id: area_code_2
  type: std::string
  initial_value: '"10YFR-RTE------B"' # France

# Modify API call to use selected areas
std::string url = "https://web-api.tp.entsoe.eu/api?securityToken=" + id(api_token) + 
                  "&documentType=A44&in_Domain="+ id(selected_area_code) + 
                  "&out_Domain="+ id(selected_area_code);
```

### Version 2.0.0 Advanced Features

#### Smart Retry Customization
```yaml
# Custom retry timing
script:
  - id: smart_price_update
    then:
      # Modify retry delays
      - delay: 120s  # 2 minutes instead of 1
      # Custom retry count
      - lambda: id(max_retries) = 5; // 5 attempts instead of 3
```

#### Enhanced Status Monitoring
```yaml
# Add custom status sensors
- platform: template
  name: "API Response Time"
  lambda: return id(api_response_time);
  
- platform: template
  name: "Data Freshness Hours"
  lambda: return (id(ha_time).now().timestamp - id(last_successful_update)) / 3600.0;
```

## 📝 Changelog

See [CHANGELOG.md](CHANGELOG.md) for detailed version history and new features.

### Version 2.0.0 Highlights
- **Smart Retry Logic**: Intelligent update system with automatic retries
- **Status Monitoring**: Real-time update status and retry tracking
- **Bidirectional Communication**: Home Assistant integration for status verification
- **Data Freshness Check**: Prevents unnecessary API calls
- **Critical Failure Detection**: Automatic alerts for system issues

## 🤝 Contributing

Contributions are welcome! Please feel free to submit:
- **Bug reports** with detailed logs
- **Feature requests** for new functionality
- **Country code updates** if you find missing areas
- **Documentation improvements** for clarity
- **Code optimizations** for performance
- **Smart retry enhancements** for better reliability

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly with ESPHome 2025.12.0+
5. Submit a pull request

## 📄 License

This project is open source and available under the MIT License. You are free to:
- Use this code for personal and commercial purposes
- Modify and distribute the code
- Include it in other projects
- Sell devices using this firmware

## 📞 Support

### Getting Help
1. **Check the troubleshooting section** above
2. **Review ESP Home logs** for error messages
3. **Verify API token** on ENTSO-E transparency platform
4. **Test network connectivity** from ESP32
5. **Check ESPHome version** (v2.0.0 requires 2025.12.0+)

### Community Resources
- **ESP Home Discord**: Real-time help from developers
- **Home Assistant Community Forum**: Automation examples
- **ENTSO-E API Documentation**: Official API reference

### Version 2.0.0 Specific Support
- **Smart Retry Issues**: Check status sensors and HA API actions
- **ESPHome Compatibility**: Ensure version 2025.12.0 or later
- **Status Monitoring**: Verify API actions are enabled in HA

## 🏷️ Project Information

- **Version**: 2.0.0
- **Last Updated**: December 18, 2025
- **Compatibility**: 
  - ESP Home 2025.12.0+ (v2.0.0)
  - ESP Home 2024.1+ (v1.0.0)
  - Home Assistant 2023.1+
- **License**: MIT
- **Author**: Legolas-2025 with AI assistance (MiniMax Agent and Google Gemini)

**Happy monitoring!** ⚡📊

For the latest updates and examples, visit the project repository and community forums.
