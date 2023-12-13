#type:ignore
import adafruit_bus_device
import busio
import board                                   
import time
from adafruit_lsm6ds.lsm6dsox import LSM6DSOX
import adafruit_mpl3115a2

sda_pin = board.GP14
scl_pin = board.GP15

i2c = busio.I2C(scl_pin, sda_pin)

lsm = LSM6DSOX(i2c_bus=i2c, address=0x6a)
mpl = adafruit_mpl3115a2.MPL3115A2(i2c, address=0x60)

st = time.monotonic()

mpl.sealevel_pressure = 1021
datacounter = 0

while True:
    with open("/DATASET (1).csv", "w") as datalog:
        ct = time.monotonic()
        accel_x, accel_y, accel_z = lsm.acceleration
        print("accel done")
        gyro_x, gyro_y, gyro_z = lsm.gyro
        print("gyro done")
        datalog.write(f"{float(st)},{float(ct)},{accel_x:.3f},{accel_y:.3f},{accel_z:.3f},{gyro_x:.3f},{gyro_y:.3f},{gyro_z:.3f}\n")
        datalog.flush()
        print(f"took data #{datacounter}")
        datacounter += 1
        time.sleep(0.01)