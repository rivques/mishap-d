flowchart TD
    Receiver
    CSS["Control Surface Servos"]
    Pico["Low-Level Controller (Pico)"]
    RasPi["Image Processor (RasPi)"]
    Gimbal
    PiCam
    Receiver --> CSS
    PiCam <--> RasPi
    Receiver --> |via aux channel|Pico
    PiCam --- |attached to| Gimbal
    Pico -->|Commands from pilot| RasPi
    RasPi --> |Gimbal tracking commands|Pico
    Pico --> Gimbal
