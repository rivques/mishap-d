#include "config.h"

#ifdef TARGETING_GROUND
#include "groundcontroller.h"
void setup(){
    groundsetup();
}
void loop(){
    groundloop();
}
#endif

#if defined(TARGETING_PAYLOAD) || defined(TARGETING_PAYLOADONLY)
#include "payloadcontroller.h"
void setup(){
    payloadsetup();
}
void loop(){
    payloadloop();
}
#endif