// output the serial monitor over uart2
// and pulse D23 low when there's a new line

void setup() {
  Serial.begin(115200);
  pinMode(23, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial2.begin(115200);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial2.write(c);
    if (c == '\n') {
      digitalWrite(23, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(23, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}