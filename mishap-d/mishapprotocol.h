#include <Arduino.h>

struct ThreadedLogMsg {
  int level;
  String msg;
};

enum PacketType : char {
  LaserAngle,
  TargetSettings,
  ClearedCache,
  PayloadTelemetry
  // etc.
};

struct MishapProtocolPacket {
  PacketType packetType;
  char data[250];
};