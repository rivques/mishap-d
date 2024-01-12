#include <Arduino.h>
#include "mishapprotocol.h"
#include "config.h"
#include <SPI.h>
#include <SD.h>

void payloadsetup() {

}

void payloadloop() {

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

void logDataToSd(Vector3d payloadLoc, Vector3d payloadVel, Vector3d impactLoc, Vector3d targetLoc, bool isTrackGood, bool isArmed, bool didDrop, float altitude, float theta, float phi) {
    SDFile myFile = SD.open("data.txt", FILE_WRITE);
        // if the file opened okay, write to it:
    if (myFile) {
        // TODO: refactor to be nicer
        myFile.print(payloadLoc.toString()); myFile.print(",");
        myFile.print(payloadVel.toString()); myFile.print(",");
        myFile.print(targetLoc.toString()); myFile.print(",");
        myFile.print(isTrackGood); myFile.print(",");
        myFile.print(isArmed); myFile.print(",");
        myFile.print(didDrop); myFile.print(",");
        myFile.print(altitude); myFile.print(",");
        myFile.print(theta); myFile.print(",");
        myFile.println(phi);
        // close the file:
        myFile.close();
        Serial.println("done.");
    } 
    else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
}