#include <Servo.h>

#define PAYLOAD_BAY_SERVO_PIN 27
#define PAYLOAD_BAY_SERVO_OPEN 180
#define PAYLOAD_BAY_SERVO_CLOSED 60

Servo payloadBayServo;

// open or close every 10 seconds
unsigned long lastServoChange = 0;
bool isOpen = false;

void setup() {
  payloadBayServo.attach(PAYLOAD_BAY_SERVO_PIN);
  payloadBayServo.write(PAYLOAD_BAY_SERVO_CLOSED);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (millis() - lastServoChange > 10000) {
    lastServoChange = millis();
    if (isOpen) {
      payloadBayServo.write(PAYLOAD_BAY_SERVO_CLOSED);
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
      payloadBayServo.write(PAYLOAD_BAY_SERVO_OPEN);
        digitalWrite(LED_BUILTIN, LOW);
    }
    isOpen = !isOpen;
  }
}