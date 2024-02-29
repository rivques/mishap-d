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

#ifdef TARGETING_PAYLOAD
#include "payloadcontroller.h"
void setup(){
    payloadsetup();
}
void loop(){
    payloadloop();
}
#endif