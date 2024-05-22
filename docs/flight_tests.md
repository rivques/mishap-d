# Test 1: April 22nd
## Media

https://github.com/rivques/mishap-d/assets/38469076/50176521-65ab-4e1a-8dff-e52d6706bea4

## Description
Our first test went fairly well, but had two critical issues. First of all, we hadn't considered how to land with an open payload bay door, so we
had to attempt a kind of skid landing that fortunately went successfully. Second, and more importantly, our datalogging failed, so we coudn't see anything
from how the test went. These data issues ultimately led to us using a Raspberry Pi Pico for logging instead of an SD card.
# Test 2: May 15th
## Media
### Takeoff
https://github.com/rivques/mishap-d/assets/38469076/751fbe20-d896-425d-ab39-77b227352c7a
### Payload release
https://github.com/rivques/mishap-d/assets/38469076/0a5dc9f4-8793-4ebd-a090-f2772520797e
### Fall
https://github.com/rivques/mishap-d/assets/38469076/972d8c86-7016-41f2-878d-53066bbe97b3
### Aftermath
![A photo of a crashed Phantom 4 with our project attached](https://github.com/rivques/mishap-d/assets/91289762/ffcf9fe4-5b3d-42f9-80c6-c5cbfefe46e5)
## Description and Data
Our second test was dramatic. Because we'd added landing legs to avoid another skid landing, our center of mass was far from where the drone's control
algorithms were expecting it. This resulted in unstable oscillations on the way up that led to complete loss of control on the descent, likely including
a full flip of the drone. Fortunately, the landing legs happened to be pointing down when the drone hit the ground and were able to take the brunt of the
damage, functioning much like a crumple zone. The only damage the drone sustained was the loss of a few small body panels that were easily reatached.
Most importantly, we got data from this flight! Below is a chart of our altitude (blue), our door status (red), and our velocity (yellow).

![MISHAP-D Flight Test 2](https://github.com/rivques/mishap-d/assets/38469076/ea0f5666-39ea-4ef4-af89-881e11699b6d)

We can see that we barely hit 50 meters (our set drop height), where we dropped our payload (a small paper helicopter), before intitating a descent peaking at almost 8 m/s leading to impact, where our battery was disconnected and our logging stopped.
