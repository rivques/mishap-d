// this file mainly holds the shared LoRa protocol
// also a few other shared structs

#include <Arduino.h>
#include "config.h"
#include <RHReliableDatagram.h>
#include <RH_RF95.h>

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

  String toString(unsigned int decimalPlaces){
    return String(x, decimalPlaces) + ", " + String(y, decimalPlaces) + ", " + String(z, decimalPlaces);
  }

  String toString(){
    return toString(2);
  }
}; // 12 bytes

enum PacketType : uint8_t {
  LaserAngle=0,
  TargetSettings=1,
  ClearedCache=2,
  PayloadTelemetry=3
  // etc.
};

#define PACKET_DATA_LENGTH 250

struct MishapProtocolPacket {
  PacketType packetType;
  byte data[PACKET_DATA_LENGTH];
};

// packet structs
struct LaserAngleData {
  float theta;
  float phi;
  bool isTrackGood;
  bool isArmed;
}; // 10 bytes

struct TargetSettingsData {
  Vector3d targetLoc;
}; // 12 bytes

struct ClearedCacheData {}; // 0 bytes

struct PayloadTelemetryData {
  unsigned long time; // time in milliseconds since payload power-on
  Vector3d payloadPos;
  Vector3d payloadVel;
  Vector3d payloadImpactPos;
  bool payload_dropped;
}; // 45 bytes

void initRadio(RH_RF95 driver, RHReliableDatagram manager){
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) delay(1);
  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  while (!manager.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!driver.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  driver.setTxPower(23, false);
}

uint8_t recv_buf[RH_RF95_MAX_MESSAGE_LEN];

struct RadioLoopParams {
  RHReliableDatagram manager;
  QueueHandle_t sendQueue;
  QueueHandle_t receiveQueue;
};

void doRadioLoop(void* params){
  RadioLoopParams* p = reinterpret_cast<RadioLoopParams*>(params);
  RHReliableDatagram manager = p->manager;
  QueueHandle_t sendQueue = p->sendQueue;
  QueueHandle_t receiveQueue = p->receiveQueue;
  while(1){
    if(manager.available()){
      //Serial.println("Received a message in the radio loop");
      // Wait for a message addressed to us from the client
      uint8_t len = sizeof(recv_buf);
      uint8_t from;
      if (manager.recvfromAck(recv_buf, &len, &from)){
        // Serial.print("Got packet from ");
        // Serial.println(from);
        xQueueSend(receiveQueue, (void*) &recv_buf, 0);
      }
    }
  }
}

MishapProtocolPacket rawBufToMPP(uint8_t* recv_buf, size_t len){
  MishapProtocolPacket result;
  memcpy(&result, recv_buf, len);
  return result;
}

template <typename T>
T decodeMPP(MishapProtocolPacket packet){
  // INVARIANTS: T MUST match packet.packetType
  T result;
  memcpy(&result, packet.data, sizeof(T));
  return result;
}

template <typename T>
void constructRawDataPacket(T dataStruct, PacketType dataType, uint8_t* output){
  // INVARIANTS: dataStruct's type MUST match dataType, and output MUST be of size RH_RF95_MAX_MESSAGE_LEN
  MishapProtocolPacket mpp;
  mpp.packetType = dataType;
  memcpy(mpp.data, &dataStruct, sizeof(T));
  memcpy(output, &mpp, RH_RF95_MAX_MESSAGE_LEN);
}
