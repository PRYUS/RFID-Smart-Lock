#include <LiquidCrystal.h>
#include <Servo.h> 
#include<SoftwareSerial.h>
SoftwareSerial mySerial(13, 10); //gsm
LiquidCrystal lcd(12,11,5,4,3,2);
int sensor=7; 
int sensor_value; 
int sms_count=0;
char master1[] ="$0007234574"; //Master card 1
char master2[]="$0007629170"; // Master card 2    
char name0[]="Prathyush";
char name1[]="Rutvik";  
String nm0 = "Prathyush";
String nm1 = "Rutvik";
int i2=6;  
int i1=9;
char input[11];        
Servo servo1;  

int count = 0; 
int countb=0;       
int flag = -1;
int f1=-1;
int i;
int flag2=1;   
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(sensor,INPUT);  
  lcd.begin(16,2);
  lcd.setCursor(0,1);
  lcd.print("I am alive");  
  servo1.attach(8);
  servo1.write(0);   
 // lcd.begin(16,2);
}
void loop()
{
  if(Serial.available())
  {
    count = 0;
    while(Serial.available() && count < 11) 
    {
      input[count] = Serial.read(); 
      count++; 
      delay(5);
    }
    if(count == 11) // 
    {
      count =0; 
      flag = 1;
      while(count<11 && flag !=0)  
      {
        if(input[count]==master1[count]||input[count]==master2[count])
        {
          flag = 1; 
          if(input[count]==master1[count])
          {
              f1=0;
          }
          else
          {
              f1=1;
          }
        }
        else
        flag= 0; 
                          
        count++; 
    }
    for(count=0; count<11; count++) 
    {
      input[count]= 'F';
    }
    count = 0; 
  }
   if(flag == 1) // If flag variable is 1, then it means the tags match
   {
      servo1.write(150);
      Serial.print("\n");
      Serial.print("Welcome ");
      //flag2=0;
      flag2=0;
      countb=0;
      if(f1==0)
      {
        for(i=0;i<i1;i++)
        {
          Serial.print(name0[i]);
        }
        sendMessage_2(nm0);
        lcd.noDisplay();
        lcd.display();
        lcd.setCursor(0,1);
        lcd.print("Welcome " + nm0);
        lcd.print("Door opening for 5 seconds...");
        Serial.print("\n");
      }
      if(f1==1)
      {
        sendMessage_2(nm1);
        for(i=0;i<i2;i++)
        {
          Serial.print(name1[i]);
        }
        lcd.noDisplay();
        lcd.display();
        lcd.setCursor(0,1);
        lcd.print("Welcome" + nm1);
        lcd.print("Door opening for 5 seconds...");
        Serial.print("\n");
      }
         
      delay (5000);
      lcd.noDisplay();
      lcd.display();
      lcd.setCursor(0,1);
      lcd.print("Door closing");
      servo1.write(0);   
      flag=-1;
      delay(1000);//Waits for the user to move away from the door
      lcd.noDisplay();
    }
    else if(flag==0)
    {
      delay (1000);
      Serial.print("\n");
      Serial.print("Access Denied");
      //lcd.noDisplay();
      //lcd.display();
      lcd.print("Access Denied");
      delay(1000);
      //lcd.noDisplay();
      countb+=1;
      flag=-1;
      flag2=1;
    }
    if(flag2==1)
    {
      Check_Burglar();
    }
  }
  if(flag2==1)
  {
    Check_Burglar();
  }
   if(countb==3)
   {
    SendTextMessage_2();
    countb=0;
   }
}

void Check_Burglar()
{
sensor_value=digitalRead(sensor); // Reading sensor value from pin 7
  if(sensor_value==HIGH) // Checking if PIR sensor sends a HIGH signal to Arduino
  {
    Serial.print("PIR HIGH!");
    SendTextMessage(); // Function to send AT Commands to GSM module
    flag2=0;
  }
}

void sendMessage_2(String name)
{
  int i;
  //Serial.print("Sending Message Of Arrival");
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+918951011498\"\r"); // change to the phone number you using 
  delay(1000);
  mySerial.print(name);
  mySerial.print(" has just entered into the house");
  mySerial.print("\n");    
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(1000);
}
void SendTextMessage()
{
  Serial.print("Sending Message..");
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+918951011498\"\r"); // change to the phone number you using 
  delay(1000);
  mySerial.println("Someone Intruded in your Room Sir\nDial 100?");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(1000);
  sms_count++;
}
void SendTextMessage_2()
{
  Serial.print("Program is screwed!");
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+918951011498\"\r"); // change to the phone number you using 
  delay(1000);
  mySerial.println("Multiple faulty attempts on your door.. Was that you?");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(1000);
  sms_count++;
}
