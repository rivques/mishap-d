%%{init: {"flowchart": {"defaultRenderer": "elk"}} }%%
flowchart TD
    Explanation(["Arrowheads are communication types\n Standard arrowhead = PWM\nCircle = bus (USB, SPI, etc)\n X = power only"])
    Transmitter
    Receiver
    Battery
    ESC["Electric Speed Controller"]
    PropMotor["Prop Motor"]
    CSS["Control Surface Servos"]
    Pico["Low-Level Controller (Pico)"]
    RasPi["Image Processor (RasPi)"]
    SDCard["SD Card"]
    Gimbal
    PiCam
    Altimeter
    FiveVolts(["5V rail"])
    Transmitter --o |"Wireless (DSM2)"|Receiver
    ESC --> PropMotor
    Battery --x |12V| ESC
    ESC --x |Supply from BEC| FiveVolts
    Receiver --> CSS
    Receiver --> |Throttle info| ESC
    FiveVolts --x RasPi
    FiveVolts --x CSS
    FiveVolts --x Gimbal
    FiveVolts --x Receiver
    FiveVolts --x Altimeter
    PiCam o--o RasPi
    Receiver --> |via AUX1, for control from ground| Pico
    PiCam --- |physically attached to| Gimbal
    Pico --o|Commands from pilot; logging| RasPi
    RasPi --o |"Gimbal tracking commands; power"|Pico
    Pico --> Gimbal
    Altimeter --o Pico
    RasPi --o |Logs| SDCard
