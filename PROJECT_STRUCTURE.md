# ESP32 Electricity Price Ticker - Project Structure

This document provides an overview of the project files and their purposes for the v4.2.0 release.

## Project Overview

Version v4.2.0 introduces a major architectural change: all scheduling logic has been moved to external Home Assistant automations. This provides maximum flexibility for customizing update timing, retry logic, and notifications without recompiling firmware. The main `entso-e-prices.yaml` file no longer updates prices on its own—it acts as a passive sensor platform awaiting external triggers from the required automations.

## Repository Structure

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
│   ├── entsoe_http_idf.h                  # HTTP client optimized for ESP-IDF
│   └── crucial_ha_automations/            # REQUIRED external automations for updates
│       ├── entsoe_midnight_automation.yaml    # Midnight: NVS promote/fallback/HTTP retry
│       └── entsoe_next_day_automation.yaml    # Next-day: 13:50 start + 30-min retries
├── entsoe-hass-compatible.yaml            # Legacy v3.5.0 configuration (Arduino framework)
├── LICENSE                                # MIT license file
├── .gitignore                             # Git ignore rules
└── Wiki/                                  # Documentation wiki
    └── v4.2.0 Wiki Home Page.md           # Wiki home page for v4.2.0
```

## v4.2.0 Configuration Subfolder

The v4.2.0 release configuration files are located directly in the `v4.2.0/` subfolder:

```
v4.2.0/
├── entso-e-prices.yaml                    # Main ESPHome configuration file
├── secrets_template.yaml                  # Template for creating secrets.yaml
├── entsoe_storage_v2.h                    # NVS storage implementation
├── entsoe_http_idf.h                      # HTTP client implementation
└── crucial_ha_automations/                # CRITICAL: Required automations folder
    ├── entsoe_midnight_automation.yaml    # Midnight automation (REQUIRED)
    └── entsoe_next_day_automation.yaml    # Next-day automation (REQUIRED)
```

**Note:** Documentation files (README.md, QUICKSTART.md, CHANGELOG.md, PROJECT_STRUCTURE.md) remain in the main folder for easy access. Only configuration-specific files are in the v4.2.0/ subfolder.

## File Descriptions

### Documentation Files (Main Folder)

#### README.md

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
  - External automation requirements and configuration

#### QUICKSTART.md

- **Purpose**: Fast setup guide for beginners
- **Location**: Main folder
- **Contains**:
  - 5-minute setup process
  - Hardware requirements
  - API token acquisition steps
  - ESP-IDF framework installation
  - C++ helper file setup
  - External automation import instructions
  - Basic automation examples
  - Quick troubleshooting

#### CHANGELOG.md

- **Purpose**: Version history and feature tracking
- **Location**: Main folder
- **Contains**:
  - Version releases with dates
  - Added features and improvements
  - Migration notes from v3.5.0 to v4.2.0
  - Reboot issue resolution details
  - External automation documentation
  - Contribution guidelines

#### PROJECT_STRUCTURE.md

- **Purpose**: This file - documents the project structure
- **Location**: Main folder
- **Contains**:
  - Complete repository structure
  - File descriptions and purposes
  - Usage workflows
  - Integration points
  - Support resources

### Core Configuration Files (v4.2.0/ Subfolder)

#### v4.2.0/entso-e-prices.yaml

- **Purpose**: Main ESPHome configuration file using ESP-IDF framework
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - ESP32 hardware configuration with ESP-IDF framework
  - WiFi and API settings with encryption
  - Sensor definitions (15+ sensors)
  - Automation schedules (passive - awaiting external triggers)
  - Price calculation logic
  - HTTP client for ENTSO-E API
  - NVS storage configuration for persistent data
  - Fallback AP configuration
- **Usage**: Download from `v4.2.0/` folder and upload to ESPHome dashboard
- **Note**: Requires ESP-IDF framework (not Arduino) for full functionality
- **Important**: No manual editing required - all credentials are in secrets.yaml
- **Critical**: Does NOT update on its own - requires external automations

#### v4.2.0/secrets_template.yaml

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

#### secrets.yaml (user-created)

- **Purpose**: User-provided file containing sensitive configuration
- **Location**: Your ESPHome project directory
- **Contains**:
  - All sensitive credentials referenced by `entso-e-prices.yaml`
  - Environment-specific settings
- **Usage**: Created by copying `v4.2.0/secrets_template.yaml` and customizing values
- **Important**: Never commit this file to version control

### C++ Helper Files (v4.2.0/ Subfolder)

#### v4.2.0/entsoe_storage_v2.h

- **Purpose**: NVS (Non-Volatile Storage) implementation for ESP-IDF
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - Functions for storing and retrieving price data in flash memory
  - Persistent storage across power cycles
  - Data validation and integrity checks
- **Integration**: Automatically included by `entso-e-prices.yaml`
- **Note**: Specific to ESP-IDF framework, provides reliable data persistence

#### v4.2.0/entsoe_http_idf.h

- **Purpose**: Optimized HTTP client implementation for ESP-IDF framework
- **Location**: `v4.2.0/` subfolder
- **Contains**:
  - ENTSO-E API communication routines
  - XML parsing utilities
  - Error handling and retry logic
  - Connection management
- **Integration**: Automatically included by `entso-e-prices.yaml`
- **Note**: Replaces Arduino-based HTTP client with ESP-IDF native implementation

### Essential Home Assistant Automations (v4.2.0/crucial_ha_automations/)

> **⚠️ CRITICAL: THESE AUTOMATIONS ARE REQUIRED FOR V4.2.0**
>
> The main YAML file no longer updates on its own. Without importing these automations, your prices will never update.

#### v4.2.0/crucial_ha_automations/entsoe_midnight_automation.yaml

- **Purpose**: Handles midnight transition and today's price update
- **Location**: `v4.2.0/crucial_ha_automations/` subfolder
- **Trigger**: At 00:00:00 (midnight)
- **Actions**:
  1. **NVS Promote**: Attempts to promote tomorrow's prices from NVS (tomorrow96 → today96)
  2. **NVS Fallback**: If promote fails, loads today's prices from today96 NVS storage
  3. **HTTP Fallback**: If NVS fails, performs HTTP fetch with up to 3 retries
- **Notifications**: Creates persistent notifications and mobile alerts on success/failure
- **Entities Used**:
  - `esphome.entso_e_prices_promote_and_load_today_from_nvs`
  - `esphome.entso_e_prices_load_today_from_nvs`
  - `button.entso_e_prices_entso_e_force_update`
  - `sensor.entso_e_prices_price_update_status`
  - `sensor.entso_e_prices_current_price_status`
- **Usage**: Import into Home Assistant (Settings → Automations → Import from file)
- **Configuration Required**: Replace `notify.mobile_app_sm_s911b` with your notification entity

#### v4.2.0/crucial_ha_automations/entsoe_next_day_automation.yaml

- **Purpose**: Handles fetching tomorrow's prices with intelligent retry logic
- **Location**: `v4.2.0/crucial_ha_automations/` subfolder
- **Trigger**: At 13:50:00 (13 minutes before standard 14:00 publish time)
- **Actions**:
  1. **Initial Trigger**: Presses force next day update button immediately
  2. **Retry Loop**: If update fails, retries every 30 minutes until 20:50
  3. **Early Exit**: Stops retrying as soon as update succeeds
- **Notifications**: Creates persistent notifications and mobile alerts on success/failure
- **Entities Used**:
  - `button.entso_e_prices_entso_e_force_next_day_update`
  - `sensor.entso_e_prices_next_day_price_update_status`
  - `sensor.entso_e_prices_next_day_current_price_status`
- **Usage**: Import into Home Assistant (Settings → Automations → Import from file)
- **Configuration Required**: Replace `notify.mobile_app_sm_s911b` with your notification entity

### Why External Automations

The v4.2.0 architecture moves all scheduling to Home Assistant automations for several key benefits:

1. **Flexibility**: Modify update timing without recompiling firmware. Change the 13:50 start time to whenever suits your needs.

2. **Retry Customization**: Adjust retry intervals and conditions easily through the HA automation editor. Increase or decrease the 30-minute retry interval.

3. **Notification Control**: Customize when and how you receive alerts about update failures. Add additional notification targets.

4. **Better Integration**: Use Home Assistant's full automation capabilities including conditions, delays, and variables. Combine with other HA automations.

5. **Easier Debugging**: View automation runs in HA's logbook and history. Trace execution step by step.

6. **No Firmware Changes**: Change behavior through configuration, not code. No need to recompile and flash when tweaking timing.

### Legal and Technical Files

#### LICENSE

- **Purpose**: MIT license for open source usage
- **Contains**: Standard MIT license text
- **Usage**: Allows free use, modification, and distribution

#### .gitignore

- **Purpose**: Excludes sensitive and build files from version control
- **Contains**:
  - ESPHome build artifacts
  - Secrets and credentials
  - Python cache files
  - PlatformIO files
  - Editor-specific files
  - System files

### Previous Release (v3.5.0/)

#### v3.5.0/entsoe-hass-compatible.yaml

- **Purpose**: Main ESPHome configuration file using Arduino framework
- **Status**: Legacy version, replaced by v4.2.0
- **Framework**: Arduino (not recommended for new installations)
- **Note**: v3.5.0 experienced random reboots that v4.2.0 with ESP-IDF definitively resolves
- **Location**: `v3.5.0/` subfolder

### Wiki Documentation

#### Wiki/v4.2.0 Wiki Home Page.md

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

## Usage Workflow

### For End Users (v4.2.0)

1. **Read** `QUICKSTART.md` in main folder for immediate setup
2. **Follow** `README.md` in main folder for detailed configuration
3. **Navigate** to `v4.2.0/` folder in repository
4. **Download** `entso-e-prices.yaml` from `v4.2.0/`
5. **Download** `secrets_template.yaml` from `v4.2.0/`
6. **Copy** `secrets_template.yaml` to `secrets.yaml` and fill in all values
7. **Download** both C++ helper files from `v4.2.0/`
8. **Place** C++ helper files alongside `entso-e-prices.yaml` in your ESPHome project
9. **Navigate** to `v4.2.0/crucial_ha_automations/` folder
10. **Download** `entsoe_midnight_automation.yaml`
11. **Download** `entsoe_next_day_automation.yaml`
12. **Import** both automation files into Home Assistant
13. **Configure** mobile notifications (replace `notify.mobile_app_sm_s911b`)
14. **Select ESP-IDF framework** when compiling in ESPHome dashboard
15. **Customize** based on documentation

### For Developers

1. **Review** `CHANGELOG.md` in main folder for version history
2. **Check** `LICENSE` for usage rights
3. **Follow** `.gitignore` rules for contributions
4. **Use** project structure for organization
5. **Ensure** changes maintain ESP-IDF compatibility
6. **Test** both v4.2.0 (ESP-IDF) and legacy versions if needed
7. **Document** any changes to automation requirements

## Configuration Overview

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

# Main sections:
# 1. Hardware & Network Configuration (ESP-IDF)
# 2. Sensor Definitions (15+ sensors)
# 3. Text Sensors (JSON, times)
# 4. Time Scheduling (passive - awaiting external triggers)
# 5. Global Variables with NVS Storage
# 6. Price Calculation Logic
```

### External Automation Architecture

**Critical Change:** v4.2.0 removes internal scheduling from the YAML file. All update triggering is now handled by external Home Assistant automations.

**What This Means:**

1. The main YAML file contains no `on_time` triggers for price updates
2. The YAML file acts as a passive sensor platform
3. Button entities exist but are not triggered automatically
4. External automations press the buttons at scheduled times
5. Without automations, prices will never update

**Automation Requirements:**

- **Two automation files must be imported** from `v4.2.0/crucial_ha_automations/`
- **Both automations must be enabled** in Home Assistant
- **Mobile notification entity must be configured** (not left as `notify.mobile_app_sm_s911b`)
- **Automations must run successfully** for prices to update

### Key Components

- **15 Price Sensors**: Real-time and statistical data
- **6 Text Sensors**: Time tracking and JSON export
- **4 Global Variables**: Price storage and configuration
- **2 Manual Buttons**: Force update buttons for manual triggering
- **NVS Storage**: Persistent data across reboots
- **2 External Automations**: REQUIRED for automatic updates
  - Midnight automation: Handles today's price update at midnight
  - Next-day automation: Handles tomorrow's price update starting at 13:50

## What is New in v4.2.0

### External Automations (REQUIRED)

v4.2.0 introduces a fundamental architectural change where all scheduling logic has been moved to external Home Assistant automations. This provides maximum flexibility for customizing update timing, retry logic, and notifications without recompiling firmware.

**Previous (v3.5.0):** YAML file contained internal `on_time` triggers for automatic updates
**Current (v4.2.0):** YAML file is passive, requires external automations for updates

**Benefits:**
- Modify update timing without recompiling firmware
- Customize retry logic through HA automation editor
- Full integration with HA notification system
- Easier debugging through HA logbook
- No firmware changes for behavior modifications

**Requirements:**
- Download both automation files from `v4.2.0/crucial_ha_automations/`
- Import into Home Assistant (Settings → Automations → Import from file)
- Enable both automations
- Configure mobile notification entity
- Verify automation traces after import

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

## Integration Points

### ENTSO-E API

- **Endpoint**: `https://web-api.tp.entsoe.eu/api`
- **Authentication**: Security token
- **Data Format**: XML market data
- **Update Frequency**: Triggered by external automations

### Home Assistant

- **Integration**: Native ESPHome component
- **Protocol**: Home Assistant API with encryption
- **Sensors**: Automatic discovery and creation
- **Updates**: Real-time via ESPHome dashboard
- **Automations**: External automations trigger updates

### ESP-IDF Framework

- **Version**: ESP-IDF 5.x compatible
- **Board**: ESP32dev
- **Memory**: Optimized for ESP32's capabilities
- **Storage**: NVS partition for persistent data

## Security Considerations

### Sensitive Data Handling

- **API Tokens**: Stored in separate `secrets.yaml` file
- **WiFi Credentials**: Excluded from version control
- **Encryption**: Home Assistant API encryption enabled
- **Local Storage**: All data processed locally on ESP32

### Automation Security

- **Notification Entity**: Must be configured (not hardcoded)
- **Entity IDs**: Automation uses specific ESPHome entity IDs
- **Access Control**: HA authentication required for automation management

### Best Practices

- Never commit `secrets.yaml` to version control
- Use strong WiFi passwords
- Regularly rotate API tokens
- Monitor network traffic for unusual activity
- Keep ESP32 firmware updated
- Review automation traces periodically
- Test notification delivery

## Deployment Options

### Development

- Use ESPHome dashboard locally
- Test with debug logging enabled
- Modify configuration for testing
- Monitor NVS storage via ESP-IDF tools
- Test automations with manual trigger
- Verify automation traces

### Production

- Set logging to INFO level
- Use stable WiFi connection
- Monitor system logs regularly
- Implement backup configuration
- Verify NVS persistence after power cycles
- Ensure automations are enabled and running
- Monitor automation traces for errors

### Multi-Device

- Same YAML configuration for all ESP32 devices
- Unique device names and API keys
- Separate area codes for different regions
- Centralized Home Assistant monitoring
- Separate automation instances per device
- Verify each device has its own automations enabled

## Directory Structure for Your ESPHome Project

When setting up v4.2.0, place the helper files **directly alongside** the main YAML file in your ESPHome project. ESPHome automatically includes `.cpp` and `.h` files from the same directory as your configuration file:

```
your-esphome-project/
├── entso-e-prices.yaml              # Main configuration (from v4.2.0/)
├── entsoe_storage_v2.h              # NVS storage helper (from v4.2.0/)
├── entsoe_http_idf.h                # HTTP client helper (from v4.2.0/)
└── secrets.yaml                     # Your credentials (from v4.2.0/secrets_template.yaml)
```

**Important:** Place both `.h` helper files in the **same folder** as `entso-e-prices.yaml`, NOT in a `src/` subfolder. ESPHome automatically includes all `.h` and `.cpp` files from the configuration directory.

**Automation files are imported into Home Assistant, not placed in this directory.**

## Downloading Files from Repository

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

### v4.2.0 Essential Automations (REQUIRED)

Navigate to the `v4.2.0/crucial_ha_automations/` folder to download required automations:

- **entsoe_midnight_automation.yaml**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/v4.2.0/crucial_ha_automations/entsoe_midnight_automation.yaml
- **entsoe_next_day_automation.yaml**: https://github.com/Legolas-2025/esp32-electricity-price-ticker/blob/main/v4.2.0/crucial_ha_automations/entsoe_next_day_automation.yaml

## Migration from v3.5.0

If you are upgrading from v3.5.0:

1. **All Credentials Now in secrets.yaml**: Unlike v3.5.0 where some values were hardcoded in YAML, v4.2.0 puts everything in secrets.yaml
2. **Update Framework**: Change from Arduino to ESP-IDF in ESPHome dashboard
3. **Add Helper Files**: Place `entsoe_storage_v2.h` and `entsoe_http_idf.h` alongside your YAML file
4. **Update Configuration**: Use `entso-e-prices.yaml` from v4.2.0 instead of `entsoe-hass-compatible.yaml`
5. **Create New secrets.yaml**: Copy `secrets_template.yaml` from v4.2.0 and fill in all values
6. **Clean Build**: Delete old `.esphome` build directory before compiling
7. **Select ESP-IDF**: Choose ESP-IDF framework when prompted by ESPHome
8. **Import Automations**: Download and import both automations from `v4.2.0/crucial_ha_automations/`
9. **Configure Notifications**: Replace `notify.mobile_app_sm_s911b` with your entity
10. **Test Persistence**: Verify NVS storage works after power cycle
11. **Verify Automations**: Check automation traces and status sensors
12. **Monitor Stability**: Confirm reboot issues are resolved

---

## Support Resources

- **Documentation**: Complete guides in README.md and QUICKSTART.md (main folder)
- **Wiki**: See `Wiki/v4.2.0 Wiki Home Page.md` for wiki-based documentation
- **Community**: ESPHome and Home Assistant forums
- **API Support**: ENTSO-E transparency platform
- **Hardware**: ESP32 manufacturer documentation
- **Repository**: https://github.com/Legolas-2025/esp32-electricity-price-ticker
