# Logging
Uses the [ArduinoLog](https://github.com/thijse/Arduino-Log) library for easy logging.

I don't think this is thread safe, so the radio task loggs via a queue of these structs:
```cpp
struct ThreadedLogMsg {
  int level;
  String msg;
}
```
# Protocol
Uses Radiohead's RHReliableDatagrams and an RFM9x LoRa radio. This gives us 251-byte packets to send back and forth. There's a class, `MishapProtocol`, that is `#include`d by both the ground controller and the payload controller, so they always have the same definitions. `MishapProtocol` contains the protocol's data structure, the function that runs on the second core to handle the radio, and a manager class to ease the job of the main core.

Each packet is a struct:
```cpp
enum PacketType {
  LaserAngle,
  TargetPosition,
  ClearedCache
  // etc.
}
struct MishapProtocolPacket {
  PacketType packetType;
  char data[247];
}
```
`PacketType` indicates what struct is packed into the `data` buffer. Upon receiving a packet, the receiver can know what type is in `data` and thus what type to unpack it into with a `memcpy`. 

## Packet types
### LaserAngle
GC->PC, indicates what angle the laser is pointing at
```cpp
struct LaserAngleData {
  double theta;
  double phi;
}
```
## MishapProtocol API
`void doRadioLoop(QueueHandle_t sendQueue, QueueHandle_t receiveQueue, QueueHandle_t loggingQueue)`
Kicked off on a thread by FreeRTOS, runs the loop that talks to the radio. Communication to the manager happens via the pointers passed in.
Parameters are [FreeRTOS queues](https://freertos.org/Embedded-RTOS-Queues.html). The first two are queues of `MishapProtocolPacket`s, the last is a queue of `ThreadedLogMsg`s.
These queues should be monitored by the main thread every loop, and the main loop should run roughly as fast as the radio loop so the queues don't overflow.
# Ground controller software structure
The controller first configures its hardware, then kicks off the radio loop in `setup()`.
In `loop()`, it:
1. Handles the receiving queues from the radio loop.
2. Updates any needed internal state from received packets.
3. Handles the user interface.
4. Sends packets as needed to tell the payload about changes in laser angle or settings.
5. Collects internal statistics loop frequency, etc. and logs them.
# Payload controller software structure
The controller first configures its hardware, then kicks off the radio loop in `setup()`.
In `loop()`, it:
1. Handles the receiving queues from the radio loop.
2. Updates any needed internal state from received packets.
3. 
