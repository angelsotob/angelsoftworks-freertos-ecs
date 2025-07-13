# AngelSoftWorks FreeRTOS ECS

**AngelSoftWorks FreeRTOS ECS** is a modular and educational template designed for embedded development on the ESP32 using FreeRTOS. It is intended for learning, internal prototyping, and small-scale real-world applications.

## ✅ Features

- Modular task-based architecture using FreeRTOS
- GPIO control for LED blinking and system output
- Simulated temperature and humidity sensors
- Wi-Fi connectivity in STA (Station) mode
- SNTP time synchronization with retry mechanism
- Clean and documented C code with Doxygen-style comments
- Organized directory structure for scalability and clarity

## 📁 Project Structure

```
.
├── include/            # Global configuration and shared data structures
│   ├── config.h
│   ├── system_state.h
│   └── wifi_config.h
├── main/               # Application source code
│   ├── main.c
│   ├── app/            # Core modules (e.g., GPIO, SNTP)
│   ├── tasks/          # FreeRTOS task implementations
│   ├── drivers/        # Sensor simulation (fake data)
│   ├── network/        # Wi-Fi initialization (STA mode)
│   └── utils/          # Utility functions (e.g., timestamp)
├── tests/              # Test scaffolding (optional)
│   └── test_dummy.c
├── scripts/            # Custom build/flash helpers (optional)
│   └── esp_build.sh
├── sdkconfig           # ESP-IDF project configuration
├── CMakeLists.txt      # CMake project definitions
└── README.md
```

## 🚀 Getting Started

### Requirements

- [ESP-IDF v4.4+](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)
- Python 3.6+, CMake, Git
- An ESP32 development board
- Serial access to flash and monitor the device

### Build and Flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

> Replace `/dev/ttyUSB0` with the actual serial port of your board.

---

## 🧠 Application Overview

This project creates a set of coordinated FreeRTOS tasks:

| Task          | Purpose                                                  |
|---------------|-----------------------------------------------------------|
| `blink_task`  | Toggles a GPIO pin to simulate LED blinking               |
| `sensor_task` | Generates simulated temperature and humidity readings     |
| `control_task`| Sets an output pin based on sensor thresholds             |
| `logger_task` | Logs system state and sensor data via UART and ESP_LOGI  |
| `watchdog_task` | Monitors task activity and resets system on failure     |

Time synchronization is handled via SNTP. Wi-Fi is configured in station mode.

---

## ⚙️ Configuration

### Wi-Fi Credentials

Edit the following in `include/wifi_config.h`:

```c
#define WIFI_SSID "your-ssid"
#define WIFI_PASS "your-password"
```

### GPIOs and Thresholds

Update `include/config.h` to change pin numbers, blinking times, and sensor thresholds.

---

## 📚 Documentation

All source files include [Doxygen](https://www.doxygen.nl/) annotations.

To generate HTML documentation:

```bash
doxygen Doxyfile
```

> You’ll need to create a valid `Doxyfile` manually (`doxygen -g`) or use a template.

---

## 🔧 Future Work

- Support for real hardware sensors via I2C or ADC
- Add unit tests and mocks in the `tests/` directory
- Implement OTA firmware updates
- Expand the watchdog system to cover additional failures
- Optionally refactor to true ECS architecture

---

## 📄 License

This project is released under the MIT License. See [`LICENSE`](./LICENSE) for details.

---

**Created with ❤️ by Ángel Soto Boullosa — AngelSoftWorks**
