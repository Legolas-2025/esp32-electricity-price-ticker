# ESP32 Electricity Price Ticker - Quick Start Guide

Get your ESP32 electricity price ticker running in under 30 minutes with the improved v4.2.0 version!

## 📋 What You Need

- **Hardware**: ESP32 development board (any model works)
- **Software**: ESP Home dashboard (web interface)
- **API**: Free ENTSO-E API token
- **Network**: WiFi connection
- **Files**: `entso-e-prices.yaml` and two C++ helper files (see below)

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

> **⚠️ IMPORTANT: v4.2.0 is Different from v3.5.0**
>
> Unlike v3.5.0 where some values were hardcoded in the YAML file and required manual editing, **v4.2.0 puts ALL credentials in secrets.yaml**. The main YAML file (`entso-e-prices.yaml`) uses `!secret` directives for all configurable values.
>
> **No editing of entso-e-prices.yaml is required** - just download it, create secrets.yaml with your values, and compile!

### Step 3: Prepare Project Files (5 minutes)

1. **Download** `entso-e-prices.yaml` from the project repository (v4.2.0 folder)
2. **Download** both helper files from the v4.2.0 folder:
   - `entsoe_storage_v2.h` - NVS storage for persistent data
   - `entsoe_http_idf.h` - Optimized HTTP client for ESP-IDF
3. **Download** `secrets_template.yaml` from v4.2.0 folder and copy to `secrets.yaml`
4. **⚠️ IMPORTANT: Create the two Home Assistant automations** (the price ticker v4.2.0 will NOT function correctly without them), they are available in the v4.2.0/crucial_ha_automations/ subfolder (ENTSO-E v420 HA Automation - ENTSO-E v420 Robust Next-Day HTTP Update 13_50–20_50.yaml and ENTSO-E v420 HA Automation - Midnight Promote - NVS Load - HTTP Fallback.yaml).

**Important: Where to Place Helper Files**

Place both `.h` helper files **directly alongside** `entso-e-prices.yaml`. ESPHome automatically includes all `.h` and `.cpp` files from the configuration directory:

**Directory Structure:**
```
your-esphome-project/
├── entso-e-prices.yaml              # Main configuration (from v4.2.0/)
├── entsoe_storage_v2.h              # NVS storage helper (from v4.2.0/)
├── entsoe_http_idf.h                # HTTP client helper (from v4.2.0/)
└── secrets.yaml                     # Your credentials (from v4.2.0/secrets_template.yaml)
```

**Do NOT put helper files in a `src/` subfolder** - place them directly alongside the main YAML file.

### Step 4: Configure Your Device (5 minutes)

1. **Create** new project in ESPHome dashboard
2. **Import** or **copy** `entso-e-prices.yaml` content
3. **Create** `secrets.yaml` file with your data:

```yaml
wifi_ssid: "YourWiFiName"
wifi_password: "YourWiFiPassword"
entsoe_api_encryption_key: "YOUR_HOME_ASSISTANT_API_KEY"
entsoe_entsoe_api_token: "YOUR_ENTSOE_API_TOKEN"
entsoe_country_area_code: "10YSI-ELES-----"  # For Slovenia
entsoe_timezone: "Europe/Ljubljana"
entsoe_fallback_ap_ssid: "ENTSOE-Fallback"
entsoe_fallback_ap_password: "fallbackpassword"
```

4. **Find your country code** in the full README (see country codes list)

### Step 5: Important - Select ESP-IDF Framework (3 minutes)

When you first compile the project in ESPHome dashboard:

1. **ESPHome will prompt** you to choose a framework
2. **Select "ESP-IDF"** (NOT Arduino)
3. This is required for:
   - NVS storage functionality
   - Stable operation without random reboots
   - Optimized HTTP client performance

**Why ESP-IDF?** The v4.2.0 version uses ESP-IDF framework instead of Arduino to fix the random reboot issues that occurred in v3.5.0. ESP-IDF provides more stable operation and better memory management.

### Step 6: Flash ESP32 (5 minutes)

1. **Connect** ESP32 to computer via USB
2. **Click** "Install" in ESPHome dashboard
3. **Select** your device port
4. **Wait** for compilation and upload (3-4 minutes first time)
5. **Watch** the logs for successful startup messages

### Step 7: Add to Home Assistant (5 minutes)

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

## 🆕 What's New in v4.2.0

### Fixed Reboot Issues
**Important**: The v3.5.0 version experienced random reboots that were initially thought to be fixed. After further investigation, we discovered that only v4.2.0 with the ESP-IDF framework truly resolves these stability issues. If you experienced random reboots with v3.5.0, upgrading to v4.2.0 with ESP-IDF will resolve this problem.

### NVS Persistent Storage
The new version includes NVS (Non-Volatile Storage) implementation:
- **Price data survives power cycles**
- **Faster startup with cached data**
- **Reduced API calls during initialization**
- Implemented via `entsoe_storage_v2.h`

### ESP-IDF Framework
Migrated from Arduino to ESP-IDF for:
- **Better stability** - No more random reboots
- **Improved memory management** - More reliable operation
- **Native ESP32 optimization** - Takes full advantage of hardware
- **Enhanced networking** - Better HTTP client performance

### Two Required Helper Files
The v4.2.0 requires two C++ helper files that must be placed **directly alongside** `entso-e-prices.yaml`:

1. **entsoe_storage_v2.h** - Handles NVS storage operations
2. **entsoe_http_idf.h** - Optimized HTTP communication

**Important:** Do NOT put helper files in a `src/` subfolder. Place them directly in the same folder as `entso-e-prices.yaml`.

Both files are automatically included by the main YAML configuration.

## 🛠️ Troubleshooting

**Device not connecting to WiFi?**
- Check WiFi credentials in `secrets.yaml`
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Verify fallback AP is configured for emergency access

**No electricity price data?**
- Verify ENTSO-E API token is correct
- Check country area code matches your location
- Look at ESP Home logs for error messages
- Ensure ESP-IDF framework was selected during compilation

**Compilation errors about missing headers?**
- Verify both helper files are in the same directory as `entso-e-prices.yaml`
- Check file names match exactly (case-sensitive)
- Ensure ESP-IDF framework is selected (not Arduino)

**Random reboots?**
- **This should not happen in v4.2.0** - If it does, ensure you selected ESP-IDF framework
- Check power supply quality
- Review logs for any error messages before reboot
- If reboots persist, perform a full clean build

**Home Assistant not finding device?**
- Ensure ESP32 and Home Assistant are on same network
- Check firewall settings
- Try using device IP instead of hostname
- Verify API encryption key matches in both configurations

## 🔧 Customization

**Adjust taxes and fees:**
In the YAML file, find these lines and adjust:
```yaml
const float PROVIDER_FEE = 0.12;     # 12% provider fee
const float VAT_RATE = 0.22;         # 22% VAT
```

**Change update frequency:**
Default is every 15 minutes. Modify in the time section.

**NVS storage behavior:**
The NVS storage is automatically managed. Price data is saved after each successful API fetch and restored on startup. You can monitor the storage status in the device logs.

## 📞 Need Help?

- **Full Documentation**: See README.md for detailed setup
- **Country Codes**: Complete list in README.md
- **API Issues**: Check ENTSO-E transparency platform status
- **ESP Home**: Visit [ESP Home documentation](https://esphome.io/)
- **Framework Issues**: See ESP-IDF documentation

## ⚡ Pro Tips

1. **Monitor your electricity bill** - Use historical data to optimize usage
2. **Create time-based automations** - Heat water, charge EVs during cheap hours
3. **Use the JSON sensor** - Export data to other systems
4. **Check logs regularly** - Monitor for API issues or connectivity problems
5. **Verify NVS persistence** - After power cycle, check if price data is restored
6. **Stay on ESP-IDF** - Don't switch back to Arduino framework

## ⚠️ Migration from v3.5.0

If you are upgrading from v3.5.0:

1. **Backup your current configuration** and `secrets.yaml`
2. **Delete old build files** - Perform a clean build
3. **Select ESP-IDF** when prompted by ESPHome dashboard
4. **Add helper files** - Place both `.h` files directly alongside `entso-e-prices.yaml`
5. **Update secrets.yaml** - Add the new fallback AP credentials
6. **Test thoroughly** - Monitor for stability over 24-48 hours
7. **Enjoy stable operation** - No more random reboots!

The migration is straightforward and the improved stability is well worth the effort.

---
