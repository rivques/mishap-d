#include <Arduino.h>
#include "mishapprotocol.h"
#include "config.h"
#include <SPI.h>
#include <SD.h>

File logFile;
String filename;

void handlePacket(MishapProtocolPacket packet){
  Serial.print("Packet is of type ");
  Serial.println(packet.packetType);
  if(packet.packetType == PacketType::LaserAngle){
    LaserAngleData newLad = decodeMPP<LaserAngleData>(packet);
    Serial.print("Theta: ");
    Serial.print(newLad.theta);
    Serial.print(" Phi: ");
    Serial.println(newLad.phi);
    logFile = SD.open(filename, FILE_WRITE);
    logFile.seek(logFile.size());
    logFile.print(millis());
    logFile.print(",");
    logFile.print(newLad.theta);
    logFile.print(",");
    logFile.println(newLad.phi);
    logFile.close();
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
  Serial.println();
}

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, PAYLOAD_LORA_ADDR);
QueueHandle_t receiveQueue;
QueueHandle_t sendQueue;
uint8_t send_buf[RH_RF95_MAX_MESSAGE_LEN];
TaskHandle_t radioTask;
RadioLoopParams radioParams = {manager, receiveQueue, sendQueue};
SPIClass* hspi;

void payloadsetup() {
    Serial.begin(115200);
    initRadio(driver, manager);
    Serial.println("Payload setup");

    // use HSPI for SD card
    hspi = new SPIClass(HSPI);
    hspi->begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI, SD_CS);
    pinMode(SD_CS, OUTPUT);
    if (!SD.begin(SD_CS, *hspi)) {
        Serial.println("SD initialization failed!");
        while(1);
    }

    // figure out which mission number we are on
    // filename format is mission_XXX.log
    int missionNumber = 0;
    File root = SD.open("/");
    File entry = root.openNextFile();
    while (entry) {
        String filename = entry.name();
        if (filename.startsWith("mission_")) {
            int number = filename.substring(8, 11).toInt();
            if (number > missionNumber) {
                missionNumber = number;
            }
        }
        entry = root.openNextFile();
    }
    root.close();
    missionNumber++; // increment to the next mission number
    String missionNoString = String(missionNumber);
    // pad with zeros
    while (missionNoString.length() < 3) {
        missionNoString = "0" + missionNoString;
    }
    filename = "/mission_" + missionNoString + ".log";
    Serial.print("Mission number is ");
    Serial.println(missionNoString);
    logFile = SD.open(filename, FILE_WRITE, true);
    if(!logFile){
        Serial.println("Failed to open log file");
        while(1);
    }
    Serial.print("Opened log file ");
    Serial.println(filename);
    logFile.println("Time,Theta,Phi");
    logFile.close();

    // receiveQueue = xQueueCreate(4, sizeof(recv_buf));
    // sendQueue = xQueueCreate(4, sizeof(send_buf));

    // xTaskCreatePinnedToCore(
    //   doRadioLoop, /* Function to implement the task */
    //   "RadioTask", /* Name of the task */
    //   10000,  /* Stack size in words */
    //   reinterpret_cast<void*>(&radioParams),  /* Task input parameter */
    //   0,  /* Priority of the task */
    //   &radioTask,  /* Task handle. */
    //   0); /* Core where the task should run */
    //   Serial.println("Created radio task");
}

void payloadloop() {
    // watch the queue for incoming messages
    // if(xQueueReceive(receiveQueue, &maintask_recv_buf, 0) == pdPASS){
    //     Serial.println("Received a message via queue");
    //     handlePacket(rawBufToMPP(maintask_recv_buf, sizeof(maintask_recv_buf)));
    // }
    if(manager.available()){
        unsigned long start = millis();
      //Serial.println("Received a message in the radio loop");
      // Wait for a message addressed to us from the client
      uint8_t len = sizeof(recv_buf);
      uint8_t from;
      if (manager.recvfrom(recv_buf, &len, &from)){
        // Serial.print("Got packet from ");
        // Serial.println(from);
        handlePacket(rawBufToMPP(recv_buf, len));
      }
        unsigned long end = millis();
        Serial.print("Time to receive and handle packet: ");
        Serial.println(end - start);
    }
}


float getTimeOfFlight(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc) {
    float a = -9.81;//accelaeeratiion
    float displacement = payloadLoc.z - targetLoc.z;
    float t1 = (-payloadVel.z + sqrt(payloadVel.z * payloadVel.z - 2 * a * displacement))/a;
    float t2 = (-payloadVel.z - sqrt(payloadVel.z * payloadVel.z - 2 * a * displacement))/a;
    if (t1 > t2)
    {
        Serial.println("use the plus sign");
        return t1;
    }
    else
    {
        Serial.println("use subtraction");
        return t2;
    }
}

Vector3d getDistance(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc, Vector3d windRes, float timeOfFlight) {
    Vector3d distance;
    distance.x = payloadVel.x * timeOfFlight + 0.5 * windRes.x * timeOfFlight * timeOfFlight; //1D kinematics equations to figure out location on 1D plane
    distance.y = payloadVel.y * timeOfFlight + 0.5 * windRes.y * timeOfFlight * timeOfFlight;
    distance.z = targetLoc.z - payloadLoc.z;
    return distance;
}

Vector3d getImpactLocation(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc) {
   float timeOfFlight = getTimeOfFlight(payloadLoc, payloadVel, targetLoc);
   Vector3d windRes = WIND_ACCELERATION; // acceleration caused by wind
   Vector3d distance = getDistance(payloadLoc, payloadVel, targetLoc, windRes, timeOfFlight);
   return payloadLoc + distance;
}


float deg2rad(float degrees) {
    float rad = (degrees * PI)/180;
    return rad;
}
Vector3d getPayloadLocation(float altitude, float theta, float phi) {
    float radius = altitude/tan(deg2rad(phi));
    Vector3d location;
    location.x = radius * cos(deg2rad(theta));
    location.y = radius * sin(deg2rad(theta));
    location.z = altitude;
    return location;
}

Vector3d getVelocity(Vector3d lastLocation, Vector3d currentLocation, unsigned long timeBetween) {
    Vector3d velocity = (currentLocation-lastLocation)/(timeBetween*1000);
    return velocity;
}

bool shouldDrop(Vector3d impactLoc, Vector3d targetLoc, bool isTrackGood, bool isArmed) {
    if (isArmed == true && isTrackGood == true && (impactLoc - targetLoc).magnitude() <= ACCEPTABLE_TARGET_ERROR) {
        return true;
    }
    else {
        return false;
    }
}

// void logDataToSd(Vector3d payloadLoc, Vector3d payloadVel, Vector3d impactLoc, Vector3d targetLoc, bool isTrackGood, bool isArmed, bool didDrop, float altitude, float theta, float phi) {
//     SDFile myFile = SD.open("data.txt", FILE_WRITE);
//         // if the file opened okay, write to it:
//     if (myFile) {
//         // TODO: refactor to be nicer
//         myFile.print(payloadLoc.toString()); myFile.print(",");
//         myFile.print(payloadVel.toString()); myFile.print(",");
//         myFile.print(targetLoc.toString()); myFile.print(",");
//         myFile.print(isTrackGood); myFile.print(",");
//         myFile.print(isArmed); myFile.print(",");
//         myFile.print(didDrop); myFile.print(",");
//         myFile.print(altitude); myFile.print(",");
//         myFile.print(theta); myFile.print(",");
//         myFile.println(phi);
//         // close the file:
//         myFile.close();
//         Serial.println("done.");
//     } 
//     else {
//         // if the file didn't open, print an error:
//         Serial.println("error opening test.txt");
//     }
// }