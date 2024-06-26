#include <Arduino.h>
#include "mishapprotocol.h"
#include "config.h"
#include <SPI.h>
#include <Servo.h>
#include <SD.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <stdint.h>
#include <Wire.h>
//#include "LIDARLite_v4LED.h" // this is a tweaked version of github.com/garmin/LIDARLite_Arduino_Library
// with LEGACY_I2C essentially forced defined
#include <Adafruit_MPL3115A2.h>
//LIDARLite_v4LED myLidarLite;

Adafruit_MPL3115A2 baro;

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
float impactToTarget;
bool isTrackGood;
bool isArmed;
bool didDrop;
unsigned long lastPacketReceivedTime;
unsigned long lastLocationUpdateTime;

float groundAltitude = 0;

Servo payloadBayServo;


void ledDebugProgramStart(){
    #ifdef TARGETING_PAYLOADONLY
    // fade in and out 
    for(int i = 0; i < 256; i++){
        analogWrite(DEBUG_LED_PIN, i);
        delay(2);
    }
    for (int i = 255; i >= 0; i--){
        analogWrite(DEBUG_LED_PIN, i);
        delay(2);
    }
    #endif
    #ifdef TARGETING_PAYLOAD
    // 3 quick fades
    for(int j = 0; j < 3; j++){
        for(int i = 0; i < 256; i++){
            analogWrite(DEBUG_LED_PIN, i);
            delay(1);
        }
        for (int i = 255; i >= 0; i--){
            analogWrite(DEBUG_LED_PIN, i);
            delay(1);
        }
    }
    #endif
}

void ledDebugSetupComplete(){
    // 2 quick fades
    for(int j = 0; j < 2; j++){
        for(int i = 0; i < 256; i++){
            analogWrite(DEBUG_LED_PIN, i);
            delay(1);
        }
        for (int i = 255; i >= 0; i--){
            analogWrite(DEBUG_LED_PIN, i);
            delay(1);
        }
    }
}

void ledDebugMissionNumber(int missionNumber){
    delay(2000);
    pinMode(DEBUG_LED_PIN, OUTPUT);
    Serial.print("LED Debugging mission number ");
    Serial.println(missionNumber);
    
    // blink number of tens quickly, then quad quick fade, then blink number of ones quickly
    for(int i = 0; i < missionNumber/10; i++){
        //analogWrite(DEBUG_LED_PIN, 200);
        digitalWrite(DEBUG_LED_PIN, HIGH);
        delay(200);
        //analogWrite(DEBUG_LED_PIN, 10);
        digitalWrite(DEBUG_LED_PIN, LOW);
        delay(100);
    }
    for(int j = 0; j < 4; j++){
        for(int i = 0; i < 256; i++){
            analogWrite(DEBUG_LED_PIN, i);
            delay(1);
        }
        for (int i = 255; i >= 0; i--){
            analogWrite(DEBUG_LED_PIN, i);
            delay(1);
        }
    }
    for(int i = 0; i < missionNumber%10; i++){
        analogWrite(DEBUG_LED_PIN, 255);
        delay(100);
        analogWrite(DEBUG_LED_PIN, 0);
        delay(200);
    }
}

void ledDebugLogError(){
    for(int i = 0; i < 3; i++){
        analogWrite(DEBUG_LED_PIN, 255);
        delay(500);
        analogWrite(DEBUG_LED_PIN, 0);
        delay(500);
    }
}

void ledDebugDidLog(){
    //1 quick blink (skipped because sd act led works)
    analogWrite(DEBUG_LED_PIN, 255);
    delay(25);
    analogWrite(DEBUG_LED_PIN, 0);

}

void ledDebugProgramEnded(){
    // infinite long fades
    for(int i = 0; i < 256; i++){
        analogWrite(DEBUG_LED_PIN, i);
        delay(2);
    }
}

bool handlePacket(MishapProtocolPacket packet){ // called when we get a packet from the ground station
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


void initSdLogging() // just sets up the serial port
{
    Serial2.begin(115200, SERIAL_8N1, -1, 17);

    Serial2.println("Time,Payload Location X,Payload Location Y,Payload Location Z,Payload Velocity X,Payload Velocity Y,Payload Velocity Z,Target Location X,Target Location Y,Target Location Z,Impact Location X,Impact Location Y,Impact Location Z,Is Track Good,Is Armed,Did Drop,Theta,Phi,Last Packet Received Time,Time Since Last Location Update");
}

float getTimeOfFlight(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc) { // figures out how long it would take the payload to hit the ground
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

Vector3d getDistance(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc, Vector3d windRes, float timeOfFlight) {// figures out how far the payload would travel
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
Vector3d getPayloadLocation(float altitude, float theta, float phi) {  // calculates where the payload is from the laser angles
    float radius = altitude/tan(deg2rad(phi));
    Vector3d location;
    location.x = radius * cos(deg2rad(theta));
    location.y = radius * sin(deg2rad(theta));
    location.z = altitude;
    return location;
}

Vector3d getVelocity(Vector3d lastLocation, Vector3d currentLocation, unsigned long timeBetween) { // difference in location over time
    Vector3d velocity = (currentLocation-lastLocation)/(static_cast<float>(timeBetween)/1000.0f);
    return velocity;
}

bool shouldDrop(Vector3d impactLoc, Vector3d targetLoc, bool isTrackGood, bool isArmed) { // figures out if the payload should drop
    if (isArmed == true && isTrackGood == true && impactToTarget <= ACCEPTABLE_TARGET_ERROR) {
        return true;
    }
    else {
        return false;
    }
}

bool recieveAndHandlePacket(){ // checks if we have a packet and if we do, handles it
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

void setupLidar(){
    Serial.begin(115200);

    if (!baro.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while(1);
  }

  // use to set sea level pressure for current location
  // this is needed for accurate altitude measurement
  // STD SLP = 1013.26 hPa
  baro.setSeaPressure(1013.26);
  delay(2000);
  groundAltitude = baro.getAltitude();
}

void tryGetLidarDistance(){ // we're using an altimeter so this isn't as hard as it was
    // Check on busyFlag to indicate if device is idle
    // (meaning = it finished the previously triggered measurement)
    //Serial.println("trying for new altitude...");
    // if (myLidarLite.getBusyFlag() == 0)
    // {
    //     //Serial.println("distanceContinuous: not busy");
    //     // Trigger the next range measurement
    //     myLidarLite.takeRange();

    //     // Read new distance data from device registers
    //     uint16_t altitude_cm = myLidarLite.readDistance();
    //     altitude = static_cast<float>(altitude_cm) / 100.0f;
    //     Serial.print("New altitude: ");
    //     Serial.println(altitude);
    // }
    altitude = baro.getAltitude() - groundAltitude;
    Serial.print("New altitude: ");
    Serial.println(altitude);
}

void saveAndEndIfNeeded(){ // stop the program when the boot button is pressed
    if(digitalRead(0) == LOW){
        Serial.println("Stopping");
        SD.end();
        while(1){
            ledDebugProgramEnded();
        }
    }
}

#ifdef TARGETING_PAYLOAD
void logStateToSd(){
    ledDebugDidLog();
    Serial2.print(millis());
    Serial2.print(",");
    Serial2.print(currentLocation.toString());
    Serial2.print(",");
    Serial2.print(currentVelocity.toString());
    Serial2.print(",");
    Serial2.print(targetLocation.toString());
    Serial2.print(",");
    Serial2.print(impactLoc.toString());
    Serial2.print(",");
    Serial2.print(isTrackGood);
    Serial2.print(",");
    Serial2.print(isArmed);
    Serial2.print(",");
    Serial2.print(didDrop);
    Serial2.print(",");
    Serial2.print(theta);
    Serial2.print(",");
    Serial2.print(phi);
    Serial2.print(",");
    Serial2.print(lastPacketReceivedTime);
    Serial2.print(",");
    Serial2.print(millis() - lastLocationUpdateTime);
    Serial2.println();
    Serial2.flush();

}
#endif

#ifdef TARGETING_PAYLOADONLY // without radio we only have altitude
void logStateToSd(){ // only log altitude and drop status
    ledDebugDidLog();
    Serial2.print(millis());
    Serial2.print(",");
    Serial2.print(altitude);
    Serial2.print(",");
    Serial2.print(didDrop);
    Serial2.println();
}
#endif

void updateLocation(){ // redo math when sensors change
    Vector3d lastLocation = currentLocation;
    currentLocation = getPayloadLocation(altitude, theta, phi);
    currentVelocity = getVelocity(lastLocation, currentLocation, millis() - lastLocationUpdateTime);
    Vector3d impactLoc = getImpactLocation(currentLocation, currentVelocity, targetLocation);
    Serial.print("Current location: ");
    Serial.println(currentLocation.toString());
    Serial.print("Last location: ");
    Serial.println(lastLocation.toString());
    Serial.print("Current velocity: ");
    Serial.println(currentVelocity.toString());
    Serial.print("Impact location: ");
    Serial.println(impactLoc.toString());
    
    impactToTarget = (impactLoc - currentLocation).magnitude();

    lastLocationUpdateTime = millis();
}

void dropPayload(){
    payloadBayServo.write(PAYLOAD_BAY_SERVO_OPEN);
    didDrop = true;
}

void sendTelemetry(){
    PayloadTelemetryData telem = PayloadTelemetryData{
        millis(),
        currentLocation,
        currentVelocity,
        impactLoc,
        didDrop
    };
    uint8_t send_buf[RH_RF95_MAX_MESSAGE_LEN];
    constructRawDataPacket(telem, PacketType::PayloadTelemetry, send_buf);
    manager.sendto(send_buf, sizeof(send_buf), GROUND_LORA_ADDR);
}



void payloadsetup()
{   
    pinMode(DEBUG_LED_PIN, OUTPUT);
    pinMode(0, INPUT_PULLUP);
    ledDebugProgramStart();
    Serial.begin(115200);
    #ifdef TARGETING_PAYLOAD
        Serial.println("Payload controller starting up with radio");
    #endif
    #ifdef TARGETING_PAYLOADONLY
        Serial.println("Payload controller starting up without radio");
    #endif
    payloadBayServo.attach(PAYLOAD_BAY_SERVO_PIN);
    payloadBayServo.write(PAYLOAD_BAY_SERVO_CLOSED);
    #ifdef TARGETING_PAYLOAD
        initRadio(driver, manager);
    #endif
    Serial.println("Initializing Lidar...");
    setupLidar();
    Serial.println("Initializing logging...");
    initSdLogging();
    ledDebugSetupComplete();
    Serial.println("Payload controller setup complete");
}

#ifdef TARGETING_PAYLOAD
void payloadloop() {
    if(recieveAndHandlePacket()){
        tryGetLidarDistance();
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
    saveAndEndIfNeeded();
}
#endif

#ifdef TARGETING_PAYLOADONLY
void payloadloop(){
    tryGetLidarDistance();
    if(altitude > PAYLOADONLY_DROP_ALTITUDE){
        dropPayload();
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
    saveAndEndIfNeeded();
}
#endif