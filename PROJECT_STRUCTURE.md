# ESP32 Electricity Price Ticker - Project Structure

This document provides an overview of the project files and their purposes for the v4.2.0 release.

## 📁 Project Structure

The repository is organized with **documentation files in the main folder** and **configuration files in the v4.2.0/ subfolder**. This keeps documentation easily accessible while separating release-specific configuration files.

```
esp32-electricity-price-ticker/
├── README.md                              # Main documentation (in main folder)
├── QUICKSTART.md                          # Quick start guide (in main folder)
├── CHANGELOG.md                           # Version history (in main folder)
├── PROJECT_STRUCTURE.md                   # This file (in main folder)
├── v4.2.0/                                # Current stable release configuration files
│   ├── entso-e-prices.yaml                # Main ESPHome configuration file (ESP-IDF)
│   ├── secrets_template.yaml              # Template for creating secrets.yaml
│   ├── entsoe_storage_v2.h                # NVS storage implementation for ESP-IDF
│   └── entsoe_http_idf.h                  # HTTP client optimized for ESP-IDF
├── v3.5.0/                                # Previous stable release
│   └── entsoe-hass-compatible.yaml        # Legacy configuration (Arduino framework)
├── LICENSE                                # MIT license file
├── .gitignore                             # Git ignore rules
└── Wiki/                                  # Documentation wiki
    └── v4.2.0 Wiki Home Page.md           # Wiki home page for v4.2.0
```

## 📁 v4.2.0 Configuration Subfolder

The v4.2.0 release configuration files are located directly in the `v4.2.0/` subfolder:

```
v4.2.0/
├── entso-e-prices.yaml                    # Main ESPHome configuration file
├── secrets_template.yaml                  # Template for creating secrets.yaml
├── entsoe_storage_v2.h                    # NVS storage implementation
└── entsoe_http_idf.h                      # HTTP client implementation
```

**Note:** Documentation files (README.md, QUICKSTART.md, CHANGELOG.md, PROJECT_STRUCTURE.md) remain in the main folder for easy access. Only configuration-specific files are in the v4.2.0/ subfolder.

## 📄 File Descriptions

### Documentation Files (Main Folder)

#### `README.md`
- **Purpose**: Comprehensive project documentation
- **Location**: Main folder
- **Contains**:
  - Complete feature overview
  - Detailed installation instructions
  - Country codes list (35+ countries)
  - Sensor descriptions
  - Home Assistant automation examples
  - Advanced configuration options
  - Troubleshooting guide
  - Security considerations
  - ESP-IDF framework setup instructions

#### `QUICKSTART.md`
- **Purpose**: Fast setup guide for beginners
- **Location**: Main folder
- **Contains**:
  - 5-minute setup process
  - Hardware requirements
  - API token acquisition steps
  - ESP-IDF framework installation
  - C++ helper file setup
  - Basic automation examples
  - Quick troubleshooting

#### `CHANGELOG.md`
- **Purpose**: Version history and feature tracking
- **Location**: Main folder
- **Contains**:
  - Version releases with dates
  - Added features and improvements
  - Migration notes from v3.5.0 to v4.2.0
  - Reboot issue resolution details
  - Contribution guidelines

#### `PROJECT_STRUCTURE.md`
- **Purpose**: This file - documents the project structure
- **Location**: Main folder
- **Contains**:
  - Complete repository structure
  - File descriptions and purposes
  - Usage workflows
  - Integration points
  - Support resources

### Core Configuration Files (v4.2.0/ Subfolder)

#### `v4.2.0/entso-e-prices.yaml`
- **Purpose**: Main ESPHome configuration file using ESP-IDF framework
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - ESP32 hardware configuration with ESP-IDF framework
  - WiFi and API settings with encryption
  - Sensor definitions (15+ sensors)
  - Automation schedules
  - Price calculation logic
  - HTTP client for ENTSO-E API
  - NVS storage configuration for persistent data
  - Fallback AP configuration
- **Usage**: Download from `v4.2.0/` folder and upload to ESPHome dashboard
- **Note**: Requires ESP-IDF framework (not Arduino) for full functionality
- **Important**: No manual editing required - all credentials are in secrets.yaml

#### `v4.2.0/secrets_template.yaml`
- **Purpose**: Template for sensitive configuration values
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - WiFi credentials (SSID and password)
  - Home Assistant API encryption key
  - ENTSO-E API token
  - Country area code
  - Timezone setting
  - Fallback AP credentials
- **Usage**: Copy to `secrets.yaml` and fill with your actual values

#### `secrets.yaml` (user-created)
- **Purpose**: User-provided file containing sensitive configuration
- **Location**: Your ESPHome project directory
- **Contains**:
  - All sensitive credentials referenced by `entso-e-prices.yaml`
  - Environment-specific settings
- **Usage**: Created by copying `v4.2.0/secrets_template.yaml` and customizing values
- **Important**: Never commit this file to version control

### C++ Helper Files (v4.2.0/ Subfolder)

#### `v4.2.0/entsoe_storage_v2.h`
- **Purpose**: NVS (Non-Volatile Storage) implementation for ESP-IDF
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - Functions for storing and retrieving price data in flash memory
  - Persistent storage across power cycles
  - Data validation and integrity checks
- **Integration**: Automatically included by `entso-e-prices.yaml`
- **Note**: Specific to ESP-IDF framework, provides reliable data persistence

#### `v4.2.0/entsoe_http_idf.h`
- **Purpose**: Optimized HTTP client implementation for ESP-IDF framework
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - ENTSO-E API communication routines
  - XML parsing utilities
  - Error handling and retry logic
  - Connection management
- **Integration**: Automatically included by `entso-e-prices.yaml`
- **Note**: Replaces Arduino-based HTTP client with ESP-IDF native implementation

### Legal and Technical Files

#### `LICENSE`
- **Purpose**: MIT license for open source usage
- **Contains**: Standard MIT license text
- **Usage**: Allows free use, modification, and distribution

#### `.gitignore`
- **Purpose**: Excludes sensitive and build files from version control
- **Contains**:
  - ESPHome build artifacts
  - Secrets and credentials
  - Python cache files
  - PlatformIO files
  - Editor-specific files
  - System files

### Previous Release (v3.5.0/)

#### `v3.5.0/entsoe-hass-compatible.yaml`
- **Purpose**: Main ESPHome configuration file using Arduino framework
- **Status**: Legacy version, replaced by v4.2.0
- **Framework**: Arduino (not recommended for new installations)
- **Note**: v3.5.0 experienced random reboots that v4.2.0 with ESP-IDF definitively resolves
- **Location**: `v3.5.0/` subfolder

### Wiki Documentation

#### `Wiki/v4.2.0 Wiki Home Page.md`
- **Purpose**: Wiki home page with quick links and navigation
- **Location**: `Wiki/` folder
- **Contains**:
  - Quick links to all documentation
  - Version overview
  - Architecture details
  - Setup checklist
  - Troubleshooting guide
  - Automation examples
  - FAQ

## 🔧 Usage Workflow

### For End Users (v4.2.0)
1. **Read** `QUICKSTART.md` in main folder for immediate setup
2. **Follow** `README.md` in main folder for detailed configuration
3. **Navigate** to `v4.2.0/` folder in repository
4. **Download** `entso-e-prices.yaml` from `v4.2.0/`
5. **Download** `secrets_template.yaml` from `v4.2.0/`
6. **Copy** `secrets_template.yaml` to `secrets.yaml` and fill in all values
7. **Download** both C++ helper files from `v4.2.0/`
8. **Place** C++ helper files alongside `entso-e-prices.yaml` in your ESPHome project
9. **Select ESP-IDF framework** when compiling in ESPHome dashboard
10. **Customize** based on documentation

### For Developers
1. **Review** `CHANGELOG.md` in main folder for version history
2. **Check** `LICENSE` for usage rights
3. **Follow** `.gitignore` rules for contributions
4. **Use** project structure for organization
5. **Ensure** changes maintain ESP-IDF compatibility
6. **Test** both v4.2.0 (ESP-IDF) and legacy versions if needed

## 📊 Configuration Overview

### All Credentials in secrets.yaml

**Important:** v4.2.0 is different from v3.5.0. In v4.2.0, **ALL credentials are stored in secrets.yaml** - nothing is hardcoded in the YAML file. The main YAML file (`entso-e-prices.yaml`) uses `!secret` directives to reference all configurable values.

**YAML File Structure:**
```yaml
esphome:
  name: entsoe-prices
  friendly_name: Entso-E Prices ESP-IDF
  platform: ESP32
  framework:
    type: esp-idf

esp32:
  board: esp32dev

wifi:
  # All credentials from secrets.yaml via !secret
  ssid: !secret wifi_ssid
  password: !secret wifi_password

api:
  # Encryption key from secrets.yaml
  encryption_key: !secret entsoe_api_encryption_key

# 6 Main Sections:
# 1. Hardware & Network Configuration (ESP-IDF)
# 2. Sensor Definitions (15+ sensors)
# 3. Text Sensors (JSON, times)
# 4. Time Scheduling
# 5. Global Variables with NVS Storage
# 6. Price Calculation Logic
```

### Key Components
- **15 Price Sensors**: Real-time and statistical data
- **6 Text Sensors**: Time tracking and JSON export
- **4 Global Variables**: Price storage and configuration
- **2 Automations**: Regular updates and data fetching
- **1 Manual Control**: Force update button
- **NVS Storage**: Persistent data across reboots

## 🆕 What's New in v4.2.0

### All Credentials in secrets.yaml
- **Previous (v3.5.0)**: Some credentials were hardcoded in YAML and had to be edited manually
- **Current (v4.2.0)**: ALL credentials are in secrets.yaml using `!secret` directives
- **No YAML editing required**: Download `entso-e-prices.yaml` and it works immediately once secrets.yaml is configured

### Framework Migration (Arduino to ESP-IDF)
- **Previous**: Used Arduino framework as base
- **Current**: Uses ESP-IDF framework for improved stability
- **Benefits**: Better memory management, more reliable operation
- **Requirement**: Must select ESP-IDF in ESPHome dashboard

### NVS Storage Implementation
- **Purpose**: Persistent storage for price data across power cycles
- **Implementation**: Custom `entsoe_storage_v2.h` helper file
- **Benefits**:
  - Data survives power outages
  - Faster initialization with cached data
  - Reduced API calls during startup
- **Storage Location**: ESP32 flash memory via NVS partition

### Reboot Issue Resolution
- **Problem**: v3.5.0 experienced random reboots
- **Solution**: ESP-IDF framework provides more stable operation
- **Impact**: Significantly improved reliability
- **Note**: Although v3.5.0 claimed to fix reboots, v4.2.0 with ESP-IDF definitively resolves the underlying issues

### Enhanced HTTP Client
- **Implementation**: `entsoe_http_idf.h` optimized for ESP-IDF
- **Improvements**: Better connection handling, improved error recovery
- **Compatibility**: Native ESP-IDF networking stack

### Updated Secrets Structure
- **All secrets** now consistently use `entsoe_` prefix
- **Centralized** in secrets.yaml with clear organization
- **Fallback AP** credentials added for emergency access

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

### ESP-IDF Framework
- **Version**: ESP-IDF 5.x compatible
- **Board**: ESP32dev
- **Memory**: Optimized for ESP32's capabilities
- **Storage**: NVS partition for persistent data

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
- Use ESPHome dashboard locally
- Test with debug logging enabled
- Modify configuration for testing
- Monitor NVS storage via ESP-IDF tools

### Production
- Set logging to INFO level
- Use stable WiFi connection
- Monitor system logs regularly
- Implement backup configuration
- Verify NVS persistence after power cycles

### Multi-Device
- Same YAML configuration for all ESP32 devices
- Unique device names and API keys
- Separate area codes for different regions
- Centralized Home Assistant monitoring

## 📁 Directory Structure for Your ESPHome Project

When setting up v4.2.0, place the helper files **directly alongside** the main YAML file in your ESPHome project. ESPHome automatically includes `.cpp` and `.h` files from the same directory as your configuration file:

```
your-esphome-project/
├── entso-e-prices.yaml              # Main configuration (from v4.2.0/)
├── entsoe_storage_v2.h              # NVS storage helper (from v4.2.0/)
├── entsoe_http_idf.h                # HTTP client helper (from v4.2.0/)
└── secrets.yaml                     # Your credentials (from v4.2.0/secrets_template.yaml)
```

**Important:** Place both `.h` helper files in the **same folder** as `entso-e-prices.yaml`, NOT in a `src/` subfolder. ESPHome automatically includes all `.h` and `.cpp` files from the configuration directory.

## 📥 Downloading Files from Repository

### Documentation Files (Main Folder)
Access these files directly in the main repository folder:

- **README.md**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/README.md
- **QUICKSTART.md**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/QUICKSTART.md
- **CHANGELOG.md**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/CHANGELOG.md
- **PROJECT_STRUCTURE.md**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/PROJECT_STRUCTURE.md

### v4.2.0 Configuration Files
Navigate to the `v4.2.0/` folder to download configuration files:

- **entso-e-prices.yaml**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/v4.2.0/entso-e-prices.yaml
- **secrets_template.yaml**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/v4.2.0/secrets_template.yaml
- **entsoe_storage_v2.h**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/v4.2.0/entsoe_storage_v2.h
- **entsoe_http_idf.h**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/v4.2.0/entsoe_http_idf.h

## 🔧 Migration from v3.5.0

If you are upgrading from v3.5.0:

1. **All Credentials Now in secrets.yaml**: Unlike v3.5.0 where some values were hardcoded in YAML, v4.2.0 puts everything in secrets.yaml
2. **Update Framework**: Change from Arduino to ESP-IDF in ESPHome dashboard
3. **Add Helper Files**: Place `entsoe_storage_v2.h` and `entsoe_http_idf.h` alongside your YAML file
4. **Update Configuration**: Use `entso-e-prices.yaml` from v4.2.0 instead of `entsoe-hass-compatible.yaml`
5. **Create New secrets.yaml**: Copy `secrets_template.yaml` from v4.2.0 and fill in all values
6. **Clean Build**: Delete old `.esphome` build directory before compiling
7. **Select ESP-IDF**: Choose ESP-IDF framework when prompted by ESPHome
8. **Test Persistence**: Verify NVS storage works after power cycle
9. **Monitor Stability**: Confirm reboot issues are resolved

---

## 📞 Support Resources

- **Documentation**: Complete guides in README.md and QUICKSTART.md (main folder)
- **Wiki**: See `Wiki/v4.2.0 Wiki Home Page.md` for wiki-based documentation
- **Community**: ESPHome and Home Assistant forums
- **API Support**: ENTSO-E transparency platform
- **Hardware**: ESP32 manufacturer documentation
- **Repository**: https://github.com/Legolas-2025/esp32-electricity-price-ticker
