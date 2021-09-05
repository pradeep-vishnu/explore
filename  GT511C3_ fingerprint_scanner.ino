/*****************************************************************
  An example script for exploring GT511C3 fingerprint scanner. 

  Description: This sketch will attempt to identify a previously enrolled fingerprint
  saved in its database.
 
-------------------- HARDWARE HOOKUP with 5V Arduino --------------------
Pin Mapping :

     UART_TX (3.3V TTL)(Pin 1) <->     LV1 <-> HV1       <->  RX (pin 4)
     UART_RX (3.3V TTL)(Pin 2) <->     LV4 <-> HV4       <->  TX (pin 5)
           GND         (Pin 3) <->     GND <-> GND       <->     GND
      Vin (3.3V~6V)    (Pin 4) <->        HV             <->      5V
                                          LV             <->     3.3V
            
Voltage Division w/ 3x 10kOhm Resistors
Otherwise, you could use 3x 10kOhm resistors:

    Voltage Divider         <-> Fingerprint Scanner(Pin #) <-> Voltage Divider <-> 5V Arduino w/ Atmega328P
                            <-> UART_TX (3.3V TTL) (Pin 1) <->                 <->       RX (pin 4)
  GND <-> 10kOhm <-> 10kOhm <-> UART_RX (3.3V TTL) (Pin 2) <->      10kOhm     <->       TX (pin 5)
          GND               <->        GND         (Pin 3) <->       GND       <->        GND
                            <->    Vin (3.3V~6V)   (Pin 4) <->                 <->        5V
          
--------------------------------------------------------------------------------
*****************************************************************/

#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

FPS_GT511C3 fps(4, 5);

int buzzer = 11;
int vibrator= 3;
int enroll = 6;
int lock=2;
int s1 = 0,s2=0, A=9;
void setup()
{
  
  pinMode(A,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(vibrator,OUTPUT);
  pinMode(enroll,INPUT);
  pinMode(lock,INPUT);
  Serial.begin(9600); //set up Arduino's hardware serial UART
  delay(100);
  fps.Open();         //send serial command to initialize fps
  //fps.SetLED(true);   //turn on LED so fps can see fingerprint
  Serial.println("Please press finger");
  identify();
}

void loop()
{
  
}
void identify()
{
fps.SetLED(true);
  // Identify fingerprint test
  if (fps.IsPressFinger())
  {
    
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    
       /*Note: 
                GT-511C3 can hold 200 fingerprint templates. */

    if (id <200)
    {//if the fingerprint matches, provide the matching template ID
      Serial.print("Verified ID:");
      Serial.println(id);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      fps.SetLED(false);
      Serial.println("Inside Car");
      unlocked();
    }
    else
    {//if unable to recognize
      Serial.println("Finger not found");
      digitalWrite(vibrator, HIGH);
      delay(250);
      digitalWrite(vibrator, LOW);
      delay(250);
      digitalWrite(vibrator, HIGH);
      delay(250);
      digitalWrite(vibrator, LOW);
      Serial.println("Please press finger");
    }
  }
  /*else
  {
    Serial.println("Please press finger");
  }*/
  delay(1000);
 fps.SetLED(false);
 delay(1000);
 identify();
}

void unlocked()
{
 s2=0;
 s1=0;
 digitalWrite(A, HIGH);
 s1 = digitalRead(enroll);
 s2 = digitalRead(lock);
 
 if(s2==1)
 {
 digitalWrite(buzzer, HIGH);
 delay(200);
 digitalWrite(buzzer, LOW);
 Serial.println("Locked");
 digitalWrite(A2, LOW);
 Serial.println("Please Press Finger");
 identify();
 }
 
 if (s1==1)
 {Enroll();}
 
 unlocked();
}
void Enroll()
{
  // Enroll test

  // find open enroll id
  fps.SetLED(true);
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid==true) enrollid++;
  }
  fps.EnrollStart(enrollid);

  // enroll
  Serial.print("Press finger to Enroll #");
  Serial.println(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    Serial.println("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      Serial.println("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        Serial.println("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0)
        {
          Serial.println("Enrolling Successful");
          fps.SetLED(false);
          unlocked();
        }
        else
        {
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
          unlocked();
        }
      }
      else { Serial.println("Failed to capture third finger"); }
    }
    else {  Serial.println("Failed to capture second finger");}
  }
  else {   Serial.println("Failed to capture first finger");}
 fps.SetLED(false);
 unlocked();
 
}
