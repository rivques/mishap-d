<!-- You have some errors, warnings, or alerts. If you are using reckless mode, turn it off to see inline alerts.
* ERRORs: 0
* WARNINGs: 0
* ALERTS: 15 -->


## Ground station CAD


## Design requirements

For the drop to be accurate the ground station needed to measure the angle between the horizontal plane and the drone as well as the change in angle with the ground station while it moved horizontally. The station is also required to contain an LCD for a menu to interact with the code and a set of pushbuttons for the arm command as well as the usual power and LCD requirements.


![alt_text](images/image1.png "image_tooltip")


Caption: This is a section view facing from the front split through the middle of the project.


## CAD Breakdown

The ground station was designed to track a moving plane and initiate a launch. The tracking is done to gather information about the plane that is going to be used to calculate the drop point for our projectile effectively guiding in our plane.


### The Trigger grip

The handle is the most complex part of the Ground station and includes a functioning trigger LCD and an arm button. All these electronics fit into a tight handle that is detachable from the main assembly. The most difficult part other than the integration of the electronics was the surfacing of the handle to the top where the LCD and trigger were housed. 


![alt_text](images/image2.png "image_tooltip")

![alt_text](images/image3.png "image_tooltip")


Caption: This is the surfacing done to seamlessly mesh the already-designed forward grip and the top of the grip where the LCD would be placed. This also creates a place for the trigger to be placed.


![alt_text](images/image4.png "image_tooltip")


Caption: This is the button layout for the trigger grip


### Wire Pathing

Wire pathing on this project was one of the most complex parts of the CAD requiring rotating parts that are still connected by wires. This can either be done through a slip ring which I would have to buy or design myself the latter option requiring at least a few weeks of work. Another way to path wires is to allow the wires to move freely within the joint although this restricts the movement of the joint. I chose the less complicated option of allowing the wires to move freely.

 \

![alt_text](images/image5.png "image_tooltip")


Caption: This is the general overview of how the wires were packed from the trigger grip to the main control computer at the base of the station.


![alt_text](images/image6.png "image_tooltip")


Caption: This is one of the joints where one disc is rotating parallel to the other disc, the top disc has wires coming in from a hole in the plate that is not close to the center while the other stationary disk has a whole closer to the center but still not directly at it. This design allows the wires to be wrapped around the main connection point and give the design more than 180  degrees of rotation.


![alt_text](images/image7.png "image_tooltip")


Caption: This is the second joint and is more simple than the first one where instead of the wires moving, a large hole has been placed for the part to move freely and not get stuck on the wires this restricts more of the movement of the rotation but it's also much simpler to construct.


### Sights 

To calculate the angle The ground station needed to be pointed at the plane originally this was going to be done with the high-power visible laser where we could see the actual laser beam and direct it on the plane but instead, we went with a more traditional sight where you look through the sight and point the reticle at the plane. I went with two options one a more simplistic iron sight based on the MP5s iron sight. The second option was a simple red dot based on the EOTech HWS EXP S3, which would use an LED instead of mirrors and lasers


![alt_text](images/image8.png "image_tooltip")
.
![alt_text](images/image9.png "image_tooltip")



### Versions and merges


![alt_text](images/image10.png "image_tooltip")



## Design style


### Design for 3d Printing

Almost all my projects contain a large amount of 3D printed parts and over the years I have Learne different ways to enhance 3D printed parts quality strength and ease of use. This is all done by adapting to the method that the 3D printer uses to create its parts, layering plastic.One specific way I  adapt to 3D printing is to avoid overhangs whenever possible. Once our example can be found within the trigger grip where without specific design choices like teardrop nuttraps there would be much more support.

Caption: Sliced trigger grip

Another example can be found within the Red Dot sight where a lot of the parts are printed individually to allow for easier printing and less support material.


![alt_text](images/image11.png "image_tooltip")

![alt_text](images/image12.png "image_tooltip")



### Hidden complexity

Another design element is hidden complexity although some elements of the design may seem simple there is a lot of thought in each element of the design for example each joint provides multiple points of stabilization for the potentiometer so instead  don't receiving all of its structural support from one part prone damage it supported by multiple parts both increasing the accuracy (by reducing wiggle) and longevity of the design.


![alt_text](images/image13.png "image_tooltip")

![alt_text](images/image14.png "image_tooltip")


Caption:These are two of the joints 


### Complex wire pathing


## Construction


### 3D printer settings 


### Wiring and soldering 


### Construction difficulties 


## Reflection 


![alt_text](images/image15.png "image_tooltip")

