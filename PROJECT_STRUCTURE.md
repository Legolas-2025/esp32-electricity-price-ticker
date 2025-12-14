# ESP32 Electricity Price Ticker - Project Structure

This document provides an overview of the project files and their purposes.

## 📁 Project Structure

```
esp32-electricity-price-ticker/
├── README.md                          # Complete documentation and guide
├── QUICKSTART.md                      # 5-minute setup guide
├── entsoe-hass-compatible.yaml        # Main ESP Home configuration file
├── secrets_template.yaml              # Template for sensitive configuration
├── CHANGELOG.md                       # Version history and changes
├── LICENSE                            # MIT license file
├── .gitignore                         # Git ignore rules
└── PROJECT_STRUCTURE.md               # This file
```

## 📄 File Descriptions

### Core Configuration Files

#### `entsoe-hass-compatible.yaml`
- **Purpose**: Main ESP Home configuration file
- **Contains**: 
  - ESP32 hardware configuration
  - WiFi and API settings
  - Sensor definitions (15+ sensors)
  - Automation schedules
  - Price calculation logic
  - HTTP client for ENTSO-E API
- **Usage**: Upload this to ESP Home dashboard

#### `secrets_template.yaml`
- **Purpose**: Template for sensitive configuration values
- **Contains**:
  - WiFi credentials
  - Home Assistant API encryption key
  - ENTSO-E API token
  - Country area code
  - Timezone setting
- **Usage**: Copy to `secrets.yaml` and fill with your actual values

### Documentation Files

#### `README.md`
- **Purpose**: Comprehensive project documentation
- **Size**: ~650 lines
- **Contains**:
  - Complete feature overview
  - Detailed installation instructions
  - Country codes list (35+ countries)
  - Sensor descriptions
  - Home Assistant automation examples
  - Advanced configuration options
  - Troubleshooting guide
  - Security considerations

#### `QUICKSTART.md`
- **Purpose**: Fast setup guide for beginners
- **Size**: ~170 lines
- **Contains**:
  - 5-minute setup process
  - Hardware requirements
  - API token acquisition steps
  - Basic automation examples
  - Quick troubleshooting

#### `CHANGELOG.md`
- **Purpose**: Version history and feature tracking
- **Contains**:
  - Version releases with dates
  - Added features and improvements
  - Planned future features
  - Contribution guidelines

### Legal and Technical Files

#### `LICENSE`
- **Purpose**: MIT license for open source usage
- **Contains**: Standard MIT license text
- **Usage**: Allows free use, modification, and distribution

#### `.gitignore`
- **Purpose**: Excludes sensitive and build files from version control
- **Contains**:
  - ESP Home build artifacts
  - Secrets and credentials
  - Python cache files
  - PlatformIO files
  - Editor-specific files
  - System files

## 🔧 Usage Workflow

### For End Users
1. **Read** `QUICKSTART.md` for immediate setup
2. **Follow** `README.md` for detailed configuration
3. **Use** `entsoe-hass-compatible.yaml` as base configuration
4. **Copy** `secrets_template.yaml` to `secrets.yaml`
5. **Customize** based on documentation

### For Developers
1. **Review** `CHANGELOG.md` for version history
2. **Check** `LICENSE` for usage rights
3. **Follow** `.gitignore` rules for contributions
4. **Use** project structure for organization

## 📊 Configuration Overview

### YAML File Structure
```yaml
esphome:
  name: entsoe-hass-compatible
  friendly_name: Entso-E Prices
  
esp32:
  board: esp32dev
  
wifi:
  # WiFi configuration
  
api:
  # Home Assistant integration
  
# 6 Main Sections:
# 1. Hardware & Network Configuration
# 2. Sensor Definitions (12+ sensors)
# 3. Text Sensors (JSON, times)
# 4. Time Scheduling
# 5. Global Variables
# 6. Price Calculation Logic
```

### Key Components
- **12 Price Sensors**: Real-time and statistical data
- **6 Text Sensors**: Time tracking and JSON export
- **4 Global Variables**: Price storage and configuration
- **2 Automations**: Regular updates and data fetching
- **1 Manual Control**: Force update button

## 🌐 Integration Points

### ENTSO-E API
- **Endpoint**: `https://web-api.tp.entsoe.eu/api`
- **Authentication**: Security token
- **Data Format**: XML market data
- **Update Frequency**: Every 15 minutes

### Home Assistant
- **Integration**: Native ESPHome component
- **Protocol**: Home Assistant API with encryption
- **Sensors**: Automatic discovery and creation
- **Updates**: Real-time via ESPHome dashboard

## 🔒 Security Considerations

### Sensitive Data Handling
- **API Tokens**: Stored in separate `secrets.yaml` file
- **WiFi Credentials**: Excluded from version control
- **Encryption**: Home Assistant API encryption enabled
- **Local Storage**: All data processed locally on ESP32

### Best Practices
- Never commit `secrets.yaml` to version control
- Use strong WiFi passwords
- Regularly rotate API tokens
- Monitor network traffic for unusual activity
- Keep ESP32 firmware updated

## 🚀 Deployment Options

### Development
- Use ESP Home dashboard locally
- Test with debug logging enabled
- Modify configuration for testing

### Production
- Set logging to INFO level
- Use stable WiFi connection
- Monitor system logs regularly
- Implement backup configuration

### Multi-Device
- Same YAML configuration for all ESP32 devices
- Unique device names and API keys
- Separate area codes for different regions
- Centralized Home Assistant monitoring

---

## 📞 Support Resources

- **Documentation**: Complete guides in README.md and QUICKSTART.md
- **Community**: ESP Home and Home Assistant forums
- **API Support**: ENTSO-E transparency platform
- **Hardware**: ESP32 manufacturer documentation

**Happy building!** 🔧⚡