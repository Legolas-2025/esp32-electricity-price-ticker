# Changelog
All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.2.0] - 2026-02-01
### ⚠️ CRITICAL: External Home Assistant Automations Required
**VERSION 4.2.0 INTRODUCES A FUNDAMENTAL ARCHITECTURAL CHANGE:** The main `entso-e-prices.yaml` file no longer performs automatic updates on its own. Instead, v4.2.0 relies on **two external Home Assistant automations** that you MUST import from the `v4.2.0/crucial_ha_automations/` folder. Without these automations, your prices will never update.

This change provides significantly more flexibility for modifying update timing, retry logic, and error handling through Home Assistant's automation system without needing to edit the ESPHome YAML configuration.

### Added
* **Essential Home Assistant Automations (v4.2.0/crucial_ha_automations/)**
  + `entsoe_midnight_automation.yaml` - Handles midnight NVS promote, NVS load fallback, and HTTP retries for today's prices
  + `entsoe_next_day_automation.yaml` - Handles 13:50 start and retry logic until 20:50 for tomorrow's prices
  + Both automations include comprehensive status monitoring and mobile notifications
  + Users can customize timing, retry intervals, and notification preferences directly in HA
  + **CRITICAL**: You must import both automation files for the system to function

### Changed
* **Removed Internal Scheduling from entso-e-prices.yaml**
  + v4.2.0 no longer contains internal `on_time` triggers for price updates
  + All scheduling logic has been moved to external HA automations
  + The YAML file now acts as a passive sensor platform awaiting external triggers
  + This separation provides cleaner separation of concerns and easier customization
  + **IMPORTANT**: If you were relying on v3.5.0's internal scheduling, you MUST set up the external automations

### Migration Notes
* **Required Action**: Download both automation files from `v4.2.0/crucial_ha_automations/` and import them into Home Assistant
* **Configure Notifications**: Search for `notify.mobile_app_sm_s911b` in the automation files and replace with your own mobile notification entity
* **No Internal Scheduling**: The main YAML no longer schedules updates - external automations are REQUIRED
* **Flexibility**: You can now modify update timing, retry logic, and notifications without recompiling the firmware

### Automation Details

#### Midnight Automation (entsoe_midnight_automation.yaml)
- **Trigger**: At 00:00:00 (midnight)
- **Primary Action**: Attempts to promote tomorrow's prices from NVS storage (tomorrow96 → today96)
- **Fallback**: If NVS promote fails, loads today's prices from today96 NVS storage
- **Second Fallback**: If NVS load fails, performs HTTP fetch with up to 3 retries
- **Notifications**: Creates persistent notifications for success/failure and sends mobile alerts on final failure
- **Entities Used**: 
  - `esphome.entso_e_prices_promote_and_load_today_from_nvs`
  - `esphome.entso_e_prices_clear_today_prices`
  - `esphome.entso_e_prices_clear_tomorrow_prices`
  - `esphome.entso_e_prices_load_today_from_nvs`
  - `button.entso_e_prices_entso_e_force_update`

#### Next-Day Automation (entsoe_next_day_automation.yaml)
- **Trigger**: At 13:50:00 (starts early to catch ENTSO-E publication)
- **Primary Action**: Presses "Force Next Day Update" button
- **Retry Logic**: If update fails, retries every 30 minutes until 20:50
- **Early Termination**: Stops retrying as soon as update succeeds
- **Notifications**: Creates persistent notifications for success/failure and sends mobile alerts on final failure
- **Entities Used**:
  - `button.entso_e_prices_entso_e_force_next_day_update`
  - `sensor.entso_e_prices_next_day_price_update_status`
  - `sensor.entso_e_prices_next_day_current_price_status`

### Version 4.2.0: Framework Migration & Definitive Stability
**MAJOR STABILITY RELEASE**: This version definitively resolves the random reboot issues that v3.5.0 attempted to fix, while introducing NVS persistent storage and external automation control for maximum flexibility.

The core ENTSO-E parsing logic, forward-fill algorithm, and all sensor IDs remain unchanged. Only the scheduling, execution model, and automation trigger mechanism were redesigned to provide definitive stability and user-configurable timing.

### Added
* **ESP-IDF Framework Migration**
  + Migrated from Arduino framework to ESP-IDF (Espressif IoT Development Framework)
  + Provides native ESP32 optimization for improved stability
  + Better memory management under concurrent operations
  + Enhanced task scheduling with RTOS-based approach
  + **CRITICAL**: Must select ESP-IDF when compiling in ESPHome dashboard
  + Clean build required (delete old `.esphome` directory)

* **NVS Persistent Storage**
  + Implements Non-Volatile Storage for price data persistence across power cycles
  + Automatic storage of fetched price data in flash memory
  + Automatic retrieval of cached data on device boot
  + Data integrity validation before use
  + Benefits:
    - Price data survives power outages and forced reboots
    - Instant sensor availability after device restart
    - Reduced API load during startup periods
    - Improved user experience during brief power interruptions
  + Implementation via `entsoe_storage_v2.h` helper file

* **Optimized HTTP Client for ESP-IDF**
  + New `entsoe_http_idf.h` helper file provides ESP-IDF-native HTTP communication
  + Better connection handling and reuse
  + Improved error recovery logic
  + Native ESP-IDF DNS resolution
  + More reliable timeout handling

* **External Automation Control**
  + All scheduling logic moved to external Home Assistant automations
  + Provides maximum flexibility for customizing update timing
  + Users can modify retry logic and notifications without firmware changes
  + See `v4.2.0/crucial_ha_automations/` for required automation files
  + **MUST IMPORT**: Both automation files are required for system operation

* **Updated Secrets Structure**
  + All secrets now consistently use `entsoe_` prefix
  + WiFi and API credentials centralized in secrets.yaml
  + New fallback AP credentials for emergency access
  + New structure:
    ```yaml
    wifi_ssid: "YourWiFiNetworkName"
    wifi_password: "YourWiFiPassword"
    entsoe_api_encryption_key: "YOUR_HOME_ASSISTANT_API_KEY"
    entsoe_entsoe_api_token: "YOUR_ENTSOE_API_TOKEN"
    entsoe_country_area_code: "10YSI-ELES-----O"
    entsoe_timezone: "Europe/YourCity"
    entsoe_fallback_ap_ssid: "ENTSOE-Fallback"
    entsoe_fallback_ap_password: "fallbackpassword"
    ```

### Changed
* **Framework Migration (Arduino → ESP-IDF)**
  + All core functionality now runs on ESP-IDF framework
  + Provides definitive fix for random reboot issues
  + Requires ESP-IDF selection during compilation
  + Clean build required for migration

* **Removed Internal Scheduling**
  + v3.5.0's internal `on_time` triggers have been removed
  + All update triggering now handled by external HA automations
  + Main YAML file now passive - awaits external button presses
  + **CRITICAL**: Requires importing automations from `v4.2.0/crucial_ha_automations/`

* **Helper Files Required**
  + v4.2.0 requires two C++ helper files in the same directory as YAML:
    - `entsoe_storage_v2.h` - NVS storage implementation
    - `entsoe_http_idf.h` - HTTP client for ESP-IDF
  + Place directly alongside `entso-e-prices.yaml`, NOT in `src/` subfolder

### Fixed
* **Random Reboots During Scheduled Updates**
  + ESP-IDF framework eliminates watchdog-triggered reboots
  + Root causes addressed:
    - Arduino framework memory management issues
    - Task scheduling conflicts during concurrent operations
    - Watchdog timer triggers under HTTP load
  + **IMPORTANT**: Although v3.5.0 claimed to fix reboots, only v4.2.0 with ESP-IDF definitively resolves the underlying stability issues

* **Power Cycle Data Loss**
  + NVS storage ensures price data survives power cycles
  + Instant sensor availability after reboot
  + No waiting for API response on startup

### Technical Improvements
* **ESP-IDF Native Implementation**
  + Framework optimized for ESP32 hardware
  + RTOS-based task scheduling
  + Native networking stack
  + Professional-grade memory management

* **NVS Storage Architecture**
  + Flash-based persistent storage
  + Automatic save/load operations
  + Data integrity validation
  + Efficient storage format for price arrays

* **External Automation Pipeline**
  + Clean separation between sensor platform and scheduling logic
  + Home Assistant native automation capabilities
  + Easier customization and debugging
  + Better integration with HA ecosystem

### Compatibility
* **ESPHome**: Requires 2025.12.0+ (for ESP-IDF support)
* **Home Assistant**: No changes required; all existing integrations work
* **API**: ENTSO-E API (no changes required)
* **Hardware**: ESP32 boards with sufficient flash for NVS partition
* **Existing Automations**: Must be replaced with new automations from `v4.2.0/crucial_ha_automations/`
* **Sensor IDs**: All sensors from v3.5.0 preserved unchanged

### Migration Notes
1. **Download Required Files from v4.2.0/**
   - `entso-e-prices.yaml` - Main configuration (ESP-IDF)
   - `secrets_template.yaml` - Template for credentials
   - `entsoe_storage_v2.h` - NVS storage helper
   - `entsoe_http_idf.h` - HTTP client helper
   - `entsoe_midnight_automation.yaml` - Midnight automation (REQUIRED)
   - `entsoe_next_day_automation.yaml` - Next-day automation (REQUIRED)

2. **Import Automations into Home Assistant**
   - Go to Settings → Automations → Import from file
   - Import `entsoe_midnight_automation.yaml`
   - Import `entsoe_next_day_automation.yaml`
   - **IMPORTANT**: Both automations are required for the system to function

3. **Configure Mobile Notifications**
   - Open both automation files in a text editor
   - Search for `notify.mobile_app_sm_s911b`
   - Replace with your own mobile notification entity (e.g., `notify.mobile_app_your_phone`)

4. **Update secrets.yaml**
   - Copy `secrets_template.yaml` to `secrets.yaml`
   - Fill in all credentials with new `entsoe_` prefix format
   - Add fallback AP credentials

5. **Place Helper Files**
   - Place `entsoe_storage_v2.h` and `entsoe_http_idf.h` directly alongside `entso-e-prices.yaml`
   - NOT in `src/` subfolder

6. **Clean Build**
   - Delete old `.esphome` build directory
   - Select ESP-IDF framework when prompted
   - Compile and upload new firmware

7. **Test Automations**
   - Verify midnight automation runs at 00:00
   - Verify next-day automation starts at 13:50
   - Check persistent notifications appear
   - Confirm mobile notifications work

### Why External Automations?
Moving scheduling to Home Assistant automations provides several advantages:

1. **Flexibility**: Modify update timing without recompiling firmware
2. **Retry Customization**: Adjust retry intervals and conditions easily
3. **Notification Control**: Customize when and how you receive alerts
4. **Better Integration**: Use HA's full automation capabilities
5. **Easier Debugging**: View automation runs in HA's logbook
6. **No Firmware Changes**: Change behavior through configuration, not code

---

## [3.5.0] - 2026-01-15
### 🛡️ Version 3.5.0: Reboot-Free Scheduled Updates & Pipeline Stability
**MAJOR STABILITY RELEASE**: This version tries to eliminate spontaneous ESP32 reboots (but does not not resolve them) during scheduled ENTSO-E API calls (midnight & 14:00), while preserving 100% backward compatibility with all existing sensors and Home Assistant automations from v3.1.1.

The core ENTSO-E parsing logic, forward-fill algorithm, and all sensor IDs remain unchanged. Only the scheduling, execution model, and safety guards were redesigned to prevent overlapping updates and watchdog issues.

### Added
* **Safe Scheduling Flags for Today & Tomorrow Pipelines**
  + Introduced `need_today_update` / `need_tomorrow_update` globals to **schedule** work instead of executing API calls directly inside `on_time` triggers.
  + Added `is_updating_today` / `is_updating_tomorrow` globals as **mutual exclusion guards** to prevent overlapping updates.
  + Ensures only one instance of the today/tomorrow update script can run at a time, even when multiple triggers fire close together (e.g. boot recovery + retry + primary schedule).

* **Worker Tick Execution Model**
  + New worker ticks run every 10 seconds for each pipeline:
    - Today worker: checks `need_today_update` and starts `smart_price_update` if not already updating.
    - Tomorrow worker: checks `need_tomorrow_update` and starts `smart_next_day_price_update` if within valid time window and not already updating.
  + Moves the heavy HTTP + XML parsing out of `on_time` callback bodies and into a **controlled, non-overlapping execution path**.
  + Reduces risk of watchdog resets caused by long-running operations inside ESPHome time callbacks.

* **Manual Update Integration with Safe Scheduling**
  + Both manual buttons now **schedule** updates via flags instead of calling scripts directly:
    - "Entso-E Force Update" sets `need_today_update = true`.
    - "Entso-E Force Next Day Update" sets `need_tomorrow_update = true` (time-gated after 14:00).
  + This ensures manual, scheduled, and boot-recovery updates all share the **same safe execution pipeline** and guards.

* **Improved Logging for Scheduling & Workers**
  + Added clear log messages:
    - `"Midnight - Scheduling Today's Prices"`
    - `"Scheduling retry for Today's Price Update"`
    - `"Boot Recovery - Scheduling Today's Prices Update"`
    - `"14:00 - Scheduling Tomorrow's Prices"`
    - `"Scheduling Retry for Tomorrow's Price Update"`
    - `"Boot Recovery - Scheduling Tomorrow's Prices"`
    - `"Worker starting Today's Price Update"`
    - `"Worker starting Tomorrow's Price Update"`
  + Greatly simplifies troubleshooting by showing exactly **when** updates are scheduled vs. actually started.

### Changed
* **on_time Triggers Now Only Schedule Work (No Heavy Logic Inside)**
  + All `on_time` entries were refactored to **set flags** instead of directly executing:
    - `script.execute: smart_price_update`
    - `script.execute: smart_next_day_price_update`
  + Heavy operations (HTTP requests + XML parsing) now run exclusively via worker ticks, protected by `is_updating_*` guards.
  + Eliminates long blocking operations inside time callbacks, reducing the risk of watchdog-triggered reboots.

* **Boot Recovery Logic Converted to Scheduling**
  + Boot recovery for today's data:
    - Previously: directly executed `smart_price_update` at `00:00:45` on boot.
    - Now: sets `need_today_update = true` once per boot, and `boot_recovery_executed = true` to avoid repeats.
  + Boot recovery for tomorrow's data:
    - Previously: directly called `smart_next_day_price_update` if within 14:00–23:00 window.
    - Now: sets `need_tomorrow_update = true` once per boot (within valid window), and flips `next_day_boot_recovery_executed` to prevent repeated attempts.
  + Guarantees that boot recovery uses the same safe worker-based update path as all other triggers.

* **Today & Tomorrow Scripts Wrapped with Re-Entrancy Guards**
  + `smart_price_update`:
    - Now begins with a guard:
      ```cpp
      if (id(is_updating_today)) {
        ESP_LOGW("entsoe", "smart_price_update skipped: already updating today");
        return;
      }
      id(is_updating_today) = true;
      ```
    - Ends by resetting: `id(is_updating_today) = false;`
  + `smart_next_day_price_update`:
    - Similar guard:
      ```cpp
      if (id(is_updating_tomorrow)) {
        ESP_LOGW("entsoe", "smart_next_day_price_update skipped: already updating tomorrow");
        return;
      }
      id(is_updating_tomorrow) = true;
      ```
    - Ends by resetting: `id(is_updating_tomorrow) = false;`
  + Prevents accidental double invocation when multiple scheduling events fire close together.

* **Unified Timeout Strategy for All HTTP Requests**
  + Today's script (`full_update_logic_script`) now also uses:
    ```cpp
    http.setTimeout(20000);  // 20 second timeout to avoid long blocks
    ```
  + Aligns with the timeout already used in the tomorrow script from v3.1.1.
  + Ensures that **both** today and tomorrow API calls fail fast on slow responses instead of triggering watchdog resets.

* **Robust Handling for "No Parsed Data" Scenario**
  + After parsing XML, both pipelines now explicitly handle the case where `count == 0`:
    - Set `last_update_success` / `next_day_last_update_success` to `false`.
    - Set status messages to `"No data points parsed"`.
    - Log a warning to help diagnose malformed or empty API responses.
  + Prevents silent failure and makes it clear in Home Assistant that no valid data was received.

### Fixed
* **Spontaneous Reboots on Scheduled API Calls**
  + Eliminated watchdog-triggered reboots observed during:
    - Midnight daily fetch for today's prices.
    - 14:00 next-day fetch for tomorrow's prices.
    - Retry cycles when ENTSO-E was slow or temporarily unavailable.
  + Root causes addressed:
    - Long blocking HTTP + XML parsing inside `on_time` callbacks.
    - Potential overlapping execution of update scripts from multiple triggers (primary + retry + boot recovery).
  + With v3.5.0, scheduled updates for both pipelines can perform full 96-point updates **back-to-back** without causing a reboot (turns out this was not addressed properly in v3.5.0).

* **Overlapping Update Calls from Multiple Triggers**
  + Previously, it was possible for:
    - Midnight main trigger and retry to overlap.
    - 14:00 main trigger, retry, and boot recovery to interleave and double-call `smart_next_day_price_update`.
  + The new `need_*` and `is_updating_*` design ensures:
    - At most one update per pipeline is running at any time.
    - Additional triggers simply set the `need_*` flag, which the worker picks up once the previous update completes.

* **Manual Updates Bypassing Safety Logic (Now Unified)**
  + Prior behavior: manual buttons called scripts directly, sometimes taking a different code path from scheduled updates.
  + New behavior: manual buttons share the exact same scheduling and guards:
    - Manual today button → `need_today_update = true`.
    - Manual tomorrow button → `need_tomorrow_update = true` (14:00–23:00 gate enforced).
  + Guarantees consistent, safe behavior regardless of how updates are triggered.

### Technical Improvements
* **Execution Model Refactor for ESPHome Time Component**
  + Decoupled scheduling from execution:
    - `on_time` now only toggles flags and logs events.
    - Worker ticks perform the actual updates under controlled conditions.
  + Aligns with ESPHome best practices of keeping time callbacks lightweight.

* **Improved Diagnostic Visibility**
  + New, explicit log messages around scheduling, worker execution, and script guarding.
  + Easier to see:
    - When the device planned an update.
    - When it actually started the HTTP call.
    - Whether it skipped updates due to an existing in-progress operation.

* **Preserved Forward-Fill & Data Integrity Logic**
  + Forward-fill algorithm from v2.3.5 and v3.1.1 remains **unchanged**:
    - ENTSO-E XML gaps between `<position>` values are still filled with the last known price.
    - Trailing gaps up to position 96 are also filled.
  + All NAN initialization and `std::isnan()` safeguards preserved for accurate min/max/avg calculations.

### Compatibility
* **ESPHome**: Requires 2025.12.0+ (same as v3.1.1)
* **Home Assistant**: No changes required, all existing integrations and entities remain intact
* **API**: ENTSO-E API (no changes required)
* **Hardware**: All ESP32 boards supported; memory usage remains well within typical limits
* **Existing Automations**:
  + Fully compatible; all existing entity IDs and behaviors for today's and tomorrow's sensors are preserved.
  + The only visible behavior change should be **increased stability** and absence of reboot-induced outages around midnight and 14:00.
* **New Globals**:
  + `need_today_update`, `is_updating_today`, `need_tomorrow_update`, `is_updating_tomorrow` are internal control flags and do **not** expose new HA entities.

### Migration Notes
* **Drop-in replacement for v3.1.1**
  + Simply replace your existing v3.1.1 YAML with the v3.5.0 version.
  + Retain your existing secrets and customization (WiFi, API token, area code, timezone, fees, VAT).
* **No sensor or entity changes**
  + All sensors documented in README for v3.1.1 are still present and behave the same.
  + No renaming, removal, or type changes were made to any exposed entities.
* **Improved Stability at Critical Times**
  + Midnight and 14:00 updates now complete without random reboots.
  + Today's sensors remain continuously available even when tomorrow's 14:00 update is running.
* **Recommended Upgrade**
  + Strongly recommended for anyone running v3.x who has observed:
    - Reboots around midnight or 14:00.
    - Temporary "all sensors unavailable" states during scheduled updates.

---

## [3.1.1] - 2025-12-30
### 🎉 Version 3.1.1: Next-Day Forecasting & Stability
**MAJOR RELEASE**: This version introduces next-day price forecasting, complete data pipeline isolation, and significant stability improvements. Enables comparison of today's and tomorrow's prices for informed energy decisions.

### Added
* **Next-Day Price Forecasting**
  + Fetches electricity prices for the next day via ENTSO-E API
  + Automatic daily fetch at 14:00 when day-ahead market data is published
  + Enables informed decisions about running appliances today vs tomorrow
  + Prices become available after ~13:15 CET/CEST when ENTSO-E publishes data
  + Data automatically cleared at 23:59:55 to prevent stale data display

* **Next-Day Price Sensors**
  + `json_next_day_hourly_prices_kwh` - JSON array of 24 hourly prices for next day
  + `next_day_hourly_avg_price_kwh` - Average price for next day
  + `next_day_min_price_kwh` - Minimum hourly price for next day
  + `next_day_max_price_kwh` - Maximum hourly price for next day
  + `next_day_price_update_status` - Binary SUCCESS/FAILED indicator
  + `next_day_price_update_attempts` - Daily attempt counter
  + `next_day_price_update_status_message` - Human-readable status

* **Next-Day Force Update Button**
  + "Entso-E Force Next Day Update" button for manual next-day fetches
  + Time-gated to only execute between 14:00-23:50
  + Prevents failed API attempts when next-day data is not yet available
  + Logs message and takes no action if pressed outside valid window

* **Boot Recovery for Next-Day Data**
  + Automatic next-day fetch on boot if within valid window (14:00-23:00)
  + Prevents permanently missing next-day data after device restarts
  + One-time execution flag ensures single recovery attempt per boot
  + Avoids unnecessary API attempts if boot occurs outside valid window

* **Data Clearing Automation**
  + Next-day data cleared at 23:59:55 via dedicated scheduled trigger
  + Complete isolation from today's data clearing at midnight
  + Prevents any interference between two data pipelines
  + All next-day globals and sensors reset during clearing

### Changed
* **Data Pipeline Isolation**
  + Completely separated today's and next-day data pipelines
  + Today's data remains stable during next-day update operations
  + No more "all sensors unavailable" during 14:00 API fetch
  + Separate global variables for each pipeline
  + Separate sensor update schedules (:02 seconds for today, :03 for next-day)
  + Independent update scripts and triggers for each pipeline

* **Sensor Update Schedule Staggering**
  + Today's sensors update at :02 seconds past each 15-minute interval
  + Next-day sensors update at :03 seconds past each 15-minute interval
  + One-second separation prevents any timing conflicts
  + Ensures sequential processing even with simultaneous triggers

* **Midnight Trigger Simplification**
  + 00:00:30 trigger now handles only today's data fetching
  + Next-day data clearing moved to separate 23:59:55 trigger
  + Eliminates interference between midnight operations
  + Today's data fetch has dedicated, uncluttered execution window

* **Updated Status Message Behaviors**
  + Next-day status message shows "Waiting for 14:00..." before 14:00
  + Next-day status message shows "Update successful" after successful fetch
  + Status messages clearly indicate pipeline state for debugging

### Fixed
* **Watchdog Timer Reboots During API Calls**
  + Added 20-second HTTP timeout to all API requests
  + Prevents ESP32 watchdog from triggering during slow API responses
  + Significantly improves device stability during 14:00 updates
  + Fixes "all sensors unavailable" issue caused by device reboots

* **Midnight Data Clearing Interference**
  + Moved next-day data clearing from 00:00:05 to 23:59:55
  + Prevents interference with today's data fetch at midnight
  + Each pipeline now has dedicated, non-overlapping schedules
  + Fixes issue where today's data would be cleared unexpectedly

* **Force Update Button Outside Valid Window**
  + Button now checks current hour before executing
  + No state change if button pressed before 14:00
  + Status message remains "Waiting for 14:00..." when button disabled
  + Prevents confusing "Updating..." states during invalid times

* **URL Construction in Next-Day Script**
  + Fixed missing URL construction line in next-day update script
  + Enables successful API calls for next-day data fetching
  + Resolved C++ compilation error "'url' was not declared in this scope"

* **YAML Syntax Errors in Boot Recovery Logic**
  + Fixed complex YAML indentation in boot recovery trigger
  + Lambda now correctly placed as sibling to if block
  + Enables proper conditional execution of boot recovery

### Technical Improvements
* **HTTP Timeout Protection**
  + http.setTimeout(20000) prevents indefinite hangs on slow API responses
  + ESP32 watchdog timer no longer triggers during normal operations
  + Improved reliability of all API fetch operations
  + Cleaner failure handling when API is slow or unreachable

* **Complete Pipeline Isolation Architecture**
  + Separate globals, scripts, sensors, and schedules for each pipeline
  + No shared state between today's and next-day operations
  + Today's data completely unaffected by next-day failures
  + Enables independent debugging and troubleshooting

* **Staggered Update Mechanism**
  + One-second separation between pipeline updates
  + Prevents concurrent execution of data processing
  + More predictable resource usage
  + Better error isolation between pipelines

* **Robust Boot Recovery**
  + Conditional execution based on current time
  + Single execution per boot prevents retry loops
  + Graceful handling of all boot scenarios
  + Automatic catch-up fetching when device restarts during valid window

### Compatibility
* **ESPHome**: Requires 2025.12.0+ (unchanged from v2.3.5)
* **Home Assistant**: No changes required, all existing integrations work
* **API**: ENTSO-E API (no changes required)
* **Hardware**: All ESP32 boards (no hardware changes)
* **Existing Automations**: Continue to work without modification
* **Secrets**: All existing credentials and settings remain compatible
* **New Sensors**: 7 new next-day sensors available for new automations

### Migration Notes
* **No breaking changes**: All existing settings and credentials remain compatible
* **Drop-in replacement**: Simply replace v2.3.5 YAML with v3.1.1 version
* **New sensors available**: Next-day sensors for forecasting automations
* **Improved stability**: Watchdog protection prevents device reboots
* **Better data isolation**: Today's data unaffected by next-day operations
* **New automation possibilities**: Compare today vs tomorrow prices
* **No configuration changes**: All existing Home Assistant automations continue to work

---

## Version History
| Version | Date | Changes |
| --- | --- | --- |
| 4.2.0 | 2026-02-01 | ESP-IDF framework migration, NVS persistent storage, external HA automations (REQUIRED), helper files |
| 3.5.0 | 2026-01-15 | Reboot-free scheduled updates (failed fix attempt), safe scheduling flags, worker-based execution, guarded scripts |
| 3.1.1 | 2025-12-30 | Next-day forecasting, data pipeline isolation, watchdog protection, boot recovery, 7 new sensors |
| 2.3.5 | 2025-12-23 | Forward-Fill parsing, double precision math, race condition fix, new diagnostic sensors |
| 2.2.1 | 2025-12-21 | CRITICAL: Midnight automation, retry deadlock & vector comparison fixes |
| 2.1.0 | 2025-12-20 | FIXED: Midnight update automation defect, enhanced retry timing |
| 2.0.0 | 2025-12-18 | Smart retry logic, status monitoring, bidirectional communication |
| 1.0.0 | 2025-12-15 | Initial release with full feature set |

---

## Support
For issues, feature requests, or questions:
* Check the troubleshooting section in README.md
* Review ESP Home logs for error messages
* Visit the project repository for community support
* Submit issues on GitHub for bug reports
* Check ESPHome version compatibility (requires 2025.12.0+ for v2.0.0+)
* **IMPORTANT**: v4.2.0 requires external HA automations from v4.2.0/crucial_ha_automations/ - prices will not update without them
* **IMPORTANT**: v4.2.0 uses ESP-IDF framework (not Arduino)
* **IMPORTANT**: v3.5.0 removes spontaneous reboots during scheduled updates via safe scheduling & worker execution
* **IMPORTANT**: v3.1.1 adds next-day forecasting and stability improvements

## Contributing
Contributions are welcome! Please read the contributing guidelines and submit pull requests for:
* Bug fixes
* Feature enhancements
* Documentation improvements
* Translation updates
* Country code additions
* Forward-Fill algorithm improvements
* Precision calculation enhancements
* Smart retry logic improvements
* Status monitoring enhancements
* Automation examples for Home Assistant
* ESP-IDF framework optimizations
* NVS storage improvements
