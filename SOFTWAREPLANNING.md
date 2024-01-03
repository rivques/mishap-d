# Protocol
Uses Radiohead's RHReliableDatagrams and an RFM9x LoRa radio. This gives us 251-byte packets to send back and forth. There's a class, `MishapProtocol`, that is `#include`d by both the ground controller and the payload controller, so they always have the same definitions.

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
`void doRadioLoop(bool isPayload, bool* packetShouldSend, bool* packetWasRecieved, MishapProtocolPacket* packetToSend, MishapProtocolPacket* packetReceived)`
Kicked off on a thread by FreeRTOS, runs the loop. Communication happens via those variables.
# Ground controller software structure
# Payload controller software structure
