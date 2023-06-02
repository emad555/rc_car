#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte CE_PIN = 8;
const byte CSN_PIN = 10;

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

struct LdrValues
{
  int ldr_1;
  int ldr_2; 
}ldrValues;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 50; // send 20 times per second

const byte LDR1 = A1;
const byte LDR2 = A4;

void setup()
{
   Serial.begin(9600);
   Serial.println("SimpleTx Starting");
   pinMode(LDR1, INPUT_PULLUP);
   pinMode(LDR2, INPUT_PULLUP);
   
   radio.begin();
   radio.setChannel(76);  //76 library default
   //RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
   radio.setPALevel(RF24_PA_HIGH);
   radio.setDataRate( RF24_250KBPS );
   radio.setRetries(3, 5); // delay, count
   radio.openWritingPipe(slaveAddress);
}

void loop()
{
   currentMillis = millis();
   if (currentMillis - prevMillis >= txIntervalMillis)
   {
      send();
      Serial.print("LDR 1 = ");
      Serial.print(ldrValues.ldr_1);
      Serial.print("    LDR 2 = ");
      Serial.println(ldrValues.ldr_2);
      prevMillis = millis();
   }
}

//====================

void send()
{
   ldrValues.ldr_1 = analogRead(LDR1);
   ldrValues.ldr_2 = analogRead(LDR2);
   radio.write( &ldrValues, sizeof(ldrValues) );
}