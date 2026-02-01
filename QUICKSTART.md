# ESP32 Electricity Price Ticker - Quick Start Guide

Get your ESP32 electricity price ticker running in under 30 minutes with the improved v4.2.0 version!

## What You Need

- **Hardware**: ESP32 development board (any model works)
- **Software**: ESPHome dashboard (web interface)
- **API**: Free ENTSO-E API token
- **Network**: WiFi connection
- **Files**: `entso-e-prices.yaml`, two C++ helper files, and **TWO Home Assistant automations** (REQUIRED)

## ⚠️ CRITICAL: External Automations Required

> **v4.2.0 uses EXTERNAL Home Assistant automations instead of internal scheduling.**
>
> **Without importing both automation files from `v4.2.0/crucial_ha_automations/`, your prices will NEVER update.**
>
> This provides more flexibility—you can modify timing, retry logic, and notifications without recompiling firmware.

### The Two Required Automations

1. **entsoe_midnight_automation.yaml**
   - Runs at 00:00:00
   - Promotes tomorrow's prices from NVS storage
   - Falls back to NVS load if promote fails
   - Falls back to HTTP fetch with retries if NVS fails
   - Sends notifications on success or failure

2. **entsoe_next_day_automation.yaml**
   - Starts at 13:50:00
   - Triggers next-day price fetch
   - Retries every 30 minutes until 20:50 if needed
   - Stops immediately on success
   - Sends notifications on success or failure

**You MUST replace `notify.mobile_app_sm_s911b` with your own notification entity.**

## 5-Minute Setup

### Step 1: Get ENTSO-E API Token (2 minutes)

1. Visit [ENTSO-E Transparency Platform](https://transparency.entsoe.eu/)
2. Click "How to register?"
3. Complete registration and get your API token
4. **Save the token** - you will need it in Step 4

### Step 2: Install ESPHome Dashboard (3 minutes)

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

### Step 3: Prepare Project Files (5 minutes)

1. **Download** `entso-e-prices.yaml` from the project repository (v4.2.0 folder)
2. **Download** both helper files from the v4.2.0 folder:
   - `entsoe_storage_v2.h` - NVS storage for persistent data
   - `entsoe_http_idf.h` - Optimized HTTP client for ESP-IDF
3. **Download** `secrets_template.yaml` from v4.2.0 folder and copy to `secrets.yaml`
4. **Download** both automation files from `v4.2.0/crucial_ha_automations/` folder:
   - `entsoe_midnight_automation.yaml`
   - `entsoe_next_day_automation.yaml`

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

### Step 5: Import Home Assistant Automations (3 minutes)

> **THIS STEP IS CRITICAL - AUTOMATIONS ARE REQUIRED**

1. Go to **Settings → Automations** in Home Assistant
2. Click the **⋮ menu** (three dots) in the top right
3. Select **Import from file**
4. Select `entsoe_midnight_automation.yaml`
5. Repeat for `entsoe_next_day_automation.yaml`

**Configure Notifications:**

1. Open both automation files in a text editor
2. Search for `notify.mobile_app_sm_s911b`
3. Replace with your own mobile notification entity
   - Find your entity: Go to **Settings → Devices & Services → Notifications**
   - Your device will have an entity like `notify.mobile_app_your_phone_name`
4. Save and reload automations

### Step 6: Important - Select ESP-IDF Framework (3 minutes)

When you first compile the project in ESPHome dashboard:

1. **ESPHome will prompt** you to choose a framework
2. **Select "ESP-IDF"** (NOT Arduino)
3. This is required for:
   - NVS storage functionality
   - Stable operation without random reboots
   - Optimized HTTP client performance

**Why ESP-IDF?** The v4.2.0 version uses ESP-IDF framework instead of Arduino to fix the random reboot issues that occurred in v3.5.0. ESP-IDF provides more stable operation and better memory management.

### Step 7: Flash ESP32 (5 minutes)

1. **Connect** ESP32 to computer via USB
2. **Click** "Install" in ESPHome dashboard
3. **Select** your device port
4. **Wait** for compilation and upload (3-4 minutes first time)
5. **Watch** the logs for successful startup messages

### Step 8: Add to Home Assistant (5 minutes)

1. **Go to** Home Assistant → Integrations
2. **Click** "Add Integration"
3. **Search** for "ESPHome"
4. **Enter** device IP address (shown in ESP Home dashboard)
5. **Done!** Your electricity price sensors are now available

### Step 9: Verify Automations Are Working

1. Check Settings → Automations - both automations should be enabled
2. Verify you replaced the notification entity (not `notify.mobile_app_sm_s911b`)
3. After 13:50, check `sensor.entso_e_prices_next_day_price_update_status` shows "SUCCESS"
4. At midnight, verify today's prices update
5. Check persistent notifications appear in HA

## What You Get

After setup, you will have these sensors in Home Assistant:

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

## Automation Examples

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

## What is New in v4.2.0

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

### External Automations (REQUIRED)

v4.2.0 requires two external Home Assistant automations for updates:

1. **Midnight Automation** (`entsoe_midnight_automation.yaml`):
   - Runs at 00:00:00
   - NVS promote → NVS fallback → HTTP retry
   - Notifications on success/failure

2. **Next-Day Automation** (`entsoe_next_day_automation.yaml`):
   - Starts at 13:50:00
   - Retries every 30 minutes until 20:50
   - Stops immediately on success
   - Notifications on success/failure

**You must import both from `v4.2.0/crucial_ha_automations/` and configure mobile notifications.**

### Two Required Helper Files

The v4.2.0 requires two C++ helper files that must be placed **directly alongside** `entso-e-prices.yaml`:

1. **entsoe_storage_v2.h** - Handles NVS storage operations
2. **entsoe_http_idf.h** - Optimized HTTP communication

**Important:** Do NOT put helper files in a `src/` subfolder. Place them directly in the same folder as `entso-e-prices.yaml`.

Both files are automatically included by the main YAML configuration.

## Troubleshooting

**Device not connecting to WiFi?**
- Check WiFi credentials in `secrets.yaml`
- Ensure 2.4GHz network (ESP32 does not support 5GHz)
- Verify fallback AP is configured for emergency access

**No electricity price data?**
- Verify ENTSO-E API token is correct
- Check country area code matches your location
- Look at ESP Home logs for error messages
- Ensure ESP-IDF framework was selected during compilation
- **Check automations are imported and enabled**
- **Verify notification entity was changed**

**Prices not updating?**
1. Go to Settings → Automations
2. Verify both automations exist and are enabled
3. Check automation traces for errors
4. Verify you replaced `notify.mobile_app_sm_s911b`
5. Check button entities exist
6. Look at logbook for automation runs

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

## Customization

**Adjust taxes and fees:**
In the YAML file, find these lines and adjust:
```yaml
const float PROVIDER_FEE = 0.12;     # 12% provider fee
const float VAT_RATE = 0.22;         # 22% VAT
```

**Change automation timing:**
Open the automations in Home Assistant and modify the trigger time:
- Midnight automation trigger: `00:00:00`
- Next-day automation trigger: `13:50:00`

**Change retry intervals:**
In the next-day automation, modify the 30-minute delay in the repeat loop.

**NVS storage behavior:**
The NVS storage is automatically managed. Price data is saved after each successful API fetch and restored on startup. You can monitor the storage status in the device logs.

## Need Help

- **Full Documentation**: See README.md for detailed setup
- **Country Codes**: Complete list in README.md
- **API Issues**: Check ENTSO-E transparency platform status
- **ESP Home**: Visit [ESP Home documentation](https://esphome.io/)
- **Framework Issues**: See ESP-IDF documentation
- **Automation Issues**: Check Home Assistant logbook and automation traces

## Pro Tips

1. **Monitor your electricity bill** - Use historical data to optimize usage
2. **Create time-based automations** - Heat water, charge EVs during cheap hours
3. **Use the JSON sensor** - Export data to other systems
4. **Check logs regularly** - Monitor for API issues or connectivity problems
5. **Verify NVS persistence** - After power cycle, check if price data is restored
6. **Stay on ESP-IDF** - Do not switch back to Arduino framework
7. **Test automations** - Check automation traces to verify they are running
8. **Configure notifications** - Ensure mobile alerts work for failure scenarios

## Migration from v3.5.0

If you are upgrading from v3.5.0:

1. **Backup your current configuration** and `secrets.yaml`
2. **Delete old build files** - Perform a clean build
3. **Select ESP-IDF** when prompted by ESPHome dashboard
4. **Add helper files** - Place both `.h` files directly alongside `entso-e-prices.yaml`
5. **Update secrets.yaml** - Add the new fallback AP credentials
6. **Import new automations** - Download from `v4.2.0/crucial_ha_automations/` and import
7. **Configure notifications** - Replace `notify.mobile_app_sm_s911b` with your entity
8. **Test thoroughly** - Monitor for stability over 24-48 hours
9. **Enjoy stable operation** - No more random reboots!

The migration is straightforward and the improved stability is well worth the effort. The external automation approach gives you more control over when and how prices are updated.
