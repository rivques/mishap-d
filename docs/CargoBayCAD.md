# Cargobay Docs
## Basics
### Design purpose and sucess statement:
* The intent of this design is to create a box that can connect to the bottom of a drone and hold the electronics for all of our various components and also hold the dropping cargo.
* This design is succesful if: everything fits right without filing or scraping, all the components of the in-sky module can fit inside without interference, it is lightweight enough for the drone to comfortably move with good dexterity, and the cargo can be dropped at any time.
### Timeline:

## Design
###  The box consists of three main components:
* The physical box (All laser cut)
* The addons (Hinges, Servo mount, and the hatch lock)
* The landing gear

### The Box
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

![full box exploded view](https://github.com/rivques/mishap-d/assets/91289762/5324bdc1-6648-4a94-9335-00b292a45180)

* The laser-cut box is a two-section container with a center wall to prevent interference between the cargo and electronics.
* The electronics bay is comprised of a [] a battery pack and a raspberry pico.
* The wholes on the box are size #4-40 for the labs screws and #1-72 for the small ones
* The large whole on the electronics bay door is the lock. The point of the design is to be able to wrap a wire into the electronics bay side wall and twist it shut.
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

* All together the landing gear doesn't fit a stick perfectally squared and horizontal, this is intentional. Intentional because of the tension forces provided giving a structural bonus of strength and tensile forces going everywhere. This does, however, cause the landing gear to explode if the drone falls out of the sky.
<details><summary>Imperfect fit</summary> 
 
![landing gear imperfections](https://github.com/rivques/mishap-d/assets/91289762/d265ea25-eabb-4198-a236-f222ed34d6b3)
</details>
<details><summary>Bye Bye Landing Gear!</summary> 
 
![landing gear bomb num.1](https://github.com/rivques/mishap-d/assets/91289762/ffcf9fe4-5b3d-42f9-80c6-c5cbfefe46e5)

![landing gear bomb num.2](https://github.com/rivques/mishap-d/assets/91289762/5a3c16c8-40b6-4bbd-a4d1-9a455e789179)
</details>

