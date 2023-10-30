%%{init: {"flowchart": {"defaultRenderer": "elk"}} }%%
flowchart TD
    Explanation("Arrowheads are communication types\n Standard arrowhead = PWM\nCircle = bus type (USB, SPI, etc)\n X = wireless\n Line type denotes if power is sent\nStandard = both comms and power\nThick = comms only\nDotted = Power only")
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
    Transmitter ==x Receiver
    ESC --> PropMotor
    Battery -.-> |12V| ESC
    ESC -.-> |5V| Receiver
    Receiver --> CSS
    Receiver ==> ESC
    ESC -.-> |5V| RasPi
    PiCam o--o RasPi
    Receiver ==> Pico
    PiCam --- |physically attached to| Gimbal
    Pico ==o|Commands from pilot; logging| RasPi
    RasPi --o |Gimbal tracking commands|Pico
    Pico --> Gimbal
    RasPi --o |Logs| SDCard
