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

#ifdef TARGETING_PAYLOAD
#endif

#ifdef TARGETING_GROUND
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13
#endif
