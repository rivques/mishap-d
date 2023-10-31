flowchart TB
    Transmitter
    Receiver
    Battery
    ESC["Electric Speed Controller"]
    PropMotor["Prop Motor"]
    CSS["Control Surface Servos"]
    Pico["Low-Level Controller (Pico)"]
    SDCard["SD Card"]
    Gimbal
    PiCam
    Altimeter
    PayloadBayDoor["Payload Bay Doors"]
    FiveVolts(["5V rail"])
    subgraph RasPi["Image Processor (RasPi)"]
        ImageInput[["Image Input"]]
        --> ScaleImage["Scale Image"]
        --> HSVThreshold["HSV Threshold"]
        --> BlobDetection["Blob Detection"]
        --> FunWithTrig["Fun with Trigonometry"]
        --> LocInfo[("LocInfo")]
        --> PhysicsPrediction["Predict flight of payload"]
    end
    FunWithTrig --> |"Keep gimbal pointing at target"| Pico
    LocInfo --o |"Logs"| SDCard
    PhysicsPrediction --o |"If trajectory hits target, release payload"| Pico
    Altimeter --o Pico
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
    FiveVolts --x PayloadBayDoor
    PiCam o--o ImageInput
    Receiver --> |via AUX1, for control from ground| Pico
    PiCam --- |physically attached to| Gimbal
    Pico --> Gimbal
    Pico --> PayloadBayDoor
    %% invisible links to pull the flowchart to be a bit more meaningful
    FunWithTrig ~~~ Pico
    Explanation(["Arrowheads are communication types\n Standard arrowhead = PWM\nCircle = bus (USB, SPI, etc)\n X = power only"])
