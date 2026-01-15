# Changelog
All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [3.5.0] - 2026-01-15
### 🛡️ Version 3.5.0: Reboot-Free Scheduled Updates & Pipeline Stability
**MAJOR STABILITY RELEASE**: This version eliminates spontaneous ESP32 reboots during scheduled ENTSO-E API calls (midnight & 14:00), while preserving 100% backward compatibility with all existing sensors and Home Assistant automations from v3.1.1.

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
  + Today’s script (`full_update_logic_script`) now also uses:
    ```cpp
    http.setTimeout(20000);  // 20 second timeout to avoid long blocks
    ```
  + Aligns with the timeout already used in the tomorrow script from v3.1.1.
  + Ensures that **both** today and tomorrow API calls fail fast on slow responses instead of triggering watchdog resets.
* **Robust Handling for “No Parsed Data” Scenario**
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
  + With v3.5.0, scheduled updates for both pipelines can perform full 96-point updates **back-to-back** without causing a reboot.
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
  + Fully compatible; all existing entity IDs and behaviors for today’s and tomorrow’s sensors are preserved.
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
  + Today’s sensors remain continuously available even when tomorrow’s 14:00 update is running.
* **Recommended Upgrade**
  + Strongly recommended for anyone running v3.x who has observed:
    - Reboots around midnight or 14:00.
    - Temporary “all sensors unavailable” states during scheduled updates.

## [3.1.1] - 2025-12-30
### 🎉 Version 3.1.1: Next-Day Forecasting & Stability
**MAJOR RELEASE**: This version introduces next-day price forecasting, complete data pipeline isolation, and significant stability improvements. Enables comparison of today's and tomorrow's prices for[...]

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

## [2.3.5] - 2025-12-23
### 🆕 Version 2.3.5: Precision & Reliability Improvements
**RECOMMENDED UPDATE**: This version introduces critical precision improvements and reliability enhancements for data integrity.

### Added
* **Forward-Fill XML Parsing Algorithm**
  + Implements robust algorithm to handle ENTSO-E data compression
  + Automatically fills missing sequential identical data points
  + Handles data gaps where ENTSO-E compresses consecutive identical prices
  + Ensures complete 96-point daily dataset even with partial API responses
  + Maintains data accuracy when TSO publishes compressed market data
* **Daily Price Update Attempts Sensor**
  + New integer sensor tracking total update attempts per day
  + Automatically resets at midnight for daily diagnostic tracking
  + Replaces legacy "Retry Count" with more accurate daily metrics
  + Provides better troubleshooting visibility for daily operations
  + Example: sensor.daily_price_update_attempts shows attempt count for current day
* **Current Price Status Sensor**
  + New text sensor indicating data validity for current time slot
  + Displays "Valid" when price data exists for current 15-minute period
  + Displays "Missing" when price data is unavailable (NAN)
  + Enables immediate detection of data integrity issues
  + Example: sensor.current_price_status shows "Valid" or "Missing"

### Changed
* **Double Precision Math Implementation**
  + Migrated all price calculation constants from float to double
  + Changes: PROV_FEE, VAT_R, and MULT constants now use double type
  + Eliminates rounding errors in daily Average Price calculations
  + Improves accuracy of financial calculations for long-term tracking
  + Affects: raw_sum_mwh accumulator, final average calculation
  + Fixes reported cases of Average Price showing incorrect values
* **Data Integrity with NAN Initialization**
  + Price vectors now initialized with NAN (Not a Number) instead of zeros
  + Missing data points now report as "unavailable" in Home Assistant
  + Prevents false "0.0" readings that could trigger incorrect automations
  + Forward-Fill algorithm populates gaps, NAN only remains for truly missing data
  + Improves reliability of automation triggers based on price data
* **Sensor Update Order Optimization**
  + Fixed race condition causing stat sensors to show "00:00" or "Unknown"
  + Time sensors now update in correct sequence after price calculations
  + Ensures time-of-highest/lowest-price display correct values immediately
  + Resolves visual glitches in Home Assistant dashboard for time sensors
  + Update sequence: statistics first, then time sensors, then all other sensors
* **Retry Count to Daily Price Update Attempts Migration**
  + Legacy "Price Update Retry Count" replaced with "Daily Price Update Attempts"
  + New sensor provides better diagnostic context with midnight reset
  + Maintains backward compatibility for automations during transition
  + Existing automations using retry_count will continue to work
  + New automations should use daily_price_update_attempts for daily metrics

### Fixed
* **Forward-Fill Gap Handling**
  + Fixed XML parsing to detect and fill missing data positions
  + Algorithm detects gaps between sequential price positions
  + Fills missing slots with last known valid price value
  + Handles both leading gaps, middle gaps, and trailing gaps
  + Ensures complete 96-point dataset for accurate price tracking
* **Daily Average Price Rounding Errors**
  + Double precision math eliminates floating-point accumulation errors
  + raw_sum_mwh now uses double instead of float accumulator
  + Final average calculation uses double precision before casting to float
  + Fixes edge cases where average showed 0.1349 instead of 0.1350
  + Provides mathematically accurate daily average prices
* **Race Condition on Time Stat Sensors**
  + Fixed timing issue where time sensors showed "00:00" briefly after update
  + Time sensors now force-update after price calculations complete
  + Explicit publish calls ensure correct values display immediately
  + Eliminates "Unknown" state during sensor refresh cycles
  + Improves dashboard reliability and user experience
* **NAN Propagation Handling**
  + Fixed handling of NAN values in min/max calculations
  + std::isnan() checks prevent NAN from affecting statistics
  + Only valid numeric values participate in min/max determination
  + Ensures accurate daily statistics even with partial data

### Technical Improvements
* **XML Parsing Enhancement**
  + New Forward-Fill algorithm processes ENTSO-E XML response
  + Detects sequential position tags and identifies gaps
  + Automatically applies last-known-value interpolation
  + Maintains data integrity for compressed market data
  + Improves robustness against varying API response formats
* **Memory Management**
  + NAN initialization ensures predictable memory state
  + Forward-Fill operates on local vector before assignment
  + No memory leaks or dangling pointers in new algorithm
  + Efficient vector operations for 96-point daily dataset
* **Precision Tracking**
  + Double precision constants for financial calculations
  + Accurate accumulation of price sums
  + Precise final average computation
  + Eliminates floating-point rounding in long-running calculations

### Compatibility
* **ESPHome**: Still requires 2025.12.0+ (same as previous versions)
* **Home Assistant**: No changes required, all existing integrations work
* **API**: ENTSO-E API (no changes required)
* **Hardware**: All ESP32 boards (no hardware changes)
* **Existing Automations**: Continue to work without modification
* **Secrets**: All existing credentials and settings remain compatible

### Migration Notes
* **No breaking changes**: All existing settings and credentials remain compatible
* **Drop-in replacement**: Simply replace v2.2.1 YAML with v2.3.5 version
* **New sensors available**: Daily Price Update Attempts, Current Price Status
* **Improved accuracy**: Double precision math provides better average prices
* **Better data integrity**: NAN initialization prevents false readings
* **Enhanced robustness**: Forward-Fill handles all data formats
* **No configuration changes**: All existing Home Assistant automations continue to work

## [2.2.1] - 2025-12-21
### 🔥 CRITICAL FIXES: Midnight Automation & Retry Logic
**CRITICAL UPDATE**: This version fixes critical midnight automation issues and retry deadlocks.

### Fixed
* **CRITICAL: Midnight Update Trigger Bug**
  + Fixed bug where on_time primary fetch was nested inside /15 minute interval, firing at 00:00:01
  + Due to clock jitter or API latency, the hour == 0 && minute == 0 check would often fail
  + Moved primary fetch to 00:00:30 with dedicated trigger to avoid race conditions with HA clock sync
* **CRITICAL: Retry Deadlock Issue**
  + Fixed logic where backup retries only activated if retry_count > 0
  + If primary trigger failed to fire, count stayed at 0, preventing all backups from running
  + Resolved deadlock where device would never retry after primary failure
* **Vector Comparison Warnings**
  + Added (int) casts to vector .size() comparisons
  + Eliminates signed/unsigned comparison warnings during compilation
  + Improves code portability and warning-free compilation

### Added
* **30-Second Buffer**
  + Primary midnight fetch now fires at 00:00:30
  + Allows Home Assistant and ESP32 clock to fully synchronize date change before request
  + Eliminates race conditions with device initialization
* **State-Driven Retry Loop**
  + New on_time trigger runs every 5 minutes
  + Checks boolean last_update_success
  + Automatically keeps trying until success is achieved
  + Handles midnight failures, network drops, or API timeouts
* **Reset Logic**
  + Explicit reset of last_update_success flag at exactly midnight
  + Ensures retry loop knows to start working for new day's data

### Changed
* **Update Verification**
  + Verification now checks if avg_price > 0 after script runs
  + Provides immediate feedback in Price Update Status sensor
* **Boot Recovery**
  + Increased boot delay to 45 seconds
  + Ensures WiFi and Home Assistant API are stable before first request

### Compatibility
* **ESPHome**: Still requires 2025.12.0+ (same as v2.0.0)
* **Home Assistant**: No changes required, all existing integrations work
* **API**: ENTSO-E API (no changes required)
* **Hardware**: All ESP32 boards (no hardware changes)

### Migration Notes
* **No breaking changes**: All existing settings and credentials remain compatible
* **Drop-in replacement**: Simply replace v2.0.0 YAML with v2.2.1 version
* **Midnight reliability**: Complete elimination of automation failures
* **No configuration changes**: All existing Home Assistant automations continue to work

## [2.1.0] - 2025-12-20
### 🔥 CRITICAL FIX: Midnight Update Automation Defect Resolved
**RECOMMENDED UPDATE**: This version fixes a critical issue where automatic price updates after midnight would fail and get stuck.

### Fixed
* **CRITICAL: Midnight update automation defect**
  + Resolved issue where device would get stuck at "Retry Count 1.0" after midnight
  + Previous single-trigger retry mechanism was insufficient for handling transient network/API issues
  + Device would remain in failed state until manual intervention
  + Root cause: v2.0.0 had only one trigger at 00:00:01 with insufficient fallback mechanism

### Changed
* **Enhanced retry logic timing**
  + Primary trigger: 00:00:01 (immediately after midnight)
  + First backup trigger: 00:00:05 (4 seconds later, was 00:00:02)
  + Final backup trigger: 00:00:15 (10 seconds after first backup, was 00:00:05)
  + Boot recovery trigger: 00:00:35 (maintained existing timing)
  + More spaced-out retry attempts provide better resilience against temporary service disruptions

### Technical Improvements
* **Multi-trigger retry mechanism**: Replaced single-trigger approach with robust multi-attempt system
* **Conditional execution**: Each trigger includes validation to prevent unnecessary API calls
* **Improved fault tolerance**: System now handles transient network issues and temporary ENTSO-E API unavailability
* **Better user experience**: Automatic recovery from midnight update failures without manual intervention
* **Enhanced error handling**: More robust recovery from various failure scenarios

### Migration Notes
* **No breaking changes**: All existing settings and credentials remain compatible
* **Drop-in replacement**: Simply replace v2.0.0 YAML with v2.1.0 version
* **Automatic improvement**: Midnight update automation will work reliably after upgrade
* **No configuration changes**: All existing Home Assistant automations continue to work

### Compatibility
* **ESPHome**: Still requires 2025.12.0+ (same as v2.0.0)
* **Home Assistant**: No changes required, all existing integrations work
* **API**: ENTSO-E API (no changes required)
* **Hardware**: All ESP32 boards (no hardware changes)

## [2.0.0] - 2025-12-18
### 🎉 Major Update: Smart Retry Logic & Status Monitoring
**BREAKING CHANGES**: This version requires **ESPHome 2025.12.0+** for the new API action responses feature.

### Added
* **Smart Retry Logic**: Intelligent update system with up to 3 automatic retry attempts
* **Status Monitoring Sensors**: New sensors for real-time update status monitoring
  + `Price Update Status` - Shows SUCCESS/FAILED status
  + `Price Update Retry Count` - Displays current retry attempt (0-3)
  + `Last Price Update Time` - Timestamp of last update attempt
  + `Price Update Status Message` - Detailed status description
* **Data Freshness Check**: Prevents unnecessary API calls if data is less than 1 hour old
* **Bidirectional Communication**: Home Assistant can verify price update status via API actions
* **Enhanced Error Handling**: Comprehensive retry logic with automatic failure detection
* **Critical Failure Alerts**: Automatic detection when all retry attempts fail

### Enhanced Features
* **Improved Midnight Updates**: Smart retry system ensures reliable price data updates at day turnover
* **Cross-Platform Verification**: ESPHome validates data updates with Home Assistant sensors
* **Automatic Reset Logic**: Retry counters reset automatically for next day's cycle
* **Enhanced Logging**: Detailed status messages and retry attempt tracking
* **API Action Response**: New `verify_price_update` action for Home Assistant integration

### Technical Improvements
* **Memory Efficient**: Smart data freshness checking reduces unnecessary API calls
* **Network Optimization**: Automatic retry with exponential backoff timing
* **Error Recovery**: Robust handling of temporary API failures
* **Status Persistence**: Update status tracked across ESPHome restarts
* **HA Integration**: New API actions for bidirectional status verification

### New Home Assistant Automations
* **Critical Failure Notifications**: Alert when all 3 update attempts fail
* **Retry Status Monitoring**: Track update progress in real-time
* **Success Notifications**: Optional alerts for successful updates
* **Dashboard Integration**: Status sensors for monitoring interface

### Compatibility
* **ESPHome**: Requires 2025.12.0+ (for API action responses)
* **Home Assistant**: Enhanced integration with new status sensors
* **API**: ENTSO-E API (no changes required)
* **Hardware**: All ESP32 boards (no hardware changes)

### Configuration Changes
* **Time Schedule**: Changed from dual fetch (00:00 + 02:00) to smart single fetch (00:00 only)
* **Button Action**: Force update button now triggers smart retry logic
* **New Globals**: Added retry tracking and status management variables
* **API Actions**: Added bidirectional communication support

### Migration Guide
Users upgrading from v1.0.0:
1. **Upgrade ESPHome** to version 2025.12.0 or later
2. **Replace YAML file** with the new version 2.0.0 configuration
3. **Update Home Assistant automations** with new status monitoring
4. **Configure API actions** in ESPHome integration settings
5. **Test smart retry logic** using the force update button

### Known Issues (Fixed in later versions)
* **Midnight Automation**: v2.0.0 had a critical midnight update automation defect
* **Trigger Timing**: Race conditions between midnight triggers and retry logic
* All these issues have been resolved in v2.2.1 and v2.3.5

## [1.0.0] - 2025-12-15
### Added
* Initial release of ESP32 Electricity Price Ticker
* Real-time 15-minute electricity price monitoring
* Hourly average price calculations
* Support for 35+ European countries via ENTSO-E API
* Comprehensive sensor suite for Home Assistant:
  + Current and next electricity price sensors
  + Daily statistics (min/max/average prices)
  + Time tracking for price extremes
  + Percentage calculations
  + JSON export of hourly prices
* Automatic VAT and provider fee calculations
* Manual force update functionality
* Home Assistant integration with API encryption
* Timezone-aware price calculations
* Robust error handling and logging
* Complete documentation package:
  + Quick start guide for beginners
  + Detailed README with country codes
  + Comprehensive troubleshooting section
  + Automation examples for Home Assistant
  + Security considerations
* Docker support for ESP Home dashboard
* Multi-platform installation options

### Technical Features
* ESP32 Arduino framework integration
* HTTP client for ENTSO-E API communication
* XML parsing for market data extraction
* Template sensors for dynamic calculations
* Text sensors for time tracking and JSON export
* Global variables for price storage and tracking
* Scheduled automation for regular updates
* WiFi fallback access point configuration
* Home Assistant time synchronization
* Memory-efficient data structures

### Documentation
* Beginner-friendly installation guide
* Complete country codes list with area codes
* API token acquisition instructions
* Home Assistant automation examples
* Advanced configuration options
* Security best practices
* Troubleshooting guide with solutions

### Compatibility
* ESP Home 2024.1+
* Home Assistant 2023.1+
* All ESP32 development boards
* 2.4GHz WiFi networks
* European electricity markets

## [Unreleased]
### Planned Features
* Support for additional API endpoints
* Historical data logging to external databases
* Multi-zone/multi-country support
* Custom price calculation formulas
* Web dashboard for local monitoring
* Mobile app notifications
* Weather-based price predictions
* Advanced analytics and reporting
* REST API for external integrations
* MQTT support for non-Home Assistant setups

### Improvements
* Enhanced power consumption optimization
* Improved memory management
* Faster API response handling
* Enhanced security features
* Better logging and debugging tools
* Enhanced Forward-Fill algorithm for complex data patterns
* Additional diagnostic sensors for advanced monitoring

## Version History
| Version | Date | Changes |
| --- | --- | --- |
| 3.5.0 | 2026-01-15 | Reboot-free scheduled updates, safe scheduling flags, worker-based execution, guarded scripts |
| 3.1.1 | 2025-12-30 | Next-day forecasting, data pipeline isolation, watchdog protection, boot recovery, 7 new sensors |
| 2.3.5 | 2025-12-23 | Forward-Fill parsing, double precision math, race condition fix, new diagnostic sensors |
| 2.2.1 | 2025-12-21 | CRITICAL: Midnight automation, retry deadlock & vector comparison fixes |
| 2.1.0 | 2025-12-20 | FIXED: Midnight update automation defect, enhanced retry timing |
| 2.0.0 | 2025-12-18 | Smart retry logic, status monitoring, bidirectional communication |
| 1.0.0 | 2025-12-15 | Initial release with full feature set |
| Unreleased | - | Future development |

## Support
For issues, feature requests, or questions:
* Check the troubleshooting section in README.md
* Review ESP Home logs for error messages
* Visit the project repository for community support
* Submit issues on GitHub for bug reports
* Check ESPHome version compatibility (requires 2025.12.0+ for v2.0.0+)
* **IMPORTANT**: v3.5.0 removes spontaneous reboots during scheduled updates via safe scheduling & worker execution
* **IMPORTANT**: v3.1.1 adds next-day forecasting and stability improvements
* **IMPORTANT**: v2.3.5 fixes precision issues and adds Forward-Fill parsing for data integrity
* **IMPORTANT**: v2.2.1 fixes critical midnight automation and retry issues from previous versions

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
* Midnight automation improvements
* Retry deadlock resolution
* Retry timing optimizations
* Data integrity improvements
* Diagnostic sensor enhancements
* Next-day forecasting features
* Data pipeline isolation architecture
* Watchdog protection mechanisms
* Boot recovery logic
* Pipeline stability improvements
* Safe scheduling & worker-based execution for long-running tasks
* Reboot-free update mechanisms around critical schedule times
