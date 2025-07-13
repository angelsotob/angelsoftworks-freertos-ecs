# AngelSoftWorks FreeRTOS ECS

AngelSoftWorks FreeRTOS ECS is a lightweight and well-structured template for embedded development using FreeRTOS on ESP32. It is designed for educational use, internal development, and real-world prototyping of embedded systems.

## ✅ Features

- Modular FreeRTOS application with separate task layers.
- GPIO control for blinking and output signal handling.
- Simulated temperature and humidity sensors.
- Wi-Fi connectivity in STA (Station) mode.
- SNTP time synchronization with retry mechanism.
- Doxygen-style documentation and code comments.
- Clean project layout with `main/`, `include/`, and `tasks/`.

## 📁 Project Structure

```
.
├── include/            # Global configuration headers
│   ├── config.h
│   └── wifi_config.h
├── main/                # Application source code
│   ├── main.c
│   ├── app/            # Core app components (e.g., gpio, time sync)
│   ├── tasks/          # FreeRTOS task implementations
│   ├── drivers/        # Hardware abstraction (fake sensors)
│   ├── network/        # Wi-Fi STA connection
│   └── utils/          # Utility functions (e.g., timestamp formatting)
├── tests/              # Unit or test mocks
│   └── test_dummy.c
├── CMakeLists.txt      # Main build definition
├── scripts/            # Optional helper scripts (build, flash)
│   └── esp_build.sh
├── sdkconfig           # ESP-IDF configuration
├── README.md
└── LICENSE
```

## 🚀 Getting Started

### Requirements

- [ESP-IDF v4.4 or newer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)
- An ESP32 development board
- Python 3.6+, Git, and required IDF dependencies

### Build and Flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

> Replace `/dev/ttyUSB0` with the actual port your ESP32 is connected to.

## 🧠 Application Overview

| Task Name     | Description                                      |
|---------------|--------------------------------------------------|
| `blink_task`  | Toggles an LED connected to a defined GPIO pin   |
| `sensor_task` | Simulates and logs temperature/humidity readings |
| `control_task`| Sets an output pin based on simulated data       |

Additionally, the system connects to Wi-Fi in STA mode and synchronizes its time using SNTP.

## ⚙️ Configuration

Update the following definitions in `include/wifi_config.h`:

```c
#define WIFI_SSID "your-network"
#define WIFI_PASS "your-password"
```

Adjust GPIO and timing in `include/config.h` as needed.

## 📚 Documentation

All source files are annotated using [Doxygen](https://www.doxygen.nl/), making it easy to generate HTML or LaTeX docs.

To generate documentation:

```bash
doxygen Doxyfile
```

(You’ll need to create a suitable `Doxyfile`.)

## 🛠 Future Improvements

- Replace fake sensors with real drivers (I2C, ADC, etc.)
- Add unit tests in `tests/`
- Integrate OTA updates and secure Wi-Fi provisioning
- Migrate tasks to a more scalable ECS-style architecture (if needed)

## 📄 License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.

---

**Made with ❤️ by Ángel Soto Boullosa — AngelSoftWorks**
