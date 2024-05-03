# logs everything that comes in over UART1 to a file

import time
import board
import busio
import digitalio

# Set up the uart
uart = busio.UART(board.GP8, board.GP9, baudrate=115200)

# Set up the write pin
write_pin = digitalio.DigitalInOut(board.GP7)
write_pin.direction = digitalio.Direction.INPUT
write_pin.pull = digitalio.Pull.UP

# Set up the LED
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

while True:
    # take all serial data and store it in a string
    # then, on the falling edge of the write pin, write the string to a file
    data = ""
    while uart.in_waiting:
        data += str(uart.read(1), 'utf-8')
    if not write_pin.value:
        with open("/log.txt", "a") as f:
            f.write(data)