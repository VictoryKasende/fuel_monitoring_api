#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "WINS"
#define WIFI_PASSWORD "WINNER20"

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define UPDATE_INTERVAL_MS 1000  // 1 second

// Tank Configuration
#define NUM_TANKS 3
#define LEDS_PER_TANK 3

// Tank Level Thresholds (percentage)
#define LOW_LEVEL_THRESHOLD 20
#define MEDIUM_LEVEL_THRESHOLD 70
#define HIGH_LEVEL_THRESHOLD 100

// Pin Definitions
// Water Sensors (ADC pins ESP32-S3)
#define SENSOR_TANK_1 1   // GPIO1 (ADC1_CH0)
#define SENSOR_TANK_2 2   // GPIO2 (ADC1_CH1)  
#define SENSOR_TANK_3 3   // GPIO3 (ADC1_CH2)

// LED Pins (GPIO différents des capteurs)
// Tank 1 LEDs
#define LED_TANK_1_RED 10
#define LED_TANK_1_YELLOW 11
#define LED_TANK_1_GREEN 12

// Tank 2 LEDs
#define LED_TANK_2_RED 13
#define LED_TANK_2_YELLOW 14
#define LED_TANK_2_GREEN 15

// Tank 3 LEDs
#define LED_TANK_3_RED 16
#define LED_TANK_3_YELLOW 17
#define LED_TANK_3_GREEN 18

// Sensor Calibration Values
#define SENSOR_MIN_VALUE 0      // Sensor value when tank is empty
#define SENSOR_MAX_VALUE 4095   // Sensor value when tank is full (for 12-bit ADC ESP32-S3)

// API Endpoints
#define API_STATUS_ENDPOINT "/api/status"
#define API_TANK_ENDPOINT "/api/tank/"
#define API_ALL_TANKS_ENDPOINT "/api/tanks"

#endif // CONFIG_H
