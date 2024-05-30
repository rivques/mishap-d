#include "config.h"

#ifdef TARGETING_GROUND
#include "groundcontroller.h"
void setup(){ // run the ground controller if that's what we're building for
    groundsetup();
}
void loop(){
    groundloop();
}
#endif

#if defined(TARGETING_PAYLOAD) || defined(TARGETING_PAYLOADONLY)
#include "payloadcontroller.h" // run the payload controller if that's what we're building for
void setup(){
    payloadsetup();
}
void loop(){
    payloadloop();
}
#endif