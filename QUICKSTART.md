# ESP32 Electricity Price Ticker - Quick Start Guide

Get your ESP32 electricity price ticker running in under 30 minutes!

## 📋 What You Need

- **Hardware**: ESP32 development board (any model works)
- **Software**: ESP Home dashboard (web interface)
- **API**: Free ENTSO-E API token
- **Network**: WiFi connection

## 🚀 5-Minute Setup

### Step 1: Get ENTSO-E API Token (2 minutes)
1. Visit [ENTSO-E Transparency Platform](https://transparency.entsoe.eu/)
2. Click "How to register?" 
3. Complete registration and get your API token
4. **Save the token** - you'll need it in Step 3

### Step 2: Install ESP Home Dashboard (3 minutes)
Choose one option:

**Option A: Docker (Recommended)**
```bash
docker run -d \
  --name esphome \
  --privileged \
  -p 6052:6052 \
  -v /path/to/esphome:/config \
  -v /etc/localtime:/etc/localtime:ro \
  ghcr.io/esphome/esphome
```

**Option B: Python pip**
```bash
pip install esphome
esphome dashboard
```

**Option C: Home Assistant Add-on**
Install "ESPHome" add-on from Home Assistant supervisor

### Step 3: Configure Your Device (5 minutes)

1. **Create new project** in ESP Home dashboard
2. **Copy** `entsoe-hass-compatible.yaml` content
3. **Create** `secrets.yaml` file with your data:

```yaml
wifi_ssid: "YourWiFiName"
wifi_password: "YourWiFiPassword"
api_encryption_key: "YOUR_HOME_ASSISTANT_API_KEY"
entsoe_api_token: "YOUR_ENTSOE_API_TOKEN"
country_area_code: "10YSI-ELES-----"  # For Slovenia
timezone: "Europe/Ljubljana"
```

4. **Find your country code** in the full README (see country codes list)

### Step 4: Flash ESP32 (5 minutes)

1. **Connect** ESP32 to computer via USB
2. **Click** "Install" in ESP Home dashboard
3. **Select** your device port
4. **Wait** for compilation and upload (2-3 minutes)

### Step 5: Add to Home Assistant (5 minutes)

1. **Go to** Home Assistant → Integrations
2. **Click** "Add Integration"
3. **Search** for "ESPHome"
4. **Enter** device IP address (shown in ESP Home dashboard)
5. **Done!** Your electricity price sensors are now available

## 📊 What You Get

After setup, you'll have these sensors in Home Assistant:

### Real-time Price Sensors
- **Current Electricity Price** - Price right now (€/kWh)
- **Next Electricity Price** - Price in next 15 minutes (€/kWh)
- **Current Hourly Price** - Average price for current hour (€/kWh)
- **Next Hourly Price** - Average price for next hour (€/kWh)

### Daily Statistics
- **Average Price Today** - Daily average (€/kWh)
- **Highest Price Today** - Peak price today (€/kWh)
- **Lowest Price Today** - Minimum price today (€/kWh)
- **Current Max Percentage** - How current price compares to today's max (%)

### Timing Information
- **Time of Highest Price Today** - When peak price occurs
- **Time of Lowest Price Today** - When minimum price occurs
- **Hourly Price JSON** - All 24 hourly prices in JSON format

### Automation Examples

**Turn on cheap devices during low-price hours:**
```yaml
automation:
  - alias: "Use electricity when price is low"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      below: 0.15  # 15 cents per kWh
    action:
      service: switch.turn_on
      entity_id: switch.your_device
```

**Notify when price spikes:**
```yaml
automation:
  - alias: "High electricity price alert"
    trigger:
      platform: numeric_state
      entity_id: sensor.current_electricity_price
      above: 0.25  # 25 cents per kWh
    action:
      service: notify.mobile_app_your_phone
      data:
        title: "High electricity price!"
        message: "Current price: {{ states('sensor.current_electricity_price') }} €/kWh"
```

## 🛠️ Troubleshooting

**Device not connecting to WiFi?**
- Check WiFi credentials in `secrets.yaml`
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)

**No electricity price data?**
- Verify ENTSO-E API token is correct
- Check country area code matches your location
- Look at ESP Home logs for error messages

**Home Assistant not finding device?**
- Ensure ESP32 and Home Assistant are on same network
- Check firewall settings
- Try using device IP instead of hostname

## 🔧 Customization

**Adjust taxes and fees:**
In the YAML file, find these lines and adjust:
```yaml
const float PROVIDER_FEE = 0.12;     // 12% provider fee
const float VAT_RATE = 0.22;         // 22% VAT
```

**Change update frequency:**
Default is every 15 minutes. Modify in the time section.

## 📞 Need Help?

- **Full Documentation**: See README.md for detailed setup
- **Country Codes**: Complete list in README.md
- **API Issues**: Check ENTSO-E transparency platform status
- **ESP Home**: Visit [ESP Home documentation](https://esphome.io/)

## ⚡ Pro Tips

1. **Monitor your electricity bill** - Use historical data to optimize usage
2. **Create time-based automations** - Heat water, charge EVs during cheap hours
3. **Use the JSON sensor** - Export data to other systems
4. **Check logs regularly** - Monitor for API issues or connectivity problems

---

**Next Steps**: Read the full README.md for advanced configuration, country codes, and troubleshooting guides!