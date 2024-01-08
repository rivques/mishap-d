#include <Arduino.h>
#include "mishapprotocol.h"

void payloadsetup(){

}

void payloadloop(){

}

Vector3d getImpactLocation(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc){
   float timeOfFlight = getTimeOfFlight(payloadLoc, payloadVel, targetLoc);
   Vector3d distance = getDistance(payloadLoc, payloadVel, targetLoc, timeOfFlight);
}

float getTimeOfFlight(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc){
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

Vector3d getDistance(Vector3d payloadLoc, Vector3d payloadVel, Vector3d targetLoc, float timeOfFlight){
    Vector3d distance;
    distance.x = payloadVel.x * timeOfFlight;
    distance.y = payloadVel.y * timeOfFlight;
    distance.z = payloadLoc.z - targetLoc.z;
    return distance;
}