# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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

### Known Issues
- **ESPHome Version**: Requires ESPHome 2025.12.0+ (older versions will show configuration errors)
- **First Run**: Initial status sensors may show "No updates yet" until first successful update
- **HA Integration**: New API actions must be enabled in ESPHome integration settings

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
| 2.0.0 | 2025-12-18 | Smart retry logic, status monitoring, bidirectional communication, boot recovery |
| 1.0.0 | 2025-12-15 | Initial release with full feature set |
| Unreleased | - | Future development |

## Support

For issues, feature requests, or questions:
- Check the troubleshooting section in README.md
- Review ESP Home logs for error messages
- Visit the project repository for community support
- Submit issues on GitHub for bug reports
- Check ESPHome version compatibility (requires 2025.12.0+ for v2.0.0)

## Contributing

Contributions are welcome! Please read the contributing guidelines and submit pull requests for:
- Bug fixes
- Feature enhancements
- Documentation improvements
- Translation updates
- Country code additions
- Smart retry logic improvements
- Status monitoring enhancements
