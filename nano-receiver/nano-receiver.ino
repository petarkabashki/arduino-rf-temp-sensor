// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

#define pinRcv 9
#define pinTx 10
//RH_ASK driver;
 RH_ASK driver(2400, pinRcv, pinTx, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

// 
// Nokia LCD display 5510 

#include <PCD8544.h>
//uint8_t sclk  = 3,   /* clock       (display pin 2) */
//uint8_t sdin  = 4,   /* data-in     (display pin 3) */
//uint8_t dc    = 5,   /* data select (display pin 4) */
//uint8_t reset = 6,   /* reset       (display pin 8) */
//uint8_t sce   = 7);  /* enable      (display pin 5) */


static PCD8544 lcd;

void setup()
{
  //pinMode(8, pinRcv);
  
#ifdef RH_HAVE_SERIAL
    Serial.begin(9600);	  // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
	;
#endif
  

  lcd.begin(84, 48);
}

const int lcdLineLen = 15;

char textbuf[7][lcdLineLen];

void addLine(char* line, int buflen) {   
  memset(textbuf[6], 0, lcdLineLen);
  memcpy(textbuf[6], line, buflen);

//Serial.println("----");
  for(int i = 0; i<6; i++) {
    memset(textbuf[i], 0, lcdLineLen);
    memcpy(textbuf[i], textbuf[1+i], lcdLineLen);
//    Serial.println(textbuf[i]);
    lcd.setCursor(0, i);
    lcd.clearLine();
    //char* lcdLine = textbuf[i];
    lcd.print(textbuf[i]);
  }
}
void loop()
{
    uint8_t buf[20];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      addLine((char*)buf, buflen);
    }
}
