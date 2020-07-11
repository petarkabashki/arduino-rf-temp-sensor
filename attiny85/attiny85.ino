
#include <SoftwareSerial.h>


#include <TinyDHT.h>        // lightweit DHT sensor library

#define DHTTYPE DHT11   // DHT 11 

#define TEMPTYPE 0        // Use Fahrenheit (0 for celsius)
#define DHTPIN 2

// ***
// *** Define the RX and TX pins. Choose any two
// *** pins that are unused. Try to avoid D0 (pin 5)
// *** and D2 (pin 7) if you plan to use I2C.
// ***
#define RX    3   // *** D3, Pin 2
#define TX    4   // *** D4, Pin 3

// ***
// *** Define the software based serial port. Using the
// *** name Serial so that code can be used on other
// *** platforms that support hardware based serial. On
// *** chips that support the hardware serial, just
// *** comment this line.
// ***
SoftwareSerial Serial(RX, TX);

DHT dht(DHTPIN, DHTTYPE);  // Define Temp Sensor


// ***
// *** Pinout ATtiny25/45/85:
// *** PDIP/SOIC/TSSOP
// *** =============================================================================================
// ***
// ***        (PCINT5/RESET/ADC0/dW) PB5   [1]*  [8]   VCC
// *** (PCINT3/XTAL1/CLKI/OC1B/ADC3) PB3   [2]   [7]   PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
// *** (PCINT4/XTAL2/CLKO/OC1B/ADC2) PB4   [3]   [6]   PB1 (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
// ***                               GND   [4]   [5]   PB0 (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0)
// ***

// *****************************************
// ***
// *** For FTDI/232 Cable:
// *** https://www.adafruit.com/products/70
// ***
// *** Connect 3V  (red)    to Pin 8
// *** Connect GND (black)  to Pin 4
// *** Connect RX  (yellow) to Pin 3
// *** Connect TX  (orange) to Pin 2
// ***
// *****************************************

// *****************************************
// ***
// *** For Serial TTL Cable:
// *** https://www.adafruit.com/products/954
// ***
// *** Connect 3V  (red)   to Pin 8
// *** Connect GND (black) to Pin 4
// *** Connect RX  (white) to Pin 3
// *** Connect TX  (green) to Pin 2
// ***
// *****************************************



void setup()
{
  pinMode(DHTPIN, INPUT);
   dht.begin();  // Initialize DHT Teperature Sensor
   
  // ***
  // *** Initialize the Serial port
  // ***
  Serial.begin(9600);
  Serial.println("Temperature IC Control Library Initializing...");


  // ***
  // *** Show a message that setup has completed.
  // ***
  Serial.println("Initialization complete.");
}

void loop()
{
   int8_t h = dht.readHumidity();               // Read humidity
  int16_t t = dht.readTemperature(TEMPTYPE);   // read temperature
  
  Serial.print("Temperature = ");
  Serial.println(t);
  Serial.print("Humidity = ");
  Serial.println(h);
  
  delay(2000);

}
