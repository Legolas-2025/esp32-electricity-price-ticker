# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
- [ ] Enhanced error recovery mechanisms
- [ ] Better power consumption optimization
- [ ] Improved memory management
- [ ] Faster API response handling
- [ ] Enhanced security features
- [ ] Better logging and debugging tools

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | 2025-12-15 | Initial release with full feature set |
| Unreleased | - | Future development |

## Support

For issues, feature requests, or questions:
- Check the troubleshooting section in README.md
- Review ESP Home logs for error messages
- Visit the project repository for community support
- Submit issues on GitHub for bug reports

## Contributing

Contributions are welcome! Please read the contributing guidelines and submit pull requests for:
- Bug fixes
- Feature enhancements
- Documentation improvements
- Translation updates
- Country code additions