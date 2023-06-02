// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte CE_PIN = 8;
const byte CSN_PIN = 10;

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN);

struct LdrValues
{
   int ldr_1;
   int ldr_2;
} ldrValues;

bool newData = false;

//===========

void setup()
{
   Serial.begin(9600);

   Serial.println("SimpleRx Starting");

   radio.begin();
   radio.setChannel(76);  //76 library default
   //RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
   radio.setPALevel(RF24_PA_HIGH);
   radio.setDataRate( RF24_250KBPS );
   radio.openReadingPipe(1, thisSlaveAddress);
   radio.startListening();
}

//=============

void loop()
{
   getData();
   showData();
}

//==============

void getData()
{
   if ( radio.available() )
   {
      radio.read( &ldrValues, sizeof(ldrValues) );
      newData = true;
   }
}

void showData()
{
   if (newData == true)
   {
      Serial.print("Data received >> ");
      Serial.print("LDR 1 = ");
      Serial.print(ldrValues.ldr_1);
      Serial.print("    LDR 2 = ");
      Serial.println(ldrValues.ldr_2);
      newData = false;
   }
}