// this file mainly holds the shared LoRa protocol
// also a few other shared structs

#include <Arduino.h>

struct ThreadedLogMsg {
  int level;
  String msg;
};

struct Vector3d {
  float x;
  float y;
  float z;

  Vector3d operator+(Vector3d other){
    return Vector3d{this->x + other.x, this->y + other.y, this->z + other.z};
  }

  Vector3d operator-(Vector3d other){
    return Vector3d{this->x - other.x, this->y - other.y, this->z - other.z};
  }

  Vector3d operator*(float scalar){
    return Vector3d{x*scalar, y*scalar, z*scalar};
  }

  Vector3d operator/(float scalar){
    return operator*(1.0F/scalar);
  }

  float magnitude(){
    return sqrt(x*x+y*y+z*z);
  }
}; // 12 bytes

enum PacketType : byte {
  LaserAngle,
  TargetSettings,
  ClearedCache,
  PayloadTelemetry
  // etc.
};

struct MishapProtocolPacket {
  PacketType packetType;
  byte data[250];
};

// packet structs
struct LaserAngleData {
  float theta;
  float phi;
}; // 8 bytes

struct TargetSettingsData {
  Vector3d targetLoc;
}; // 12 bytes

struct ClearedCacheData {}; // 0 bytes

struct PayloadTelemetryData {
  unsigned long time; // time in milliseconds since payload power-on
  float altitude;
  Vector3d payloadPos;
  Vector3d payloadVel;
  Vector3d payloadImpactPos;
  bool payload_dropped;
}; // 45 bytes