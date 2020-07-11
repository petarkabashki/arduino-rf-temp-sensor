
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//RH_ASK driver;
 RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

#define DHTPIN 2     // Digital pin connected to the DHT sensor 

#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  
#ifdef RH_HAVE_SERIAL

#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
  ;
#endif

  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

const char *ro = "R01";
const char *errmsg = sprintf("%s:ERROR", ro);

void loop() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    driver.send((uint8_t *)errmsg, strlen(errmsg));
    driver.waitPacketSent();
  }
  else {
    char msg[10];
    sprintf(msg, "%s:T:%02d", ro, (int)event.temperature);
    
    Serial.println("Sending:");
    Serial.println(msg);
    
    driver.send((uint8_t *)msg, 8);
    driver.waitPacketSent();
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    driver.send((uint8_t *)errmsg, strlen(errmsg));
    driver.waitPacketSent();
  }
  else {
    char msg[10];
    sprintf(msg, "%s:H:%02d", ro, (int)event.relative_humidity);
    
    Serial.println("Sending:");
    Serial.println(msg);
    
    driver.send((uint8_t *)msg, 8);
    driver.waitPacketSent();
  }
}
