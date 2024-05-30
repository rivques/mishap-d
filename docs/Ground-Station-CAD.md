
# Ground station CAD


## Design requirements

For the drop to be accurate the ground station needed to measure the angle between the horizontal plane and the drone as well as the change in angle with the ground station while it moved horizontally. The station is also required to contain an LCD for a menu to interact with the code and a set of push buttons for the arm command as well as the usual power and LCD requirements.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image2.png" width="600">

Caption: This is a section view facing from the front split through the middle of the project.


## CAD Breakdown

The ground station was designed to track a moving plane and initiate a launch. The tracking is done to gather information about the plane that is going to be used to calculate the drop point for our projectile effectively guiding our plane.


### The Trigger grip

The handle is the most complex part of the Ground station and includes a functioning trigger LCD and an arm button. All these electronics fit into a tight handle that is detachable from the main assembly. The most difficult part other than the integration of the electronics was the surfacing of the handle to the top where the LCD and trigger were housed. 


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image18.png" width="600">

<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image8.png" width="600">


Caption: This is the surfacing done to seamlessly mesh the already-designed forward grip and the top of the grip where the LCD would be placed. This also creates a place for the trigger to be placed.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image4.png" width="600">


Caption: This is the button layout for the trigger grip


### Wire Pathing

Wire pathing on this project was one of the most complex parts of the CAD requiring rotating parts that are still connected by wires. This can either be done through a slip ring which I would have to buy or design myself, the latter option requiring at least a few weeks of work. Another way to path wires is to allow the wires to move freely within the joint although this restricts the movement of the joint. I chose the less complicated option of allowing the wires to move freely.



<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image16.png" width="600">


Caption: This is the general overview of how the wires were packed from the trigger grip to the main control computer at the base of the station.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image20.png" width="600">


Caption: This is one of the joints where one disc is rotating parallel to the other disc, the top disc has wires coming in from a hole in the plate that is not close to the center while the other stationary disk has a hole closer to the center but still not directly at it. This design allows the wires to be wrapped around the main connection point and give the design more than 180  degrees of rotation.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image6.png" width="600">

Caption: This is the second joint and is more simple than the first one where instead of the wires moving, a large hole has been placed for the part to move freely and not get stuck on the wires this restricts more of the movement of the rotation but it's also much simpler to construct.


### Sights 

To calculate the angle The ground station needed to be pointed at the plane originally this was going to be done with the high-power visible laser where we could see the actual laser beam and direct it on the plane but instead, we went with a more traditional sight where you look through the sight and point the reticle at the plane. I went with two options, one a more simplistic iron sight based on the MP5s iron sight. The second option was a simple red dot based on the EOTech HWS EXPS3, which would use an LED instead of mirrors and lasers


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image7.png" width="600">
.
<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image19.png" width="600">



### Versions and merges


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image10.png" width="600">


Versions and merges are also core elements of how this project was designed where versions and branches are used to test out different theories or ideas if they don't work you can move back to the main but if they do you can merge them this way multiple people can work on the document at the same time without damaging each other's work.


# Design style


### Design for 3d Printing

Almost all my projects contain a large amount of 3D printed parts and over the years I have learned different ways to enhance 3D printed parts' quality, strength, and ease of use. This is all done by adapting to the method that the 3D printer uses to create its parts, layering plastic.One specific way I  adapt to 3D printing is to avoid overhangs whenever possible once our example can be found within the trigger grip. Without specific design choices like teardrop nut traps, there would be much more support and hassle removing said support.

<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image3.png" width="600">

Caption: Sliced trigger grip

Another example can be found within the Red Dot sight where a lot of the parts are printed individually to allow for easier printing and less support material.

<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image1.png" width="600">

<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image7.png" width="600">



### Hidden complexity

Another design element is hidden complexity although some elements of the design may seem simple there is a lot of thought into each element of the design for example each joint provides multiple points of stabilization for the potentiometer so it doesn't receive all of its structural support from one part prone damage. Instead, it is supported by multiple parts both increasing the accuracy (by reducing wiggle) and longevity of the design.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image17.png" width="600">

<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image5.png" width="600">


Caption: These are two of the joints 


### Clean Cad

In the ground station section of the CAD document, there are no errors and almost all contexts have been fully updated this takes a lot of care and maintaining contexts and features is not easy.


## Construction


### 3D printer settings 


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image10.png" width="600">


These are the settings that I fine-tuned at home they allow me to print with a near-perfect detachment of support on flat structures and much-improved detachment on more complex curves.


### Wiring and soldering

For this project the soldering wasn't particularly difficult the issue was routing wires through the entire project which was a finicky and time-consuming process. In the end, though I achieved a clean route and used shrink wrap and connectors whenever possible to add support and help with ease of use.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image21.jpg" width="600">

<img src="https://github.com/rivques/mishap-d/blob/3f42e853a8b2bf39fef3c0bed54094c4a2ee6a2f/assets/image22.jpg" width="600">

<img src="https://github.com/rivques/mishap-d/blob/3f42e853a8b2bf39fef3c0bed54094c4a2ee6a2f/assets/image23.jpg" width="600">



### Construction difficulties 

There were some fit and finish problems within the design and one of the main issues was fitting the LCD snugly into the design. The main issue that I discovered was that the CAD model I had downloaded from Grabcad for reference seemed correct but was wrong in minor details that made my first mount unusable. Another issue that I had was removing support in really tight spaces where a Dremel was required.


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image11.png" width="600">


Another issue I had with the design was the Joint between the trigger grip and the tube that would get the y-plane rotation. This design was originally supposed to be two parts but kept snapping off this was a major issue as I would have to rewire the design every time it broke eventually I just stuck with keeping both parts printed together which originally I tried to avoid because it would end in a circle being printed on its side.


# Reflection 

The main issue with the project was that I took too long and didn't give the coders enough time to integrate the ground station with the rest of the project. The complexity of the design requirements required some elements of a complex design but I think I took it too far, some things could have been simplified and turned into an initial prototype that my teammates could have worked on for code giving them more time to integrate and still getting me time to add the complex features I wanted. For example, I could have routed the wires on the exterior of the project leaving them loose although this would have hurt the perfectionist within me it would have worked and saved me weeks of perfecting the complex routing of the wires. I also got carried away with a complex CAD of some of the parts the trigger grip is one of the most complex things I've ever designed and uses some pretty time-consuming CAD trickery and surfacing, just going with a stick would have been quicker and again I probably should have gone with a prototype before investing weeks into a complex but better design. 


<img src="https://github.com/rivques/mishap-d/blob/47df40ad20b2594f7d03712b8e78049119fa7873/assets/image15.png" width="600">

