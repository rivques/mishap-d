# Code Structure
## `mishap-d/`
### `config.h`
This file contains many `#define`s that configure the system. Besides setting things like pinouts, this file also configures which subsystem the program gets built for (one of `GROUND`, `PAYLOAD`, and `PAYLOADONLY`).
### `groundcontroller.h`
This is the firmware for the ground station. It's in charge of managing the display interface, reading the tracking potentiometers, and communicating via LoRa with the payload. 
### `mishapprotocol.h`
This is the shared protocol that's used to communicate between the gorund station and the payload. It also has some shared linear algebra definitions.
### `payloadcontroller.h`
This is the firmware for the payload. If it's running with aground istation, it receives information from the ground station and its altimeter, calculates its position, projects forward the path of its payload, decides whether to drop its payload, and logs data. If it's running without a ground station, it monitors its altitude and logs data.
### `mishap-d.ino`
This is the entry point of the program and immediately calls out to the payload or ground specific functions in the other files.
### `graham-interface.ino.dontcompile`
This is the display code that Graham worked on that was integrated into `groundcontroller.h`. The `.dontcompile` extension exists because the Arduino extension for VS code was trying (and failing) to build it when I wanted to upload other code.
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
The first things developed were the physics simulation and the radio protocol. This happened roughly over the course of January. By the end of January development of the menu on the ground station had also started, and would continue throughout February. Meanwhile, an attempt was being made to support multithreading of radio duties. This attempt was ultimately abandoned. At the start of March work was done on SD card logging, and then the display code was integrated with the existing ground station code. The payload circuit board was designed and assembled, and by mid-April debugging was being done. Towards the end of April a failure in the LIDAR sensor led to a pivot to a barometric altimeter, and this pivot was complete by early May. Around the same time frame, corruption issues with the SD card led to the use of a Raspberry Pi Pico for logging instead.