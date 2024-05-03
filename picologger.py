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

# Set up the uart
uart = busio.UART(board.GP8, board.GP9, baudrate=115200)

# Set up the LED
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

while True:
    # take all serial data and store it in a string
    # then, on the falling edge of the write pin, write the string to a file
    data = ""
    while uart.in_waiting:
        char = str(uart.read(1), 'utf-8')
        data += char
        if char == '/n':
            led.value = True
            with open(filename, "a") as f:
                f.write(data)
            led.value = False