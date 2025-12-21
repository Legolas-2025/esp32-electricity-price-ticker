# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.2.1] - 2025-12-22

### 🔥 CRITICAL FIXES: Lambda Type Conversion & Midnight Automation
**CRITICAL UPDATE**: This version completely resolves compilation/runtime issues and midnight automation failures.

### Fixed
- **CRITICAL: Lambda type conversion error**
  - Resolved Price Update Status sensor compilation error
  - Fixed ternary operator return type with explicit std::string casting
  - Before: `return id(last_update_success) ? "SUCCESS" : "FAILED/WAITING";` (implicit conversion)
  - After: `return id(last_update_success) ? std::string("SUCCESS") : std::string("FAILED/WAITING");` (explicit casting)
  
- **CRITICAL: Vector Comparison Warning**
  - Added (int) casts to vector .size() comparisons
  - Eliminates signed/unsigned comparison warnings during compilation
  - Improves code portability and warning-free compilation
  
- **CRITICAL: Midnight automation silent failure**
  - Eliminated midnight update deadlock completely
  - Fixed trigger timing race conditions
  - Proper state management during midnight transitions

### Changed
- **Enhanced midnight timing**
  - Changed from 00:00:01 to 00:00:30 for better stability
  - Added proper state reset before update execution
  - Eliminates race conditions with device initialization
- **Improved retry logic**
  - Every 5 minutes with smart success checking
  - Continuous retry until successful (no hard limit)
  - Better state coordination between triggers

### Technical Improvements
- **Lambda compilation fixes**: Explicit string type casting prevents compilation errors
- **Midnight automation reliability**: Bulletproof timing prevents silent failures
- **State management**: Proper coordination between retry_count and last_update_success
- **Enhanced logging**: Better error tracking and status reporting
- **Boot recovery**: Improved power outage recovery at 45 seconds

### Compatibility
- **ESPHome**: Still requires 2025.12.0+ (same as previous versions)
- **Home Assistant**: No changes required, all existing integrations work
- **API**: ENTSO-E API (no changes required)
- **Hardware**: All ESP32 boards (no hardware changes)

### Migration Notes
- **No breaking changes**: All existing settings and credentials remain compatible
- **Drop-in replacement**: Simply replace v2.1.0 or v2.2.0 YAML with v2.2.1 version
- **Compilation fixes**: Resolves lambda type errors that may have appeared in v2.1.0
- **Midnight reliability**: Complete elimination of automation failures
- **No configuration changes**: All existing Home Assistant automations continue to work

---

## [2.2.0] - 2025-12-21

### 🔥 CRITICAL FIX: Midnight Update & Robust Retry Logic
**RECOMMENDED UPDATE**: This version fixes critical midnight automation issues and trigger timing deadlocks.

### Fixed
- **CRITICAL: Midnight Update Trigger Bug**
  - Fixed bug where on_time primary fetch was nested inside /15 minute interval, firing at 00:00:01
  - Due to clock jitter or API latency, the hour == 0 && minute == 0 check would often fail
  - Primary midnight fetch now properly isolated and timed
  
- **CRITICAL: Retry Deadlock Issue**
  - Fixed logic where backup retries only activated if retry_count > 0
  - If primary trigger failed to fire, retry_count stayed at 0, preventing all backup retries
  - Resolved infinite deadlock where device would never retry after primary failure
  
- **Trigger timing deadlock**
  - Fixed coordination between midnight triggers and retry logic
  - Proper state management prevents infinite retry loops
  - Enhanced state reset mechanisms

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
- **Enhanced retry timing**
  - Primary trigger: 00:00:30 (improved from 00:00:01)
  - Retry interval: Every 5 minutes with success checking
  - Boot recovery: Increased to 45 seconds for WiFi/API stability
  - More robust timing prevents race conditions
  
- **Update Verification**
  - Verification now checks if avg_price > 0 after script runs
  - Provides immediate feedback in Price Update Status sensor
  
- **State management improvements**
  - Better coordination between retry_count and last_update_success
  - Proper state transitions during midnight reset
  - Enhanced error recovery mechanisms

### Technical Improvements
- **Midnight automation reliability**: Bulletproof timing prevents failures
- **Enhanced error handling**: Better recovery from transient issues
- **State coordination**: Improved interaction between global variables
- **Retry logic robustness**: More reliable failure detection and recovery
- **Boot recovery enhancement**: Better power outage handling

### Compatibility
- **ESPHome**: Requires 2025.12.0+ (same as v2.0.0+)
- **Home Assistant**: No changes required
- **API**: ENTSO-E API (no changes required)
- **Migration**: Drop-in replacement from v2.0.0 and v2.1.0

### Known Issues (Fixed in v2.2.1)
- **Lambda type conversion**: v2.2.0 had potential compilation issues with status sensor lambdas
- This issue has been completely resolved in v2.2.1

---

## [2.1.0] - 2025-12-20

### 🔥 CRITICAL FIX: Midnight Update Automation Defect (Thought To Be Resolved, But Failed)
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
**KNOWN ISSUES**: This version has midnight automation and lambda issues that are fixed in v2.2.1.

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
- **Lambda Type Issues**: Potential compilation errors with status sensor lambdas
- **Trigger Timing**: Race conditions between midnight triggers and retry logic
- All these issues have been resolved in v2.2.1

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

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 2.2.1 | 2025-12-21 | CRITICAL: Lambda type conversion & vector comparison fixes |
| 2.2.0 | 2025-12-21 | CRITICAL: Midnight update trigger & retry deadlock fixes |
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
- **IMPORTANT**: v2.2.1 fixes critical lambda and midnight issues from previous versions

## Contributing

Contributions are welcome! Please read the contributing guidelines and submit pull requests for:
- Bug fixes
- Feature enhancements
- Documentation improvements
- Translation updates
- Country code additions
- Smart retry logic improvements (enhanced in v2.2.1)
- Status monitoring enhancements
- Lambda type fixes (resolved in v2.2.1)
- Midnight automation improvements (bulletproof in v2.2.1)
- Retry timing optimizations (enhanced in v2.2.1)
