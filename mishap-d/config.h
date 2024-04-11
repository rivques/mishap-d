#pragma once

// for Reasons, this is how I'm developing two different programs with a shared library
// uncomment the one we're doing
//#define TARGETING_GROUND
//#define TARGETING_PAYLOAD
#define TARGETING_PAYLOADONLY

#if (defined(TARGETING_GROUND) && defined(TARGETING_PAYLOAD)) || (defined(TARGETING_GROUND) && defined(TARGETING_PAYLOADONLY)) || (defined(TARGETING_PAYLOAD) && defined(TARGETING_PAYLOADONLY))
#error More than one target is defined. Pick one please.
#endif
#if !defined(TARGETING_GROUND) && !defined(TARGETING_PAYLOAD) && !defined(TARGETING_PAYLOADONLY)
#error No targets are defined. Pick one please.
#endif

// configuration of globals
#define STARTING_GROUND_STATION_LOCATION Vector3d{0, 0, 1} // meters
#define WIND_ACCELERATION Vector3d{0, 0, 0} // m/s^2
#define ACCEPTABLE_TARGET_ERROR 1 // meters

// specific defs, like pins and stuff
#define GROUND_LORA_ADDR 1
#define PAYLOAD_LORA_ADDR 2

// radio pins and config
#define RFM95_CS    4
#define RFM95_INT   2
#define RFM95_RST   16
#define RF95_FREQ 915.0

#if defined(TARGETING_PAYLOAD) || defined(TARGETING_PAYLOADONLY)

// SD card pins
#define SD_CS 15
#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_CLK   14

#define PAYLOAD_BAY_SERVO_PIN 27
#define PAYLOAD_BAY_SERVO_OPEN 0
#define PAYLOAD_BAY_SERVO_CLOSED 30

#define DEBUG_LED_PIN 1
#endif

#ifdef TARGETING_GROUND
#define OLED_RESET 27
#define OLED_I2C_ADDR 0x3D
#define OLED_TEXT_SIZE 1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define PIN_BACK_BUTTON 5 
#define PIN_ENCODER_CLK 35
#define PIN_ENCODER_DT 33
#define PIN_ENCODER_SW 32
#endif
