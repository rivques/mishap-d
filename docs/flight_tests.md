# Test 1: April 22nd
**INSERT VIDEO HERE**
Our first test went fairly well, but had two critical issues. First of all, we hadn't considered how to land with an open payload bay door, so we
had to attempt a kind of skid landing that fortunately went successfully. Second, and more importantly, our datalogging failed, so we coudn't see anything
from how the test went. These data issues ultimately led to us using a Raspberry Pi Pico for logging instead of an SD card.
# Test 2: May 15th
**INSERT TAKEOFF HERE**
**INSERT FALL HERE**
Our second test was dramatic. Because we'd added landing legs to avoid another skid landing, our center of mass was far from where the drone's control
algorithms were expecting it. This resulted in unstable oscillations on the way up that led to complete loss of control on the descent, likely including
a full flip of the drone. Fortunately, the landing legs happened to be pointing down when the drone hit the ground and were able to take the brunt of the
damage, functioning much like a crumple zone. The only damage the drone sustained was the loss of a few small body panels that were easily reatached.
Most importantly, we got data from this flight! Below is a chart of our altitude (blue), our door status (red), and our velocity (yellow).
**INSERT CHART HERE**
We can see that we barely hit 50 meters (our set drop height), where we dropped our payload (a small paper helicopter), before intitating a descent peaking at almost 8 m/s leading to impact, where our battery was disconnected and our logging stopped.