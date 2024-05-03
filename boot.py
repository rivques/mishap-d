# SPDX-FileCopyrightText: 2021 Kattni Rembor for Adafruit Industries
# Modified by Matthew Miller, Charlottesville High School
# Further modified by River Lewis

# SPDX-License-Identifier: MIT

"""
boot.py file for Pico data logging example. If pin GP0 is NOT connected to GND when
the pico starts up, make the filesystem writeable by CircuitPython.
"""
import board
import digitalio
import storage
import time

write_pin = digitalio.DigitalInOut(board.GP0)
write_pin.direction = digitalio.Direction.INPUT
write_pin.pull = digitalio.Pull.UP

led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT
time.sleep(2)

# If write pin is connected to ground on start-up, CircuitPython can write to CIRCUITPY filesystem.
if write_pin.value: # Data Mode, shown by 10 short blinks
    storage.remount("/", readonly=False)
    for i in range(10):
        led.value = True
        time.sleep(0.05)
        led.value = False
        time.sleep(0.05)

else: # Code Mode, shown by 5 short blinks
    for i in range(5):
        led.value = True
        time.sleep(0.05)
        led.value = False
        time.sleep(0.05)