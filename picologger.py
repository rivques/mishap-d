# logs everything that comes in over UART1 to a file

import time
import board
import busio
import digitalio
import os

fileprefix = "/logs/mission_"
fileext = ".csv"
filecounter = len(os.listdir("/logs")) + 1
filename =f"{fileprefix}{filecounter:03}{fileext}"
print(f"Logging to {filename}")

# Set up the LED
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT
# blink out the file number
# 10s place:
for i in range(filecounter//10):
    led.value = True
    time.sleep(0.1)
    led.value = False
    time.sleep(0.1)
time.sleep(0.5)
# 1s place:
for i in range(filecounter%10):
    led.value = True
    time.sleep(0.1)
    led.value = False
    time.sleep(0.1)

# Set up the uart
uart = busio.UART(board.GP8, board.GP9, baudrate=115200)

while True:
    # take all serial data and store it in a string
    # then, on the falling edge of the write pin, write the string to a file
    data = ""
    while uart.in_waiting:
        try:
            char = str(uart.read(1), 'utf-8')
        except UnicodeError:
            char = "?"
        print(char, end="")
        data += char
        if char == '\n':
            led.value = True
            with open(filename, "a") as f:
                f.write(data)
            led.value = False
            data = ""