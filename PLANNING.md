# auto-high-altitude-payload-delivery
## The concept
We want to build a remote control airplane that, with a camera, can accurately find a target and drop a payload on it. There are two big challenges here: First of all, the plane has to figure out where it is with respect to the target. Second, the plane has to figure out when to drop the payload.

To simplify the problem, a human will control the plane and fly it on an approach over the target. The only thing the plane has control over is when it drops the payload. It is very important that the human flies the plane on a straight route over the target, because the plane will not be able to compensate for lateral error. (If we have extra time, we may attempt to change this with PID rudder control.)

To determine relative position, the plane will use computer vision. A gimbaled camera on the bottom of the plane will track the target. By keeping the target in the center of the frame, the angle between the plane and the target can be determined by the angle of the gimbal.

To determine when to drop the payload, the plane will be constantly running a physics simulation of what would happen if it chose that moment to drop. Only if the projected path would intersect the target, and the operator is giving the plane permission to drop, will the payload be released.

## Success statement:
* The plane will fly, collect data, and store data on a pico.
* The plane will meet all requirements to recieve an A in Engineering 4.
* The plane will accurately drop payloads on target.
## A Big Flowchart
![A big flowchart, code in system-diagram.mflow](/assets/system-diagram.png)
<details>
<summary>Explanation</summary>
asdfsdf
</details>

## The roadmap
1. get the easy points (w/ altitude sensor and nothing else)
2. do the cool stuff
## What we already have
plane (dumb)
## What problems we will need to solve
The plane has to calculate its location relative to its target and decide on the perfect time to drop the payload.
* Plane cant see
* plane cant do math
* plane doesnt understand what a target is
* plane cant fly
* plane cant see
* plane cant store data
* 

## Parts we will need
* Raspberry Pi Pico
* Raspberry Pi B Model ?
* DSM2 Reciever
* DSM2 Transmitter
* Model Airplane
* Servo motors
* 3S LiPo battery
* 
## Safety
we will not get hit wit the plane
we will charge batteries safely
we will not drop stuff on people who dont want stuff to be droppd on them
## Schedule
1. cad payload bay and electronics bay