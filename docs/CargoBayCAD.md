# Cargobay Docs
## Basics
### Design purpose and success statement:
* This design intends to create a box that can connect to the bottom of a drone and hold the electronics for all of our various components and also hold the dropping cargo.
* This design is successful if: everything fits right without filing or scraping, all the components of the in-sky module can fit inside without interference, it is lightweight enough for the drone to comfortably move with good dexterity, and the cargo can be dropped at any time.
* If a picture is worth a thousand words then an STL is worth a million! Please click on the STL renderings to truly conceptualize my design choices!
  
### Timeline:

## Design
###  The box consists of three main components:
* The physical box (All laser cut)
* The addons (Hinges, Servo mount, and the hatch lock)
* The landing gear

## The Box
<details>
<summary>Trials and errors of the box </summary>

### The box went through two renditions plus a couple renovations.
[Originally the box looked like this:](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/boxtry1-try2.md)

### Box Label and Purposes:
 <div class="image-container">    
    <a href="https://github.com/rivques/mishap-d/edit/main/docs/CargoBayCAD.md">    
      <img src="https://github.com/rivques/mishap-d/assets/91289762/71ebeea5-5f6d-4a5d-a75d-fd87f49da171" alt="GitHub" width="580" height="500">    
    </a>

  </div>  
  
| Sign  |  Object and Intended Purpose | Photo |
| ------------- | ------------- | ---- |
| Circle  | This is the lidar, it measures the height of the box. The orientation is down, towards the ground  | ![lidar mount](https://github.com/rivques/mishap-d/assets/91289762/b8dd66a4-f906-4cd0-a5d5-ab57b53b2111) |
| Triangle  | Electronics bay door. The locks on the side are intended to lock it in place. It can't fall into the box or out of the box, therefore, stuck.  | ![SIDE ELECTRONICS DOOR](https://github.com/rivques/mishap-d/assets/91289762/6bdf5b7f-37a0-4f17-b8a0-940c22bf9030) |
| Pentagon | This is the bracket that Mr. Miller made, it is used to lock the electronics bay door and  very helpful for full assemblies because it locks a nut inside its jaws and prevents it from falling out. | ![bracket locking electronics bay door](https://github.com/rivques/mishap-d/assets/91289762/478cef75-d241-4ef0-9613-791eaa76ad99) |
| Arrow | This is the release lock. It is a lock that is connected to a servo and when the servo moves the lock is released and the door can open and drop the payload. | ![servo lock](https://github.com/rivques/mishap-d/assets/91289762/84286759-9981-40ea-a782-b1483bb3ff6f) |





The problems with this box:
* The door on the side is intended for access to the wires. When put together there wasn't enough space to do what it was designed for: Accessing the wires with the ability to change them around.
* There was no switch hole
* You couldn't access the USB port on the microprocessor without taking it out.
* The hinges were too tight on the bolt that held them in place, therefore they would be unable to open due to just gravity.
* The big hole on the side of the back wall is designed to be an opening for the SD card reader and writer and it was mismeasured
* The holes that were intended to screw into the hinges interfered with the box's brackets.
  
</details>

### Current box:

<img src="https://github.com/rivques/mishap-d/blob/15924a79be7bf9a228da18630f715c2e00e89422/assets/Assembly%202.png" width="600">

* The laser-cut box is a two-section container with a center wall to prevent interference between the cargo and electronics.
* The electronics bay is comprised of a [] battery pack and a Raspberry Pico.
* The wholes on the box are sizes #4-40 for the lab's screws and #1-72 for the small ones
* The large hole on the electronics bay door is the lock. The point of the design is to be able to wrap a wire into the electronics bay side wall and twist it shut.
  <details>
  <summary>Photo Example</summary>
  <div class="image-container">    
    <a href="https://github.com/rivques/mishap-d/edit/main/docs/CargoBayCAD.md">    
      <img src="https://github.com/rivques/mishap-d/assets/91289762/6ee7e501-42f9-4481-b5fa-4350b2900304" alt="GitHub" width="500" height="700">    
    </a>
  </div>  
  </details>

  ## Landing Gear
  
* The Landing gear only went through one rendition. To make them I used a plane and connected a tube to the end of a corner cap and used that same plane to connect to the entry slot.

[What it looks like](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/endcap.md)

[The entry slot](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/receiver.md)

* All together the landing gear doesn't fit a stick perfectly squared and horizontally, this is intentional. Intentional because of the tension forces provided giving a structural bonus of strength and tensile forces going everywhere. This does, however, cause the landing gear to explode if the drone falls out of the sky.
<details><summary>Imperfect fit</summary> 
 
![landing gear imperfections](https://github.com/rivques/mishap-d/assets/91289762/d265ea25-eabb-4198-a236-f222ed34d6b3)
</details>
<details><summary>Bye Bye Landing Gear!</summary> 

 
![landing gear bomb num.1](https://github.com/rivques/mishap-d/assets/91289762/ffcf9fe4-5b3d-42f9-80c6-c5cbfefe46e5)

![landing gear bomb num.2](https://github.com/rivques/mishap-d/assets/91289762/5a3c16c8-40b6-4bbd-a4d1-9a455e789179)
</details>

## Addons
### Hinges!
![opening doors](https://github.com/rivques/mishap-d/assets/91289762/68b1797e-d195-4143-83e4-84167e003d6e)
* There are two sets of hinges, one that goes on a wall-to-floor relationship and creates a 90-degree angle and one that has a floor-to-floor relationship with a 180-degree relationship.

| Catagory | 90 Degree Hinges | 180 Degree Hinges |
| -------- | ---------------- | ----------------- |
| STL file view | [Wall-to-Floor](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/angled%20hinges.md) | [Floor-to-Floor](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/straight%20hinges.md) |
| Photo example | <img src="https://github.com/rivques/mishap-d/assets/91289762/a7cbe299-030f-4916-95d3-27764919067f" width="500" height="500" /> | <img src="https://github.com/rivques/mishap-d/assets/91289762/fad67f51-4504-476e-8fad-b35a5d2c8554" width="500" height="500" /> |

### Servo Mount
* The servo mount was created because of my laziness, but it's the best thing to come out of this lab in years.
* The servo mount requires two holes in the wall and it wraps around the servo and tightens with a self-engraving bolt.
* It doesn't fall because the system is screwed in from the outside with a washer to ensure the screw applies pressure to the wall and the servo mount.
* The mount offers additional support to the servo by having a space for its screw holes, the platform is held in place by surrounding material.

  [Here be the STL](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/servo%20mount%20on%20wall.md)
  
| Top Right Profile | Section View Profile |
| ---- | ---- |
| ![front profile servo mount](https://github.com/rivques/mishap-d/assets/91289762/468412ae-5729-44a5-9b58-eb9be3fedeec) | ![side profile screw washer servo mount](https://github.com/rivques/mishap-d/assets/91289762/0ba4db49-f677-4ae6-b2b6-89ad9c74a286) |

### Drone Mounts
* Very simple design intended to screw the Cergo Bay onto the drone
* 
[Rendered version of STL](https://github.com/rivques/mishap-d/blob/main/docs/STLFILES/drone%20mount.md)



## Wiring
The cargo bay wiring was prototyped on a breadboard but was transferred to a protoboard for rigidity. Below is the actual wiring of the board (our original plan is [here](/payload-circuit.fzz))
* 9V in to ESP32 VIN and input leg of voltage regulator
* Servo: Power -> ESP32 VIN, GND -> GND, Signal -> ESP32 D27
* MPL3115A2 (altimeter): MPL SDA -> ESP32 D21, MPL SCL -> ESP32 D22, GND -> GND, MPL VIN -> voltage regulator output
* Raspberry Pi Pico: ESP32 TX2-> Pico GP9, GND -> GND, and Pico VSYS -> voltage regulator output
* Debug LED: Cathode -> GND, Anode -> ESP32 D32
* Voltage regulator: 33µF cap across GND and VIN, 0.1µF cap across GND and VOUT
