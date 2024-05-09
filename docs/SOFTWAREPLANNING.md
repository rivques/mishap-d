# Logging
Uses the [ArduinoLog](https://github.com/thijse/Arduino-Log) library for easy logging.

I don't think this is thread safe, so the radio task logs via a queue of these structs:
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
enum PacketType : byte {
  LaserAngle,
  TargetSettings,
  ClearedCache,
  PayloadTelemetry
  // etc.
}
struct MishapProtocolPacket {
  PacketType packetType;
  byte data[250];
}
```
`PacketType` indicates what struct is packed into the `data` buffer. Upon receiving a packet, the receiver can know what type is in `data` and thus what type to unpack it into with a `memcpy`. 

## Packet types
### LaserAngle
GC->PC, indicates what angle the laser is pointing at
```cpp
struct LaserAngleData {
  float theta;
  float phi;
} // 8 bytes
```
### TargetSettings
GC->PC, tells the payload where the target is relative to the ground station
```cpp
struct TargetSettingsData {
  float target_x;
  float target_y;
  float target_z;
} // 12 bytes
```
### ClearedCache
PC->GC, asks the GC for the target settings (for example, because the payload has just powered on)
```cpp
struct ClearedCacheData {} // 0 bytes
```
### PayloadTelemetry
PC->GC, a bundle of all the telemetry data
```cpp
struct PayloadTelemetryData {
  unsigned long time; // time in milliseconds since plane power-on
  float altitude;
  Vector3d payloadLoc;
  Vector3d payloadVel;
  Vector3d impactLoc;
  bool payload_dropped;
} // 45 bytes
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
3. Updates its sensor readings.
4. Calculates its current position from its internal state. `Vector3d getPayloadLocation(float altitude, float theta, float phi)`
5. Calculates its current velocity from its internal state. `Vector3d getVelocity(Vector3d lastLocation, Vector3d currentLocation, unsigned long timeBetween)`
6. Does a physics simulation of dropping the payload. `Vector3d getImpactLocation(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc)`
7. Checks if the payload should be dropped (based on impact location, target location, track validity, arm state, etc.) `bool shouldDrop(Vector3d impactLoc, Vector3d targetLoc, bool isTrackGood, bool isArmed)`
8. Logs data to nonvolatile storage. `void logDataToSd(Vector3d payloadLoc, Vector3d payloadVel, Vector3d impactLoc, Vector3d targetLoc, bool isTrackGood, bool isArmed, bool didDrop, float altitude, float theta, float phi)` (and anything else we might want to log)
9. Sends data out as a telemetry packet.
# Coordinate system
The origin's XY is at the tracking station, and the Z is the ground directly below the tracking station. The +X axis is directly forward from the tracking station. The +Y axis is directly to the right from the tracking station.