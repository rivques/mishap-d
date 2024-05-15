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
| Circle  | This is the lidar, it measures the height of the box. The orientation is down, towards the ground  | <img src="https://github.com/rivques/mishap-d/assets/91289762/0c6c89df-119a-4e4c-913e-5d7756eb4ee2c" alt="lidar and mount" width="400" height="400"> |
| Triangle  | Electronics bay door. The locks on the side are intended to lock it in place. It can't fall into the box or out of the box, therefore, stuck.  | <img src="https://github.com/rivques/mishap-d/assets/91289762/9e82a09a-c1d4-48b8-99a6-dc7143d2aebc" alt="electronics bay door" width="400" height="400"> |
| Pentagon | This is the bracket that Mr. Miller made, it is used to lock the electronics bay door and  very helpful for full assemblies because it locks a nut inside its jaws and prevents it from falling out. | <img src="https://github.com/rivques/mishap-d/assets/91289762/4f1fe433-bf0c-4b72-9d92-66867a009ef3" alt="Bracket Locking electronics bay door" width="400" height="400"> |






The problems with this box:
* The door on the side is intened for access to the wires. When put together there wasn't enough space to do what it was designed for: Accessing the wires with the ability to change them around.
* There was no switch hole
* You couldnt access the USB port on the microprocessor without taking it out.
* The hinges were too tight on the bolt that held them in place, therefore they would be unable to open due to just gravity.
* The big hole on the side of the back wall is designed to be an opening for the SD card reader and writer and it was mismeasured
* The holes that were intended to screw into the hinges interfered with the box's brackets.
</details>

### Current box:
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
