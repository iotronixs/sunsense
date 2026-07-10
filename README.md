🌞 ☀️ 🛡️

```
    ███████╗██╗   ██╗███╗   ██╗███████╗███████╗███╗   ██╗███████╗███████╗
    ██╔════╝██║   ██║████╗  ██║██╔════╝██╔════╝████╗  ██║██╔════╝██╔════╝
    ███████╗██║   ██║██╔██╗ ██║███████╗█████╗  ██╔██╗ ██║███████╗█████╗  
    ╚════██║██║   ██║██║╚██╗██║╚════██║██╔══╝  ██║╚██╗██║╚════██║██╔══╝  
    ███████║╚██████╔╝██║ ╚████║███████║███████╗██║ ╚████║███████║███████╗
    ╚══════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚══════╝╚═╝  ╚═══╝╚══════╝╚══════╝
```

**Real-time UV Radiation Monitoring with ESP32-C3**

# SunSense UV Monitoring Device
Build For CREATEd

## Project Overview
SunSense is an IoT-enabled UV radiation monitoring device designed to measure and display ultraviolet (UV) levels in real-time. It utilizes an ESP32-C3 microcontroller for processing, a GY-ML8511 UV sensor for accurate readings, and a 1.8-inch SPI TFT display (128x160 resolution) for visualizing data. The project includes data storage capabilities for logging UV exposure over time, making it ideal for applications in personal health monitoring, outdoor activities, or environmental sensing.

This project demonstrates integration of sensors, displays, and storage on a compact ESP32-C3 Supermini board, with code written in Arduino-compatible C++.

## Features
- **Real-time UV Sensing**: Measures UV intensity using the GY-ML8511 sensor.
- **Graphical Display**: 1.8" SPI TFT screen for dashboard visualization.
- **Data Storage**: Persistent storage for UV data logs.
- **Low-Power Design**: Optimized for battery-powered operation on the ESP32-C3 supermini.
- **Modular Codebase**: Separated into dashboard, storage, and main modules for easy maintenance.

## Hardware Requirements
- **Development Board**: ESP32-C3 Supermini
- **UV Sensor**: GY-ML8511 (connected to pin 0, powered at 3.3V)
- **Display**: 1.8" SPI TFT 128x160
    - Pin Configuration:
        - RST: 10
        - CS: 7
        - DC: 6
        - DIN: 3
        - CLK: 2
        - VCC: 5V
        - BL: 4
        - GND: GND

## Software Requirements
- **Language**: C++ (Arduino framework)
- **IDE**: Arduino IDE or compatible (e.g., PlatformIO)
- **Libraries**:
    - ESP32 core libraries
    - TFT display library (e.g., Adafruit_GFX, Adafruit_ST7735)
    - UV sensor library (e.g., custom or GY-ML8511 compatible)

## Installation
1. Clone the repository: `git clone https://github.com/your-repo/sunsense.git`
2. Open `sunsense_main/sunsense.ino` in Arduino IDE.
3. Install required libraries via Arduino Library Manager.
4. Upload the code to ESP32-C3 Supermini.
5. Connect hardware as per pin configurations.

# Arduino Installation Guide

## Prerequisites
- Windows, macOS, or Linux operating system
- Administrative access to your computer
- Internet connection

## Step 1: Download Arduino IDE

1. Visit [arduino.cc](https://www.arduino.cc/en/software)
2. Select your operating system
3. Download the latest Arduino IDE version

## Step 2: Install Arduino IDE

### Windows
1. Run the installer executable
2. Follow the installation wizard
3. Accept the license agreement
4. Choose installation directory
5. Complete the setup

### macOS
1. Open the downloaded `.zip` file
2. Drag Arduino app to Applications folder

### Linux
1. Extract the downloaded archive
2. Navigate to the folder and run `./install.sh`

## Step 3: Install Required Libraries

1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for your required library
4. Click **Install** next to the library name
5. Wait for installation to complete

## Common Libraries
- **Wire** - I2C communication
- **SPI** - Serial Peripheral Interface
- **Servo** - Servo motor control
- **LiquidCrystal** - LCD displays

## Verify Installation

1. Connect Arduino board via USB
2. Go to **Tools** → **Board** and select your board
3. Select **Tools** → **Port** and choose the COM port
4. Test with File → Examples → Basics → Blink
5. Upload and verify LED blink

## Troubleshooting
- Update USB drivers if port doesn't appear
- Check board compatibility
- Ensure library versions match your Arduino model
## Installing ESP32-C3 SuperMini Board

1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. In "Additional Boards Manager URLs", add:
    ```
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```
4. Click **OK**
5. Go to **Tools** → **Board** → **Boards Manager**
6. Search for "esp32"
7. Install "esp32" by Espressif Systems
8. Go to **Tools** → **Board** → **ESP32 Arduino**
9. Select **ESP32-C3 Dev Module**
10. Configure port and upload speed (115200 baud recommended)
11. Test with File any of the test codes from the test_codes folder - use led blink
## Troubleshooting Upload Issues

### Serial Port Not Appearing
1. Install the CP2102 USB driver:
    - Search "CP2102 driver download" on Google
    - Download the appropriate version for your OS
    - Run the installer and follow instructions
    - Restart Arduino IDE
2. Alternative drivers: CH340, PL2303 (depending on your board)
3. Check Device Manager (Windows) or System Report (macOS) for unknown devices

### Enable CDC Boot for Serial Communication
1. Go to **Tools** → **Board** → **ESP32-C3 Dev Module**
2. Select **Tools** → **USB CDC On Boot** → **Enable**
3. This allows serial communication over USB without additional drivers

### Upload Fails
1. Verify correct board selected: **Tools** → **Board** → **ESP32-C3 Dev Module**
2. Verify correct port selected: **Tools** → **Port** → select COM port
3. Set upload speed to **115200** baud: **Tools** → **Upload Speed**
4. Try holding **BOOT** button during upload if issues persist

### Serial Monitor not working
1. Hold boot button, press reset and leave booth button to reset the serial monitor

### General Tool Configuration Checklist
- Board: ESP32-C3 Dev Module
- Flash Mode: DIO
- Flash Frequency: 80MHz
- Upload Speed: 115200
- USB CDC On Boot: Enabled
- Core Debug Level: None


## Usage
- Power on the device.
- The TFT display will show the UV dashboard with real-time readings.
- Data is automatically logged to storage for later retrieval.
- Monitor UV levels to protect against overexposure.

## Code Structure
- `sunsense.ino`: Main Arduino sketch handling setup and loop.
- `dashboard.cpp` / `dashboard.h`: Manages dashboard rendering and UI.
- `storage.cpp` / `storage.h`: Handles data persistence and logging.

## Contributing
Feel free to fork and submit pull requests. Ensure code follows Arduino best practices.

## License
This project is licensed under the MIT License.

## Author
[@jpg.py](https://github.com/jpg.py)

## Date
2026-27-04
