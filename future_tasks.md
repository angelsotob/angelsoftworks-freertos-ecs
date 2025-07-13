
# Future Improvements and Development Ideas

This document lists potential improvements and additional features for the current FreeRTOS-based embedded system. These ideas aim to extend functionality, improve robustness, and explore new hardware integration.

---

## 🧠 System Enhancements

### 1. Sensor Data Validation
- Implement moving average, weighted average, or basic Kalman filter on `sensors_data_t`.
- Detect and discard out-of-range or inconsistent values.

### 2. Visual Indicators / LED Debugging
- Use specific LED blinking patterns to indicate task states (logger active, watchdog issues, etc.).
- Helps with field debugging without a console.

### 3. Logger Task Enhancements
- Save data to **flash** or SD card if the network is unavailable.
- Add log levels: `INFO`, `WARN`, `ERROR`.
- Allow disabling logs via `config.h`.

### 4. Dynamic Control via UART or Web
- Enable/disable tasks (logger, sensors...) from UART interface.
- Adjust sampling interval or task delay during runtime.

---

## 🌡️ Sensor-Related Improvements

### 1. Dedicated Drivers for Real Sensors
- Create drivers like `dht.c`, `sht30.c` under `main/drivers/`.
- For I2C sensors, use a helper like `i2cdev.h` in `main/utils/`.

### 2. Temperature Compensation
- Compensate readings if humidity and temperature sensors are physically separate or sampled at different times.

### 3. Auto-Calibration on Startup
- Capture initial readings as baseline or calibration reference.

---

## 📡 Connectivity & Backend Extensions

### 1. Send Data via MQTT or HTTP POST
- Use local or cloud brokers (e.g., Mosquitto, Node-RED).
- Requires new tasks: `network_task`, `mqtt_task`, etc.

### 2. Embedded Web Server (Optional)
- Use `esp_http_server.h` to create a simple endpoint returning the latest `system_state_t`.

---

## 🧪 Testing / Diagnostics

### 1. Autotest Task
- Check if all `task_alive_flags_t` fields are `true`.
- Restart or report dead tasks.

### 2. Logging with Checksum
- Store logs temporarily in RAM or flash with CRC verification.
- Useful for crash recovery and debugging.

---

## 🔋 Power Management

- Suspend tasks or peripherals when idle.
- Disable WiFi if sensor data remains stable.
- Use this for future battery-powered versions.

---

## 🧱 Ideas with Discrete Components

### 1. Relay or Transistor Control Based on Humidity
- Automatically turn on/off devices (e.g., pump, fan) depending on humidity threshold.

### 2. Buzzer Alert
- Activate a buzzer if humidity exceeds critical levels.

### 3. Demo Mode
- A button that triggers LED sequences and mock sensor values for demonstration purposes.

---

## Next Steps

- Prioritize and track relevant items in Jira or your preferred task manager.
- Group related improvements into epics or themes (e.g., "Diagnostics", "Connectivity").

