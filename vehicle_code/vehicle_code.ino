
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte CE_PIN = 8;
const byte CSN_PIN = 10;

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN);

struct RecValues
{
   int throttle;
   int steer;
} RecValues;

bool newData = false;

//===========

int fSpeed = 250;
int bSpeed = 250;


void forward(int f){
        digitalWrite(2,1);
        digitalWrite(3,0);
        analogWrite(5,f);


        digitalWrite(4,0);
        digitalWrite(7,1);
        analogWrite(6,f);
}

void backward(int b){
        digitalWrite(2,0);
        digitalWrite(3,1);
        analogWrite(5,b);


        digitalWrite(4,1);
        digitalWrite(7,0);
        analogWrite(6,b);
}

void terminate(){
          digitalWrite(2,0);
        digitalWrite(3,0);
        analogWrite(5,0);


        digitalWrite(4,0);
        digitalWrite(7,0);
        analogWrite(6,0);
}

void left(){
        digitalWrite(2,1);
        digitalWrite(3,0);
        analogWrite(5,fSpeed);


        digitalWrite(4,0);
        digitalWrite(7,1);
        analogWrite(6,150);
}

void right(){
        digitalWrite(2,1);
        digitalWrite(3,0);
        analogWrite(5,150);


        digitalWrite(4,0);
        digitalWrite(7,1);
        analogWrite(6,fSpeed);
}

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

   pinMode(2,OUTPUT);
   pinMode(3,OUTPUT);
   pinMode(5,OUTPUT);

   pinMode(4,OUTPUT);
   pinMode(7,OUTPUT);
   pinMode(6,OUTPUT);

   
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
      radio.read( &RecValues, sizeof(RecValues) );
      newData = true;

//drive();
  

        int f = map(RecValues.throttle, 600,1023,50,250);
        f = constrain(f,50,250); 
//        Serial.print("Forward values: ");
//        Serial.println(f);
        
        int b = map(RecValues.throttle, 400,20,50,250);
        b = constrain(b,50,250);    
//        Serial.print("back values: ");
//        Serial.println(b);

      if(RecValues.throttle == 0 )
      {
        RecValues.throttle = 1023;
      }

       if(RecValues.throttle > 600 )
      {
      
     forward(f);
     delay(20);
      }

      if(RecValues.throttle < 400)
      {
 backward(b);
      delay(20);

      }
      
      if(RecValues.throttle > 400 and RecValues.throttle < 600)
      {
//terminate();
      }


      if(RecValues.steer > 900)
      {
     right();
           delay(20);

      }

      if(RecValues.steer < 30)
      {
   left();
         delay(20);

      }
      
      if(RecValues.steer < 900 and RecValues.throttle > 30)
      {
  terminate();
      }l


     
   }
}

   
void showData()
{
   if (newData == true)
   {
      
      Serial.print("Data received >> ");
      Serial.print("Throttle = ");
      Serial.print(RecValues.throttle);
      Serial.print("Steer = ");
      Serial.println(RecValues.steer);
      newData = false;  
   }   
}

//void drive(){
//  if(RecValues.throttle > 600){
// int throttle = map(RecValues.throttle, 600,1023,50,255);
// int steer = map(RecValues.steer, 0,1023,-100,100); //100 is just a number that controls the steepness of the turns
//  int rightM = throttle - steer;
//  int leftM = throttle + steer;
//
//        digitalWrite(2,1);
//        digitalWrite(3,0);
//        analogWrite(5,0);
//
//
//        digitalWrite(4,0);
//        digitalWrite(7,1);
//        analogWrite(6,0);
//  
//  }
//
//  else if (RecValues.throttle < 400){
//
//int  throttle = map(RecValues.throttle, 400,0,50,255);
//int   steer = map(RecValues.steer, 0,1023,-100,100); //100 is just a number that controls the steepness of the turns
//  int rightM = throttle - steer;
//  int leftM = throttle + steer;
//
//        digitalWrite(2,0);
//        digitalWrite(3,1);
//        analogWrite(5,0);
//
//
//        digitalWrite(4,1);
//        digitalWrite(7,0);
//        analogWrite(6,0);
//    }
//  }
