// for Reasons, this is how I'm developing two different programs with a shared library
// uncomment the one we're doing
// #define TARGETING_GROUND
#define TARGETING_PAYLOAD

#if defined(TARGETING_GROUND) && defined(TARGETING_PAYLOAD)
#error Both TARGETING_GROUND and TARGETING_PAYLOAD are defined. Pick one please.
#endif
#if !defined(TARGETING_GROUND) && !defined(TARGETING_PAYLOAD)
#error Neither TARGETING_GROUND nor TARGETING_PAYLOAD are defined. Pick one please.
#endif

#ifdef TARGETING_GROUND
#include "groundcontroller.h"
void setup(){
    groundsetup();
}
void loop(){
    groundloop();
}
#endif

#ifdef TARGETING_PAYLOAD
#include "payloadcontroller.h"
void setup(){
    payloadsetup();
}
void loop(){
    payloadloop();
}
#endif