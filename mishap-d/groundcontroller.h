#include <Arduino.h>
#include "mishapprotocol.h"
#include "config.h"
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_SSD1306.h> // Hardware-specific library for ST7789
// Adafruit_SSD1306 oleddisplay(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
RH_RF95 driver(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(driver, GROUND_LORA_ADDR);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool needSendTargetPos = false;

// display state
int numMenu = 8; // number of items in the menu
int menuItem = 1;
int curArrow;
int lastArrow;
int curDecimal;
int lastDecimal;
int pixelLength = 5 * OLED_TEXT_SIZE;
int encTics2Switch = 3; // for the menu, how many times do you want to move the enc before it switches spots
float disNum;
int sigFig = 3;
bool backButtonState = false;
float displayedNumber = 0;
bool pressed;
int curMenuSelection = 1;
float magnitude;
float curNum;
bool magChose = false;
bool pmag;
bool cmag;
float pastDisNum;
int scaledEncoderPosition;
int currentState;
int initState;
unsigned long bebounceDelay = 0;

void handlePacket(MishapProtocolPacket packet){
  Serial.print("Packet is of type ");
  Serial.println(packet.packetType);
  if(packet.packetType == PacketType::LaserAngle){
    LaserAngleData newLad = decodeMPP<LaserAngleData>(packet);
    Serial.print("Theta: ");
    Serial.print(newLad.theta);
    Serial.print(" Phi: ");
    Serial.println(newLad.phi);
  }
  else if(packet.packetType == PacketType::TargetSettings){
    TargetSettingsData newTSD = decodeMPP<TargetSettingsData>(packet);
    Serial.print("Target X: ");
    Serial.print(newTSD.targetLoc.x);
    Serial.print(" Target Y: ");
    Serial.print(newTSD.targetLoc.y);
    Serial.print(" Target Z: ");
    Serial.println(newTSD.targetLoc.z);
  }
  else if(packet.packetType == PacketType::ClearedCache){
    Serial.println("cleared cache data");
  }
  else{
    Serial.print("Received unrecognized packet with id ");
    Serial.println(packet.packetType);
  }
}

int counterToMenu(int tics, int items){
  int menu = scaledEncoderPosition / tics;
  return menu;
}

void showPayloadStatus(){ // menu item display
  display.clearDisplay();
  display.print("stuf");
}

void menuItem2(){ // menu item display
}

void txtSetup(){
  display.setTextSize(OLED_TEXT_SIZE); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font
}

void mainMenuSetup(){
  // menu Items
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F(" Payload Status")); // prints menu items in order vertically
  display.println(F(" Set Target X"));
  display.println(F(" Set Target Y"));
  display.println(F(" Set Target Z"));
  display.println(F(" MenuItem#5"));
  display.println(F(" MenuItem#6"));
  display.println(F(" MenuItem#7"));
  display.println(F(" MenuItem#8"));
}

void encSetup(){
  pinMode(PIN_ENCODER_CLK, INPUT); // setting inputs for the encoder
  pinMode(PIN_ENCODER_DT, INPUT);
  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);
  initState = digitalRead(PIN_ENCODER_CLK);
  // attachInterrupt(0, updateEncoderValue, CHANGE);
  // attachInterrupt(1, updateEncoderValue, CHANGE);
  // attachPCINT(digitalPinToPCINT(PIN_ENCODER_SW), buttonPress, CHANGE);
}

bool encoderIsPressed(){
  int buttonVal = digitalRead(PIN_ENCODER_SW); // turning raw data to int
  // If we detect LOW signal, button is pressed
  if(buttonVal == LOW){
    if(millis() - bebounceDelay > 200){ // debounce
      // Serial.println("Button pressed!");
    }
    long debounceDelay = millis();
  }
  return !buttonVal;
}

void updateEncoderValue(){
  /*
    Tics is the amount of times you want the encoder to "click" before it switches menu items.
    This is used because the encoder doesn get read perfectly so a coushin is added to increase accuracy of menu selection.

    Items represents the amount of items in the selected menu.
    The more items the longer you want to be able to scroll before the selection arrow goes back to the top.
  */
  // Read the current state of PIN_ENCODER_CLK
  currentState = digitalRead(PIN_ENCODER_CLK);
  // If last and current state of PIN_ENCODER_CLK are different, then we can be sure that the pulse occurred
  if(currentState != initState && currentState == 1){
    // Encoder is rotating counterclockwise so we decrement the scaledEncoderPosition
    if(digitalRead(PIN_ENCODER_DT) != currentState){
      scaledEncoderPosition++;
    }
    else{
      // Encoder is rotating clockwise so we increment the scaledEncoderPosition
      scaledEncoderPosition--;
    }
    // print the value in the serial monitor window
  }
  // Remember last PIN_ENCODER_CLK state for next cycle
  initState = currentState;
  if(scaledEncoderPosition >= numMenu * encTics2Switch){
    scaledEncoderPosition = 2; // reset scaledEncoderPosition when it hits the "bottom" of the menu
  }
  if(scaledEncoderPosition <= 1){
    scaledEncoderPosition = (numMenu * encTics2Switch) - 1;
  }
}

int getMenuSelectionFromEncVal(int enc){
  encTics2Switch = 3; // presets, fully customizable when calling updateEncoderValue();
  numMenu = 8;        // items in menu

  curArrow = 8 * counterToMenu(encTics2Switch, numMenu); // location of arrow

  display.setCursor(1, curArrow);
  display.print(F(">"));
  ;
  if(lastArrow != curArrow){

    int bottomOfPixel = lastArrow + 6;

    for(int i = lastArrow; i <= bottomOfPixel; i++){ // turns all pixels in a 5x6 to black, effectivly deleting the arrow
      for(int ii = 1; ii <= pixelLength; ii++){                                          //
        display.drawPixel(ii, i, SSD1306_BLACK); //
      };
    }
    lastArrow = curArrow;
  }
  int menuItem = curArrow / 8;
  return menuItem;
}

void chooseNumber(){
  display.clearDisplay();
  cmag = magChose;

  if(magChose == false){
    numMenu = 10; // items in menu
    encTics2Switch = 2;
    curDecimal = scaledEncoderPosition / 2;
    magnitude = map(counterToMenu(encTics2Switch, numMenu), 1, 10, -5, 5); // maps values from 1-10 to -5 - 5, this will be the exponent that 10 is to the power to. This decides the magnitude of the number.
    if(magnitude > 0){
      disNum = round(pow(10, magnitude)); // rounds number because some math is funky and the rounding the computer does makes the number bad. so i round it to the magnitude in digits.
    }
    else{
      disNum = pow(10, magnitude);
    }
  }
  else if(magChose == true){
    disNum = counterToMenu(encTics2Switch, numMenu) * pow(10, magnitude); // encoder value * 10^magnitude returned from last function = the printed value
  }
  if(encoderIsPressed() == true){
    magChose = !magChose;
    while(encoderIsPressed() == true){
    }
  }
  Serial.println(pastDisNum);
  if(pmag == 1 && cmag == 0){
    Serial.println("happeneing");
    displayedNumber = displayedNumber + pastDisNum;
  }
  pmag = cmag;
  pastDisNum = disNum;
}

void displaySetup(){
  pinMode(PIN_BACK_BUTTON, INPUT_PULLUP);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //;
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  encSetup();

  txtSetup();

  Serial.println("done setup");
}

void selectNumberToVariable(float* target, String name, bool* onchanged = NULL){ // takes a number from the user and sets it to a variable
  chooseNumber();
  display.setCursor(0, 0);
  display.print("Choose ");
  display.print(name);
  display.setCursor(0, 8);
  display.print(disNum, 5); // display number
  display.setCursor(0, 16);
  display.print("Final ");
  display.print(name);
  display.setCursor(0, 24);
  display.print(displayedNumber, 5); // display number
  if(backButtonState == true){
    // copy the number to the target
    *target = displayedNumber;
    curMenuSelection = 1;
    if(onchanged != NULL){
      *onchanged = true;
    }
  }
}
float targetX = 0;
float targetY = 0;
float targetZ = 0;

void doDisplay(){  // the display loop
  display.display();
  unsigned long loopStart = millis();
  updateEncoderValue();
  backButtonState = !digitalRead(PIN_BACK_BUTTON); // active low
  if(curMenuSelection == 1){ // main menu selection screen
    displayedNumber = 0;
    mainMenuSetup();
    int menuSelection = getMenuSelectionFromEncVal(scaledEncoderPosition);
    if(encoderIsPressed() == true){
      curMenuSelection = menuSelection + 2;
      display.clearDisplay();
      while(encoderIsPressed() == true){
      }
    }
  }
  else if(curMenuSelection == 2){
    showPayloadStatus();
  }
  else if(curMenuSelection == 3){
    selectNumberToVariable(&targetX, "Target X", &needSendTargetPos);
  }
  else if(curMenuSelection == 4){
    selectNumberToVariable(&targetY, "Target Y", &needSendTargetPos);
  }
  else if (curMenuSelection == 5){
    selectNumberToVariable(&targetZ, "Target Z", &needSendTargetPos);
  }
  Serial.print("Loop took ");
  Serial.println(millis() - loopStart);
}

void groundsetup(){
  Serial.begin(115200);
  initRadio(driver, manager);
  Serial.println("setting up display");
  displaySetup();
  Serial.println("done setting up display");
  // LaserAngleData lad = LaserAngleData{1.5, 3.1415};
  // uint8_t test_buf[RH_RF95_MAX_MESSAGE_LEN];
  // constructRawDataPacket(lad, PacketType::LaserAngle, test_buf);

  // Serial.print("Raw data: ");
  // for (int x = 0; x < 9; x++)
  // {
  //    Serial.print(test_buf[x], BIN);
  //    Serial.print(" ");
  // }
  // Serial.println();

  // MishapProtocolPacket packet = rawBufToMPP(test_buf, sizeof(test_buf));
  // handlePacket(packet);
  // uint8_t another_buf[RH_RF95_MAX_MESSAGE_LEN];
  // TargetSettingsData tsd = TargetSettingsData{Vector3d{2.43, 2, -12.32}};
  // constructRawDataPacket(tsd, PacketType::TargetSettings, another_buf);
  // handlePacket(rawBufToMPP(another_buf, sizeof(another_buf)));
}

long lastSend = 0;

void doLaserAngles(){ // figure out laser data and send it to payload
  if(millis() - lastSend > 1000){
    LaserAngleData lad = LaserAngleData{ millis() / 1000.0, hallRead(), false, false };
    uint8_t send_buf[RH_RF95_MAX_MESSAGE_LEN];
    constructRawDataPacket(lad, PacketType::LaserAngle, send_buf);
    Serial.print("Sending lad packet: ");
    Serial.print(lad.theta);
    Serial.print(", ");
    Serial.println(lad.phi);
    Serial.print("First bytes of packet: ");
    for(int i = 0; i < 16; i++){
      Serial.print(send_buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    unsigned long start = millis();
    manager.sendto(send_buf, sizeof(send_buf), PAYLOAD_LORA_ADDR);
    Serial.print("Sent packet in ");
    Serial.println(millis() - start);
    lastSend = millis();
  }
}

void groundloop(){ // main loop
  doDisplay();
  // send a packet every second with the current time and the esp32's built-in hall effect sensor
  // use laserangledata for testing
  doLaserAngles();
  if(needSendTargetPos){
    TargetSettingsData tsd = TargetSettingsData{Vector3d{targetX, targetY, targetZ}};
    uint8_t send_buf[RH_RF95_MAX_MESSAGE_LEN];
    constructRawDataPacket(tsd, PacketType::TargetSettings, send_buf);
    manager.sendto(send_buf, sizeof(send_buf), PAYLOAD_LORA_ADDR);
    needSendTargetPos = false;
  }
}