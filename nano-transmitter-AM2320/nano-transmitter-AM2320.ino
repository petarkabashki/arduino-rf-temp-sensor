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

// MQ2 sensor
#include "MQUnifiedsensor.h"
/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino UNO")
#define         mq2Pin                     (A2)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-2") //MQ2
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ2CleanAir        (9.83) //RS / R0 = 9.83 ppm 

/*****************************Globals***********************************************/
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, mq2Pin, Type);
/*****************************Globals***********************************************/



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

//MQ-2
 //Set math model to calculate the PPM concentration and the value of constants
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(574.25); MQ2.setB(-2.222); // Configurate the ecuation values to get LPG concentration
  /*
    Exponential regression:
    Gas    | a      | b
    H2     | 987.99 | -2.162
    LPG    | 574.25 | -2.222
    CO     | 36974  | -3.109
    Alcohol| 3616.1 | -2.675
    Propane| 658.71 | -2.168
  */

  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ2.init(); 
  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ2.setRL(10);
  */
  /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
  // In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
  // and now is on clean air (Calibration conditions), and it will setup R0 value.
  // We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
  // This routine not need to execute to every restart, you can load your R0 if you know the value
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ2.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 

//  MQ2.serialDebug(true);
//\MQ-2
}

const char ro[] = "R02";
//const char *errmsg = sprintf("%s:ERROR", ro);

void readAndSend() {
  
  char msg[20];
  memset(msg, 0, 20);
  sprintf(msg, "%s:T:%02d", ro, (int)am2320.readTemperature());
  
  Serial.println("Sending:");
  Serial.println(msg);
  
  driver.send((uint8_t *)msg, 8);
  driver.waitPacketSent();
  delay(100);
  memset(msg, 0, 20);
  sprintf(msg, "%s:H:%02d", ro, (int)am2320.readHumidity());
  
  Serial.println(msg);
  
  driver.send((uint8_t *)msg, 8);
  driver.waitPacketSent();

  memset(msg, 0, 20);
  MQ2.update(); // Update data, the arduino will be read the voltage on the analog pin
  float mq2ppm = MQ2.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  //delay(5);
  sprintf(msg, "%s:MQ2:%04d", ro, (int)(100*mq2ppm));
  Serial.println(msg);
  //Serial.println(mq2ppm);
  
  driver.send((uint8_t *)msg, 17);
  driver.waitPacketSent();
}

void loop() {
  // Delay between measurements.
  delay(1500);
  readAndSend();
}
