#include <Arduino.h>
#include "mishapprotocol.h"
#include "config.h"
#include <SPI.h>
#include <SD.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>

//  logging globals
File logFile;
String filename;
SPIClass* hspi;

// radio globals
RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, PAYLOAD_LORA_ADDR);
uint8_t send_buf[RH_RF95_MAX_MESSAGE_LEN];

// state globals
Vector3d currentLocation;
Vector3d targetLocation;
Vector3d currentVelocity;
Vector3d impactLoc;
float theta;
float phi;
float altitude;
bool isTrackGood;
bool isArmed;
bool didDrop;
unsigned long lastPacketReceivedTime;
unsigned long lastLocationUpdateTime;


bool handlePacket(MishapProtocolPacket packet){
    // returns true if we need to update position
  Serial.print("Packet is of type ");
  Serial.println(packet.packetType);
  lastPacketReceivedTime = millis();
  if(packet.packetType == PacketType::LaserAngle){
    LaserAngleData newLad = decodeMPP<LaserAngleData>(packet);
    Serial.print("Theta: ");
    Serial.print(newLad.theta);
    Serial.print(" Phi: ");
    Serial.print(newLad.phi);
    Serial.print(" isTrackGood: ");
    Serial.print(newLad.isTrackGood);
    Serial.print(" isArmed: ");
    Serial.println(newLad.isArmed);
    theta = newLad.theta;
    phi = newLad.phi;
    isTrackGood = newLad.isTrackGood;
    isArmed = newLad.isArmed;
    return true;
  } else if(packet.packetType == PacketType::TargetSettings){
    TargetSettingsData newTSD = decodeMPP<TargetSettingsData>(packet);
    Serial.print("Target X: ");
    Serial.print(newTSD.targetLoc.x);
    Serial.print(" Target Y: ");
    Serial.print(newTSD.targetLoc.y);
    Serial.print(" Target Z: ");
    Serial.println(newTSD.targetLoc.z);
    targetLocation = newTSD.targetLoc;
  } else if(packet.packetType == PacketType::ClearedCache){
    Serial.println("cleared cache data");
    Serial.println("Should not have just received cleared cache data");
  } else {
    Serial.print("Received unrecognized packet with id ");
    Serial.println(packet.packetType);
  }
  return false;
}


void initSdLogging()
{
    // use HSPI for SD card
    hspi = new SPIClass(HSPI);
    hspi->begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI, SD_CS);
    pinMode(SD_CS, OUTPUT);
    if (!SD.begin(SD_CS, *hspi))
    {
        Serial.println("SD initialization failed!");
        while (1);
    }

    // figure out which mission number we are on
    // filename format is mission_XXX.csv
    int missionNumber = 0;
    File root = SD.open("/");
    File entry = root.openNextFile();
    while (entry)
    {
        String filename = entry.name();
        if (filename.startsWith("mission_"))
        {
            int number = filename.substring(8, 11).toInt();
            if (number > missionNumber)
            {
                missionNumber = number;
            }
        }
        entry = root.openNextFile();
    }
    root.close();
    missionNumber++; // increment to the next mission number
    String missionNoString = String(missionNumber);
    // pad with zeros
    while (missionNoString.length() < 3)
    {
        missionNoString = "0" + missionNoString;
    }
    filename = "/mission_" + missionNoString + ".csv";
    Serial.print("Mission number is ");
    Serial.println(missionNoString);
    logFile = SD.open(filename, FILE_WRITE, true);
    if (!logFile)
    {
        Serial.println("Failed to open log file");
        while (1);
    }
    Serial.print("Opened log file ");
    Serial.println(filename);
    logFile.println("Time,Payload Location X,Payload Location Y,Payload Location Z,Payload Velocity X,Payload Velocity Y,Payload Velocity Z,Target Location X,Target Location Y,Target Location Z,Impact Location X,Impact Location Y,Impact Location Z,Is Track Good,Is Armed,Did Drop,Theta,Phi,Last Packet Received Time,Time Since Last Location Update");
    logFile.close();
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

bool recieveAndHandlePacket(){
    bool result = false;
    if(manager.available()){
        unsigned long start = millis();
      //Serial.println("Received a message in the radio loop");
      // Wait for a message addressed to us from the client
      uint8_t len = sizeof(recv_buf);
      uint8_t from;
      if (manager.recvfrom(recv_buf, &len, &from)){
        // Serial.print("Got packet from ");
        // Serial.println(from);
        result = handlePacket(rawBufToMPP(recv_buf, len));
      }
        unsigned long end = millis();
        Serial.print("Time to receive and handle packet: ");
        Serial.println(end - start);
    }
    return result;
}

void logStateToSd(){
    logFile = SD.open(filename, FILE_WRITE);
    logFile.seek(logFile.size());
    logFile.print(millis());
    logFile.print(",");
    logFile.print(currentLocation.toString());
    logFile.print(",");
    logFile.print(currentVelocity.toString());
    logFile.print(",");
    logFile.print(targetLocation.toString());
    logFile.print(",");
    logFile.print(impactLoc.toString());
    logFile.print(",");
    logFile.print(isTrackGood);
    logFile.print(",");
    logFile.print(isArmed);
    logFile.print(",");
    logFile.print(didDrop);
    logFile.print(",");
    logFile.print(theta);
    logFile.print(",");
    logFile.print(phi);
    logFile.print(",");
    logFile.print(lastPacketReceivedTime);
    logFile.print(",");
    logFile.print(millis() - lastLocationUpdateTime);
    logFile.println();
    logFile.flush();
    logFile.close();

}

void readAltitudeSensor(){
    altitude = 3;
}

void updateLocation(){
    Vector3d lastLocation = currentLocation;
    currentLocation = getPayloadLocation(altitude, theta, phi);
    currentVelocity = getVelocity(lastLocation, currentLocation, millis() - lastLocationUpdateTime);
    Vector3d impactLoc = getImpactLocation(currentLocation, currentVelocity, targetLocation);
    Serial.print("Current location: ");
    Serial.println(currentLocation.toString());
    Serial.print("Current velocity: ");
    Serial.println(currentVelocity.toString());
    Serial.print("Impact location: ");
    Serial.println(impactLoc.toString());

    lastLocationUpdateTime = millis();
}

void dropPayload(){

}

void sendTelemetry(){

}

void payloadsetup()
{
    Serial.begin(115200);
    Serial.println("Payload controller starting up");
    
    initRadio(driver, manager);

    initSdLogging();
}

void payloadloop() {
    if(recieveAndHandlePacket()){
        readAltitudeSensor();
        updateLocation();
        if(shouldDrop(impactLoc, targetLocation, isTrackGood, isArmed)){
            dropPayload();
        }
        sendTelemetry();
    }
    logStateToSd();
    if(Serial.available()){
        Serial.println("Serial input detected");
        String input = Serial.readString();
        if(input.startsWith("stop")){
            Serial.println("Stopping");
            SD.end();
            while(1);
        }
    }
}