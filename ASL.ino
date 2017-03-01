///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Description & Info//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Authors & Permitted Users: Kirtana Rajan, Kristian Diaz, Matt Fach, David Owen

This code is created to read Analog Voltage and write a binary state to each sensor.
Binary states will then be analyzed as a combination to decode had position into ASL characters.
This version of the code will only differentiate between ALL NON-SPECIAL CHARACTERS meaning all
characters that dont require special hardware:Accelerometer,Pressure Sensor, Gyroscope.

This Method is heavily based on the ALS Alphabet to Binary Sensor Conversion Table created by 
Kirtana M. Rajan UCSD winter 2015


note: Use the following for device hook-up
Read_1-->thumb --> Thumb
Read_2-->indexBottom --> Index 1 (closest to palm)
Read_3-->indexTop --> Index 2 (closest to finger tip)

Read_4-->middleBottom --> Middle 1 (closest to palm)a
Read_5-->middleTop --> Middle 2 (Closest to finger tip)
Read_6-->ringBottom --> Ring 1 (Closest to palm)

Read_7-->ringTop --> Ring 2 (Closest to finger tip)
Read_8-->pinkyBottom --> Little 1 (Closest to palm)
Read_9-->pinkyTop --> Little 2 (Closest to finger tip)
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Coding Begins Here//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>
#include "Adafruit_BLE_UART.h"
// Connect CLK/MISO/MOSI to hardware SPI
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_REQ 2
#define ADAFRUITBLE_RDY 1     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 0

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);




//Global Initializations
float THRESH=3.5;                //////////////DIS IS THE THRESHOLD THINGY~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  ADJUST THIS FOR THE ON OFF THIGY!
int Active;
//------------------------------------------------------------------------------------------------------------------------------------------------
//these constants describe the pins. They won't change:

const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)
//------------------------------------------------------------------------------------------------------------------------------------------------
int fsrPin = 12;     
int fsrReading; 

//------------------------------------------------------------------------------------------------------------------------------------------------
// the setup routine runs once when you press reset:
void setup() 
{
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init
  BTLEserial.begin();
  aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;
  
  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  
  while(status !=ACI_EVT_CONNECTED)
  {
   // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("Waiting for Bluetooth connection..."));
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
        break;
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }

  }
  
 //----------------------------------------------------------------------------------------------------------------------------------------------
// pinMode(groundpin, OUTPUT);
// pinMode(powerpin, OUTPUT);
// digitalWrite(groundpin, LOW); 
// digitalWrite(powerpin, HIGH);
  //----------------------------------------------------------------------------------------------------------------------------------------------

}


//Functioning Code Begins Here
void loop() 
{  
// read the analog sensor pins
//================================================================================================================================================
// NEED TO CHANGE THE ANALOG PINS SINCE THE ACCELEROMETER REQUIRES A0 (Z), A1 (Y) AND A2 (X)  [DEPENDS ON WHETHER THE BOARD IS FROM SPARKFUN OR ACCELEROMETER]
//  // print the sensor values:
Serial.print(analogRead(xpin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(ypin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(zpin));
  Serial.println();
  // delay before next reading:
  delay(100);
  
  //----------------------------------------------------------------------------------------------------------------------------------------------
  int Read_1 = analogRead(A9); //Thumb
  int Read_2 = analogRead(20); //Index Bottom
  int Read_3 = analogRead(21); //Index top
  int Read_4 = analogRead(17); //Middle Bottom
  int Read_5 = analogRead(18); //Middle Top
  int Read_6 = analogRead(A11);//Ring Bottom
  int Read_7 = analogRead(A13); //Ring Top
  int Read_8 = analogRead(A16); //Pinky Bottom
  int Read_9 = analogRead(A17); //Pinky Top
  // Analog pins map 0v-5v into values 0-1023.
  //The values need to be converted into a digital voltage value

  Serial.print("Read: ");
  Serial.println(Read_1);
  Serial.print("Read: ");
  Serial.println(Read_2);
  Serial.print("Read: ");
  Serial.println(Read_3);
  Serial.print("Read: ");
  Serial.println(Read_4);
  
  
//////////////////RIGHT HERE WHERE THESE STUPID 3.3 VOLTAGE THINGIES ARE!!!  THIS  THEWE ARE THE ADC VOLTAGE REFRENCE!!!!  THERE HERE!!  DO ONE THING FOR ME!  ...CAN YOU INTRODUCE ME TO THAT CUTE READHEAD.....
  float thumb=Read_1*(3.3/1023.0); 
  float indexBottom=Read_2*(3.3/1023.0); 
  float indexTop=Read_3*(3.3/1023.0);
  float middleBottom=Read_4*(3.3/1023.0); 
  float middleTop=Read_5*(3.3/1023.0); 
  float ringBottom=Read_6*(3.3/1023.0);
  float ringTop=Read_7*(3.3/1023.0); 
  float pinkyBottom=Read_8*(3.3/1023.0); 
  float pinkyTop=Read_9*(3.3/1023.0);
  
// print out the values read for De-Bugging purposes:
Serial.println("_______________________"); 
Serial.print("Thumb: " );
Serial.println(thumb); 
Serial.print("Index Bottom: ");
Serial.println(indexBottom); 
Serial.print("Index Top: ");
Serial.println(indexTop); 
Serial.print("Middle Bottom :");
Serial.println(middleBottom); 
Serial.print("Middle Top :");
Serial.println(middleTop); 
Serial.print("Ring Bottom :");
Serial.println(ringBottom); 
Serial.print("Ring Top :");
Serial.println(ringTop); 
Serial.print("Pinky Bottom :");
Serial.println(pinkyBottom); 
Serial.print("Pinky Top :");
Serial.println(pinkyTop);
Serial.println("_______________________");
//------------------------------------------------------------------------------------------------------------------------------------------------
Sensor_one();
delay(800); 
 // ----------------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Voltage Hardcoding Setting///
//Debugging Purposes ONLY//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//float on = 2.5; 
//float off = 1.5;
//thumb = on;
//indexBottom = off;
//indexTop = on;
//middleBottom = on;
//middleTop = on;
//ringBottom = on;
//ringTop = on;
//pinkyBottom = on;
//pinkyTop = on;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Find number of "Active" inputs. Active if sensor is bent:Voltage>=Threshold//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Active=0;
Serial.print("Active Updated to ");
Serial.println(Active);
if(thumb>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(indexBottom>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(indexTop>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(middleBottom>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(middleTop>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(ringBottom>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}    
if(ringTop>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(pinkyBottom>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}
if(pinkyTop>=THRESH)
{
  Active=Active+1; 
  Serial.print("Active Updated to "); 
  Serial.println(Active);
}      
delay(1000);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Deduction Cascade//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(Active==1)
{
  Serial.print("Active Detected to be: "); 
  Serial.println(Active);
  //Only 1 Letter associated with 1-Active
  Serial.println(". Letter detected is B");   
  BLuetooth("B");
  
}
if(Active==2)
{
  Serial.print("Active Detected to be: "); 
  Serial.println(Active);
//Only 1 Letter associated with 2-Active
Serial.println(". Letter detected is H"); 
BLuetooth("H");  
}
if(Active==3)
{
  Serial.print("Active Detected to be: "); 
  Serial.println(Active);
  //There are 3 Letters are Associated with 3-Active  
  //Of those 3 only 2 are associated with Thumb-Active
  if(thumb>=THRESH)
  {
    //Of the 2 only 1 is associated with Pointer_1-Active
    if(indexBottom>=THRESH)
    {
      Serial.println(". Letter Detected is F");
      BLuetooth("F");
    }
    else
    {
      Serial.println(". Letter Detected is W");
      BLuetooth("W");
    }
  }
  else
  {
    Serial.println(". Letter Detected is Y");
    BLuetooth("Y");
  }
}


if(Active==4)
{
  Serial.print("Active Detected to be: "); 
  Serial.println(Active);
//There are 6 Letters Associated with 4-Active  
//Of those 6 only 4 are associated with Thumb-Active
if(thumb>=THRESH)
{if(ringTop=THRESH && fsrReading >= 700 && fsrReading < 1010){
      Serial.println("Letter Detected is U");
}
      else {
      Serial.println("Letter detected is V");
    }
//Of the 4 only 1 is associated with Pointer_1-Active 
}
else if(indexBottom>=THRESH)  
    {
      if (pinkyBottom>=THRESH)
      {
         Serial.println(". Letter Detected is D");
      BLuetooth("D");
      }
      else
      {
      Serial.println(". Letter Detected is I");
      BLuetooth("I");
      }
    }
    //Of the 3 only 2 are associated with Pointer_2-Active
    else if(indexTop>=THRESH)
    {
      if(middleBottom>=THRESH)
      {
        Serial.println(". Letter Detected is X");
        BLuetooth("X");
      }
      else
      {
        Serial.println(". Letter Detected is C");
        BLuetooth("C");
      }
    }
    else if (ringBottom>=THRESH)
    { 
      if (pinkyBottom>=THRESH)
    {
      Serial.println(". Letter Detected is A");
      BLuetooth("A");
    }
    else if (xpin >= 455 && xpin <= 537)
    { 
      Serial.println(".Letter detected is K");
      BLuetooth("K");
    }
    else
    Serial.println(". Letter Detected is P");
    BLuetooth("P");
}
}


if(Active==5)
{
  if (indexBottom>=THRESH)
  {
    Serial.print("Active Detected to be: "); 
    Serial.println(Active);
//2 Letter associated with 5-Active
Serial.println(". Letter detected is E");
BLuetooth("E");
}
else
Serial.println(".Letter detected is R");
BLuetooth("R");
}

if(Active==6)
{
  if (middleTop>=THRESH)
  {
    if (ringBottom>=THRESH)
    {
     Serial.println(". Letter detected is S");
     BLuetooth("S");
   }
   else if (xpin <=374 && xpin >=355) 
   {
    Serial.print("Active Detected to be: "); 
    Serial.println(Active);
    Serial.println(". Letter detected is Q"); 
    BLuetooth("Q");
  }
  else 
  { 
    Serial.print("Active Detected to be: "); 
    Serial.println(Active);
    Serial.println(". Letter detected is L");
    BLuetooth("L");  
  }
}   
}

if(Active==7)
{
  if (indexBottom >=THRESH)
  {
    Serial.print("Active Detected to be: "); 
    Serial.println(Active);
    //Only 1 Letter associated with 7-Active
    Serial.println(". Letter detected is J"); 
    BLuetooth("J");
  }
  else if (xpin >=355 && xpin <=510) 
  {
 Serial.print("Active Detected to be: "); 
 Serial.println(Active);
 Serial.println(". Letter detected is Z");
 BLuetooth("Z");  
}
else
Serial.println(".Letter detected is G");
BLuetooth("G");
}

if(Active==8)
{
  Serial.print("Active Detected to be: "); 
  Serial.println(Active);
  Serial.println(". There are no letters associated with 8 Active sensors.");  
//No Letters associated with 8-Active
}
if(Active==9)
{
  if (fsrReading >= 700 && fsrReading < 1010)
  {
    Serial.print("Active Detected to be: "); 
    Serial.println(Active);
// 3 Letter associated with 9-Active
Serial.println(". Letter detected is M");
BLuetooth("M");
}
else if (fsrReading >= 100 && fsrReading < 700) 
{
  Serial.println(". Letter detected is N");
  BLuetooth("N");
}
else if (fsrReading < 100)
{
  Serial.println(".Letter detected is T");
  BLuetooth("T");
}
else
Serial.println(".Letter detected is O");
BLuetooth("O");
}
delay(1000);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

int Sensor_one()
{

 fsrReading = analogRead(fsrPin);  
 
 Serial.print("Analog reading = ");
  Serial.print(fsrReading);     // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (fsrReading < 100) 
  {
    Serial.println(" - Big Squeeze");
    return fsrReading;
  } 
  else if (fsrReading < 400) 
  {
   Serial.println(" - Medium Squeeze");
   return fsrReading;
 } 
 else if (fsrReading < 700) 
 {
   Serial.println(" - Light squeeze");
   return fsrReading;
 } 
 else if (fsrReading < 1010) 
 {
   Serial.println(" - Light Touch");
   return fsrReading;
 } 
 else 
 {
  Serial.println(" - No Pressure");
  return fsrReading;
}  

}



void BLuetooth(String d)
{
     BTLEserial.print(d);
}




//--------------------------------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Coding Ends Here//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






