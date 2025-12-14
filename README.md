# ESP32 Electricity Price Ticker for Home Assistant
![ESP32 Electricity Price Ticker](header.png)

[![ESP32](https://img.shields.io/badge/ESP32-Ready-blue)](https://www.espressif.com/en/products/devkits)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Compatible-green)](https://www.home-assistant.io/)
[![ENTS0-E API](https://img.shields.io/badge/ENTS0--E%20API-Free-orange)](https://transparency.entsoe.eu/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub stars](https://img.shields.io/github/stars/YOUR_USERNAME/esp32-electricity-price-ticker?style=social)](https://github.com/YOUR_USERNAME/esp32-electricity-price-ticker)
[![GitHub forks](https://img.shields.io/github/forks/YOUR_USERNAME/esp32-electricity-price-ticker?style=social)](https://github.com/YOUR_USERNAME/esp32-electricity-price-ticker/fork)
[![GitHub issues](https://img.shields.io/github/issues/YOUR_USERNAME/esp32-electricity-price-ticker)](https://github.com/YOUR_USERNAME/esp32-electricity-price-ticker/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/YOUR_USERNAME/esp32-electricity-price-ticker)](https://github.com/YOUR_USERNAME/esp32-electricity-price-ticker/pulls)

A complete ESP32-based solution that fetches real-time electricity prices from the ENTSO-E API and provides comprehensive price sensors for Home Assistant automation. Perfect for optimizing electricity costs and smart home energy management across 35+ European countries. Inspired by https://github.com/JaccoR/hass-entso-e, modified to hardware version.

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
- **Automatic data refresh** at 2 AM daily
- **Manual force update button** for testing
- **Home Assistant time synchronization**

## 📋 Requirements

### Hardware
- ESP32 development board (any model: ESP32-WROOM, ESP32-DevKit, etc.)
- USB cable for programming
- Stable 2.4GHz WiFi connection

### Software
- ESP Home dashboard (web interface)
- Home Assistant instance
- ENTSO-E API account (free)

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

Choose one of these installation methods:

#### Option A: Docker (Recommended)
```bash
# Create directory for ESP Home configuration
mkdir esphome
cd esphome

# Run ESP Home in Docker
docker run -d \
  --name esphome \
  --privileged \
  -p 6052:6052 \
  -v $(pwd):/config \
  -v /etc/localtime:/etc/localtime:ro \
  ghcr.io/esphome/esphome

# Access dashboard at http://localhost:6052
```

#### Option B: Python Installation
```bash
# Install ESP Home via pip
pip install esphome

# Start dashboard
esphome dashboard

# Access dashboard at http://localhost:6052
```

#### Option C: Home Assistant Add-on
1. Open Home Assistant
2. Go to Supervisor > Add-on Store
3. Search for "ESPHome"
4. Install and start the add-on

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
7. **Click "Finish"**

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
| **Slovenia** | `10YSI-ELES-----O` | EUR |ELES (Default example)|
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
| `Average Electricity Price Today` | Daily average price | Daily at 2 AM |
| `Highest Electricity Price Today` | Maximum price today | Daily at 2 AM |
| `Lowest Electricity Price Today` | Minimum price today | Daily at 2 AM |
| `Highest Hourly Electricity Price Today` | Maximum hourly price today | Daily at 2 AM |
| `Lowest Hourly Electricity Price Today` | Minimum hourly price today | Daily at 2 AM |

### Percentage Sensor
| Sensor Name | Description | Range |
|-------------|-------------|-------|
| `Current Max Hourly Price Percentage` | Current price as % of today's max | 0-100% |

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

The default update schedule is:
- **Every 15 minutes**: Sensor value updates
- **2:00 AM daily**: New market data fetch

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
          
      # Fetch new data at 3:00 AM instead of 2:00
      - if:
          condition:
            lambda: 'return id(ha_time).now().hour == 3 && id(ha_time).now().minute == 0;'
          then:
            - script.execute: full_update_logic_script
```

## 🤖 Home Assistant Automation Examples

### 1. Turn On Devices During Cheap Hours

```yaml
automation:
  - alias: "Run dishwasher during cheap electricity"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      below: 0.12  # 12 cents per kWh threshold
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
        value_template: "{{ current_price < 0.15 }}"  # Only charge if price < 15 cents
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
      above: 0.25  # 25 cents per kWh
    action:
      service: notify.mobile_app_your_phone
      data:
        title: "⚡ High Electricity Price Alert"
        message: >
          Current price: {{ states('sensor.current_electricity_price') }} €/kWh
          Consider reducing power consumption
        data:
          priority: high
          actions:
            - action: "reduce_consumption"
              title: "Reduce Consumption"
            - action: "ignore"
              title: "Ignore"
```

### 4. Heat Water During Low-Price Period

```yaml
automation:
  - alias: "Heat water during low price hours"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      below: 0.11  # 11 cents per kWh
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
          {% if price < 0.10 %}  # Very cheap
            22.0
          {% elif price < 0.15 %}  # Moderate
            20.0
          {% else %}  # Expensive
            18.0
          {% endif %}
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

#### 4. Inaccurate Price Calculations
**Symptoms**: Prices seem wrong compared to official sources

**Solutions**:
- Adjust provider fee percentage in YAML
- Verify VAT rate for your country
- Check if your area uses different currency
- Consider transmission/distribution fees

#### 5. ESP32 Restarts or Crashes
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
  level: DEBUG  # Change from INFO to DEBUG
  logs:
    esphttpclient: DEBUG
    http_request: DEBUG
```

### Log Analysis

Common log messages and their meanings:

```
[INFO][entsoe]: Successfully updated all prices and found 96 prices.
```
✅ **Good**: Data fetched successfully

```
[WARN][entsoe]: Failed to parse prices. Found only 45 points.
```
⚠️ **Warning**: Incomplete data, may be temporary API issue

```
[WARN][entsoe]: HTTP Request failed. Code: 401
```
❌ **Error**: API authentication failed, check token

```
[WARN][entsoe]: HTTP Request failed. Code: 429
```
⚠️ **Warning**: Rate limit exceeded, too many requests

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
  initial_value: '"10YDE-RENEW----U"'  # Germany
- id: area_code_2
  type: std::string
  initial_value: '"10YFR-RTE------B"'  # France

# Modify API call to use selected area
std::string url = "https://web-api.tp.entsoe.eu/api?securityToken=" + id(api_token) + 
                  "&documentType=A44&in_Domain=" + id(selected_area_code) + 
                  "&out_Domain=" + id(selected_area_code);
```

## 📝 Changelog

### Version 1.0.0
- Initial release
- 15-minute price monitoring
- Hourly average calculations
- VAT and fee calculations
- Home Assistant integration
- Support for 35+ European countries

## 🤝 Contributing

Contributions are welcome! Please feel free to submit:
- **Bug reports** with detailed logs
- **Feature requests** for new functionality
- **Country code updates** if you find missing areas
- **Documentation improvements** for clarity
- **Code optimizations** for performance

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

### Community Resources
- **ESP Home Discord**: Real-time help from developers
- **Home Assistant Community Forum**: Automation examples
- **ENTSO-E API Documentation**: Official API reference

### Professional Support
For commercial implementations or custom modifications, consider:
- **Professional ESP Home consultants**
- **Home Assistant installation services**
- **Custom firmware development**

## 🏷️ Project Information

- **Version**: 1.0.0
- **Last Updated**: December 2025
- **Compatibility**: ESP Home 2024.1+, Home Assistant 2023.1+
- **License**: MIT
- **Author**: MiniMax Agent

---

**Happy monitoring!** ⚡📊

For the latest updates and examples, visit the project repository and community forums.
