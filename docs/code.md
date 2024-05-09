# Code Structure
## `mishap-d/`
## `tests/`
This folder contains various test and helper programs for the ESP32. 
### `basic-radio-client/` and `basic-radio-server/`
This is a test program for using LoRa with the ESP32 and the RFM9x chips.
### `lidar_test/`
This program tests the Garmin LidarLitev4-LED.
### `i2c_scan/`
This is a program adapted from [Adafruit's](https://learn.adafruit.com/scanning-i2c-addresses/arduino) that scans for devices on the I2C bus of the ESP32.
### `seriallogtest/`
This is a repeater that just echoes its serial input out over UART2, used for testing the datalogger.
### `servoassistant`
This moves a servo back and forth between the open and closed positions, used for testing the payload bay.
## `*.py` and `lib/`
The `lib` folder and the various Python files in the root directory are designed for the Raspberry Pi Pico that's serving
as a datalogger. It's currently running `picologger.py` and `boot.py`, and `dataLogTest.py` was a previous experiment. This Pico listens to the UART2 peripheral of the ESP32 and saves all the data it sees to a log file for later inspection.
# History of Development