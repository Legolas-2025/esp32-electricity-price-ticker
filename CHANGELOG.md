# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.3.5] - 2025-12-23

### 🆕 Version 2.3.5: Precision & Reliability Improvements
**RECOMMENDED UPDATE**: This version introduces critical precision improvements and reliability enhancements for data integrity.

### Added
- **Forward-Fill XML Parsing Algorithm**
  - Implements robust algorithm to handle ENTSO-E data compression
  - Automatically fills missing sequential identical data points
  - Handles data gaps where ENTSO-E compresses consecutive identical prices
  - Ensures complete 96-point daily dataset even with partial API responses
  - Maintains data accuracy when TSO publishes compressed market data

- **Daily Price Update Attempts Sensor**
  - New integer sensor tracking total update attempts per day
  - Automatically resets at midnight for daily diagnostic tracking
  - Replaces legacy "Retry Count" with more accurate daily metrics
  - Provides better troubleshooting visibility for daily operations
  - Example: sensor.daily_price_update_attempts shows attempt count for current day

- **Current Price Status Sensor**
  - New text sensor indicating data validity for current time slot
  - Displays "Valid" when price data exists for current 15-minute period
  - Displays "Missing" when price data is unavailable (NAN)
  - Enables immediate detection of data integrity issues
  - Example: sensor.current_price_status shows "Valid" or "Missing"

### Changed
- **Double Precision Math Implementation**
  - Migrated all price calculation constants from float to double
  - Changes: PROV_FEE, VAT_R, and MULT constants now use double type
  - Eliminates rounding errors in daily Average Price calculations
  - Improves accuracy of financial calculations for long-term tracking
  - Affects: raw_sum_mwh accumulator, final average calculation
  - Fixes reported cases of Average Price showing incorrect values

- **Data Integrity with NAN Initialization**
  - Price vectors now initialized with NAN (Not a Number) instead of zeros
  - Missing data points now report as "unavailable" in Home Assistant
  - Prevents false "0.0" readings that could trigger incorrect automations
  - Forward-Fill algorithm populates gaps, NAN only remains for truly missing data
  - Improves reliability of automation triggers based on price data

- **Sensor Update Order Optimization**
  - Fixed race condition causing stat sensors to show "00:00" or "Unknown"
  - Time sensors now update in correct sequence after price calculations
  - Ensures time-of-highest/lowest-price display correct values immediately
  - Resolves visual glitches in Home Assistant dashboard for time sensors
  - Update sequence: statistics first, then time sensors, then all other sensors

- **Retry Count to Daily Price Update Attempts Migration**
  - Legacy "Price Update Retry Count" replaced with "Daily Price Update Attempts"
  - New sensor provides better diagnostic context with midnight reset
  - Maintains backward compatibility for automations during transition
  - Existing automations using retry_count will continue to work
  - New automations should use daily_price_update_attempts for daily metrics

### Fixed
- **Forward-Fill Gap Handling**
  - Fixed XML parsing to detect and fill missing data positions
  - Algorithm detects gaps between sequential price positions
  - Fills missing slots with last known valid price value
  - Handles both leading gaps, middle gaps, and trailing gaps
  - Ensures complete 96-point dataset for accurate price tracking

- **Daily Average Price Rounding Errors**
  - Double precision math eliminates floating-point accumulation errors
  - raw_sum_mwh now uses double instead of float accumulator
  - Final average calculation uses double precision before casting to float
  - Fixes edge cases where average showed 0.1349 instead of 0.1350
  - Provides mathematically accurate daily average prices

- **Race Condition on Time Stat Sensors**
  - Fixed timing issue where time sensors showed "00:00" briefly after update
  - Time sensors now force-update after price calculations complete
  - Explicit publish calls ensure correct values display immediately
  - Eliminates "Unknown" state during sensor refresh cycles
  - Improves dashboard reliability and user experience

- **NAN Propagation Handling**
  - Fixed handling of NAN values in min/max calculations
  - std::isnan() checks prevent NAN from affecting statistics
  - Only valid numeric values participate in min/max determination
  - Ensures accurate daily statistics even with partial data

### Technical Improvements
- **XML Parsing Enhancement**
  - New Forward-Fill algorithm processes ENTSO-E XML response
  - Detects sequential position tags and identifies gaps
  - Automatically applies last-known-value interpolation
  - Maintains data integrity for compressed market data
  - Improves robustness against varying API response formats

- **Memory Management**
  - NAN initialization ensures predictable memory state
  - Forward-Fill operates on local vector before assignment
  - No memory leaks or dangling pointers in new algorithm
  - Efficient vector operations for 96-point daily dataset

- **Precision Tracking**
  - Double precision constants for financial calculations
  - Accurate accumulation of price sums
  - Precise final average computation
  - Eliminates floating-point rounding in long-running calculations

### Compatibility
- **ESPHome**: Still requires 2025.12.0+ (same as previous versions)
- **Home Assistant**: No changes required, all existing integrations work
- **API**: ENTSO-E API (no changes required)
- **Hardware**: All ESP32 boards (no hardware changes)
- **Existing Automations**: Continue to work without modification
- **Secrets**: All existing credentials and settings remain compatible

### Migration Notes
- **No breaking changes**: All existing settings and credentials remain compatible
- **Drop-in replacement**: Simply replace v2.2.1 YAML with v2.3.5 version
- **New sensors available**: Daily Price Update Attempts, Current Price Status
- **Improved accuracy**: Double precision math provides better average prices
- **Better data integrity**: NAN initialization prevents false readings
- **Enhanced robustness**: Forward-Fill handles all data formats
- **No configuration changes**: All existing Home Assistant automations continue to work

---

## [2.2.1] - 2025-12-21

### 🔥 CRITICAL FIXES: Midnight Automation & Retry Logic
**CRITICAL UPDATE**: This version fixes critical midnight automation issues and retry deadlocks.

### Fixed
- **CRITICAL: Midnight Update Trigger Bug**
  - Fixed bug where on_time primary fetch was nested inside /15 minute interval, firing at 00:00:01
  - Due to clock jitter or API latency, the hour == 0 && minute == 0 check would often fail
  - Moved primary fetch to 00:00:30 with dedicated trigger to avoid race conditions with HA clock sync
  
- **CRITICAL: Retry Deadlock Issue**
  - Fixed logic where backup retries only activated if retry_count > 0
  - If primary trigger failed to fire, count stayed at 0, preventing all backups from running
  - Resolved deadlock where device would never retry after primary failure
  
- **Vector Comparison Warnings**
  - Added (int) casts to vector .size() comparisons
  - Eliminates signed/unsigned comparison warnings during compilation
  - Improves code portability and warning-free compilation

### Added
- **30-Second Buffer**
  - Primary midnight fetch now fires at 00:00:30
  - Allows Home Assistant and ESP32 clock to fully synchronize date change before request
  - Eliminates race conditions with device initialization
  
- **State-Driven Retry Loop**
  - New on_time trigger runs every 5 minutes
  - Checks boolean last_update_success
  - Automatically keeps trying until success is achieved
  - Handles midnight failures, network drops, or API timeouts
  
- **Reset Logic**
  - Explicit reset of last_update_success flag at exactly midnight
  - Ensures retry loop knows to start working for new day's data

### Changed
- **Update Verification**
  - Verification now checks if avg_price > 0 after script runs
  - Provides immediate feedback in Price Update Status sensor
  
- **Boot Recovery**
  - Increased boot delay to 45 seconds
  - Ensures WiFi and Home Assistant API are stable before first request

### Compatibility
- **ESPHome**: Still requires 2025.12.0+ (same as previous versions)
- **Home Assistant**: No changes required, all existing integrations work
- **API**: ENTSO-E API (no changes required)
- **Hardware**: All ESP32 boards (no hardware changes)

### Migration Notes
- **No breaking changes**: All existing settings and credentials remain compatible
- **Drop-in replacement**: Simply replace v2.1.0 YAML with v2.2.1 version
- **Midnight reliability**: Complete elimination of automation failures
- **No configuration changes**: All existing Home Assistant automations continue to work

---

## [2.1.0] - 2025-12-20

### 🔥 CRITICAL FIX: Midnight Update Automation Defect Resolved
**RECOMMENDED UPDATE**: This version fixes a critical issue where automatic price updates after midnight would fail and get stuck.

### Fixed
- **CRITICAL: Midnight update automation defect**
  - Resolved issue where device would get stuck at "Retry Count 1.0" after midnight
  - Previous single-trigger retry mechanism was insufficient for handling transient network/API issues
  - Device would remain in failed state until manual intervention
  - Root cause: v2.0.0 had only one trigger at 00:00:01 with insufficient fallback mechanism

### Changed
- **Enhanced retry logic timing**
  - Primary trigger: 00:00:01 (immediately after midnight)
  - First backup trigger: 00:00:05 (4 seconds later, was 00:00:02)
  - Final backup trigger: 00:00:15 (10 seconds after first backup, was 00:00:05)
  - Boot recovery trigger: 00:00:35 (maintained existing timing)
  - More spaced-out retry attempts provide better resilience against temporary service disruptions

### Technical Improvements
- **Multi-trigger retry mechanism**: Replaced single-trigger approach with robust multi-attempt system
- **Conditional execution**: Each trigger includes validation to prevent unnecessary API calls
- **Improved fault tolerance**: System now handles transient network issues and temporary ENTSO-E API unavailability
- **Better user experience**: Automatic recovery from midnight update failures without manual intervention
- **Enhanced error handling**: More robust recovery from various failure scenarios

### Migration Notes
- **No breaking changes**: All existing settings and credentials remain compatible
- **Drop-in replacement**: Simply replace v2.0.0 YAML with v2.1.0 version
- **Automatic improvement**: Midnight update automation will work reliably after upgrade
- **No configuration changes**: All existing Home Assistant automations continue to work

### Compatibility
- **ESPHome**: Still requires 2025.12.0+ (same as v2.0.0)
- **Home Assistant**: No changes required, all existing integrations work
- **API**: ENTSO-E API (no changes required)
- **Hardware**: All ESP32 boards (no hardware changes)

---

## [2.0.0] - 2025-12-18

### 🎉 Major Update: Smart Retry Logic & Status Monitoring
**BREAKING CHANGES**: This version requires **ESPHome 2025.12.0+** for the new API action responses feature.

### Added
- **Smart Retry Logic**: Intelligent update system with up to 3 automatic retry attempts
- **Status Monitoring Sensors**: New sensors for real-time update status monitoring
  - `Price Update Status` - Shows SUCCESS/FAILED status
  - `Price Update Retry Count` - Displays current retry attempt (0-3)
  - `Last Price Update Time` - Timestamp of last update attempt
  - `Price Update Status Message` - Detailed status description
- **Data Freshness Check**: Prevents unnecessary API calls if data is less than 1 hour old
- **Bidirectional Communication**: Home Assistant can verify price update status via API actions
- **Enhanced Error Handling**: Comprehensive retry logic with automatic failure detection
- **Critical Failure Alerts**: Automatic detection when all retry attempts fail

### Enhanced Features
- **Improved Midnight Updates**: Smart retry system ensures reliable price data updates at day turnover
- **Cross-Platform Verification**: ESPHome validates data updates with Home Assistant sensors
- **Automatic Reset Logic**: Retry counters reset automatically for next day's cycle
- **Enhanced Logging**: Detailed status messages and retry attempt tracking
- **API Action Response**: New `verify_price_update` action for Home Assistant integration

### Technical Improvements
- **Memory Efficient**: Smart data freshness checking reduces unnecessary API calls
- **Network Optimization**: Automatic retry with exponential backoff timing
- **Error Recovery**: Robust handling of temporary API failures
- **Status Persistence**: Update status tracked across ESPHome restarts
- **HA Integration**: New API actions for bidirectional status verification

### New Home Assistant Automations
- **Critical Failure Notifications**: Alert when all 3 update attempts fail
- **Retry Status Monitoring**: Track update progress in real-time
- **Success Notifications**: Optional alerts for successful updates
- **Dashboard Integration**: Status sensors for monitoring interface

### Compatibility
- **ESPHome**: Requires 2025.12.0+ (for API action responses)
- **Home Assistant**: Enhanced integration with new status sensors
- **API**: ENTSO-E API (no changes required)
- **Hardware**: All ESP32 boards (no hardware changes)

### Configuration Changes
- **Time Schedule**: Changed from dual fetch (00:00 + 02:00) to smart single fetch (00:00 only)
- **Button Action**: Force update button now triggers smart retry logic
- **New Globals**: Added retry tracking and status management variables
- **API Actions**: Added bidirectional communication support

### Migration Guide
Users upgrading from v1.0.0:
1. **Upgrade ESPHome** to version 2025.12.0 or later
2. **Replace YAML file** with the new version 2.0.0 configuration
3. **Update Home Assistant automations** with new status monitoring
4. **Configure API actions** in ESPHome integration settings
5. **Test smart retry logic** using the force update button

### Known Issues (Fixed in later versions)
- **Midnight Automation**: v2.0.0 had a critical midnight update automation defect
- **Trigger Timing**: Race conditions between midnight triggers and retry logic
- All these issues have been resolved in v2.2.1 and v2.3.5

---

## [1.0.0] - 2025-12-15

### Added
- Initial release of ESP32 Electricity Price Ticker
- Real-time 15-minute electricity price monitoring
- Hourly average price calculations
- Support for 35+ European countries via ENTSO-E API
- Comprehensive sensor suite for Home Assistant:
  - Current and next electricity price sensors
  - Daily statistics (min/max/average prices)
  - Time tracking for price extremes
  - Percentage calculations
  - JSON export of hourly prices
- Automatic VAT and provider fee calculations
- Manual force update functionality
- Home Assistant integration with API encryption
- Timezone-aware price calculations
- Robust error handling and logging
- Complete documentation package:
  - Quick start guide for beginners
  - Detailed README with country codes
  - Comprehensive troubleshooting section
  - Automation examples for Home Assistant
  - Security considerations
- Docker support for ESP Home dashboard
- Multi-platform installation options

### Technical Features
- ESP32 Arduino framework integration
- HTTP client for ENTSO-E API communication
- XML parsing for market data extraction
- Template sensors for dynamic calculations
- Text sensors for time tracking and JSON export
- Global variables for price storage and tracking
- Scheduled automation for regular updates
- WiFi fallback access point configuration
- Home Assistant time synchronization
- Memory-efficient data structures

### Documentation
- Beginner-friendly installation guide
- Complete country codes list with area codes
- API token acquisition instructions
- Home Assistant automation examples
- Advanced configuration options
- Security best practices
- Troubleshooting guide with solutions

### Compatibility
- ESP Home 2024.1+
- Home Assistant 2023.1+
- All ESP32 development boards
- 2.4GHz WiFi networks
- European electricity markets

## [Unreleased]

### Planned Features
- [ ] Support for additional API endpoints
- [ ] Historical data logging to external databases
- [ ] Multi-zone/multi-country support
- [ ] Custom price calculation formulas
- [ ] Web dashboard for local monitoring
- [ ] Mobile app notifications
- [ ] Weather-based price predictions
- [ ] Advanced analytics and reporting
- [ ] REST API for external integrations
- [ ] MQTT support for non-Home Assistant setups

### Improvements
- [ ] Enhanced power consumption optimization
- [ ] Improved memory management
- [ ] Faster API response handling
- [ ] Enhanced security features
- [ ] Better logging and debugging tools
- [ ] Enhanced Forward-Fill algorithm for complex data patterns
- [ ] Additional diagnostic sensors for advanced monitoring

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 2.3.5 | 2025-12-23 | Forward-Fill parsing, double precision math, race condition fix, new diagnostic sensors |
| 2.2.1 | 2025-12-21 | CRITICAL: Midnight automation, retry deadlock & vector comparison fixes |
| 2.1.0 | 2025-12-20 | FIXED: Midnight update automation defect, enhanced retry timing |
| 2.0.0 | 2025-12-18 | Smart retry logic, status monitoring, bidirectional communication |
| 1.0.0 | 2025-12-15 | Initial release with full feature set |
| Unreleased | - | Future development |

## Support

For issues, feature requests, or questions:
- Check the troubleshooting section in README.md
- Review ESP Home logs for error messages
- Visit the project repository for community support
- Submit issues on GitHub for bug reports
- Check ESPHome version compatibility (requires 2025.12.0+ for v2.0.0+)
- **IMPORTANT**: v2.3.5 fixes precision issues and adds Forward-Fill parsing for data integrity
- **IMPORTANT**: v2.2.1 fixes critical midnight automation and retry issues from previous versions

## Contributing

Contributions are welcome! Please read the contributing guidelines and submit pull requests for:
- Bug fixes
- Feature enhancements
- Documentation improvements
- Translation updates
- Country code additions
- Forward-Fill algorithm improvements
- Precision calculation enhancements
- Smart retry logic improvements (enhanced in v2.2.1)
- Status monitoring enhancements
- Midnight automation improvements (bulletproof in v2.2.1)
- Retry deadlock resolution (fixed in v2.2.1)
- Retry timing optimizations (enhanced in v2.2.1)
- Data integrity improvements (enhanced in v2.3.5)
- Diagnostic sensor enhancements (added in v2.3.5)
