#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//RH_ASK driver;
 RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

Adafruit_AM2320 am2320 = Adafruit_AM2320();

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
  am2320.begin();
}

const char *ro = "R02";
const char *errmsg = sprintf("%s:ERROR", ro);

void readAndSend() {
  
  char msg[10];
  sprintf(msg, "%s:T:%02d", ro, (int)am2320.readTemperature());
  
  Serial.println("Sending:");
  Serial.println(msg);
  
  driver.send((uint8_t *)msg, 8);
  driver.waitPacketSent();
  delay(100);
  sprintf(msg, "%s:H:%02d", ro, (int)am2320.readHumidity());
  
  Serial.println("Sending:");
  Serial.println(msg);
  
  driver.send((uint8_t *)msg, 8);
  driver.waitPacketSent();
}

void loop() {
  // Delay between measurements.
  delay(15000);
  readAndSend();
}
