#include <Arduino.h>
#include "mishapprotocol.h"
#include "config.h"
//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_SSD1306.h> // Hardware-specific library for ST7789
//Adafruit_SSD1306 oleddisplay(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


#include <RHReliableDatagram.h>
#include <RH_RF95.h>
RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, GROUND_LORA_ADDR);

void handlePacket(MishapProtocolPacket packet){
  Serial.print("Packet is of type ");
  Serial.println(packet.packetType);
  if(packet.packetType == PacketType::LaserAngle){
    LaserAngleData newLad = decodeMPP<LaserAngleData>(packet);
    Serial.print("Theta: ");
    Serial.print(newLad.theta);
    Serial.print(" Phi: ");
    Serial.println(newLad.phi);
  } else if(packet.packetType == PacketType::TargetSettings){
    TargetSettingsData newTSD = decodeMPP<TargetSettingsData>(packet);
    Serial.print("Target X: ");
    Serial.print(newTSD.targetLoc.x);
    Serial.print(" Target Y: ");
    Serial.print(newTSD.targetLoc.y);
    Serial.print(" Target Z: ");
    Serial.println(newTSD.targetLoc.z);
  } else if(packet.packetType == PacketType::ClearedCache){
    Serial.println("cleared cache data");
  } else {
    Serial.print("Received unrecognized packet with id ");
    Serial.println(packet.packetType);
  }
}


void groundsetup(){
  Serial.begin(115200);
  initRadio(driver, manager);
  // LaserAngleData lad = LaserAngleData{1.5, 3.1415};
  // uint8_t test_buf[RH_RF95_MAX_MESSAGE_LEN];
  // constructRawDataPacket(lad, PacketType::LaserAngle, test_buf);
  
  // Serial.print("Raw data: ");
  // for (int x = 0; x < 9; x++)
  // {
  //    Serial.print(test_buf[x], BIN);
  //    Serial.print(" ");
  // }
  // Serial.println();
  
  // MishapProtocolPacket packet = rawBufToMPP(test_buf, sizeof(test_buf));
  // handlePacket(packet);
  // uint8_t another_buf[RH_RF95_MAX_MESSAGE_LEN];
  // TargetSettingsData tsd = TargetSettingsData{Vector3d{2.43, 2, -12.32}};
  // constructRawDataPacket(tsd, PacketType::TargetSettings, another_buf);
  // handlePacket(rawBufToMPP(another_buf, sizeof(another_buf)));
}

void groundloop(){
  // send a packet every second with the current time and the esp32's built-in hall effect sensor
  // use laserangledata for testing

  LaserAngleData lad = LaserAngleData{millis()/1000.0, hallRead()};
  uint8_t send_buf[RH_RF95_MAX_MESSAGE_LEN];
  constructRawDataPacket(lad, PacketType::LaserAngle, send_buf);
  Serial.print("Sending lad packet: ");
  Serial.print(lad.theta);
  Serial.print(", ");
  Serial.println(lad.phi);
  Serial.print("First bytes of packet: ");
  for(int i = 0; i < 16; i++){
    Serial.print(send_buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  manager.sendtoWait(send_buf, sizeof(send_buf), PAYLOAD_LORA_ADDR);
  delay(1000);
}
