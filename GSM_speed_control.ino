#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//Create software serial object to communicate with A6
SoftwareSerial mySerial(3,2); //A6 Tx & Rx is connected to Arduino #3 & #2

//send any shorter message containing "speed option" where option can be max, min, half or any number between 0 and 99

/*
  If your message is long enough just like ours, then you’ll probably receive it with some missing characters. This is not because of a faulty code. Your SoftwareSerial receive buffer is getting filled up and discarding characters. You are not reading fast enough from the buffer.

  The simplest solution to this is to increase the size of the SoftwareSerial buffer from its default size of 64 bytes to 512 bytes (or smaller, depending on what works for you).



  On a Windows PC, go to C:\Program Files ....>>>>>>>>>>>.

*/

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define T2 A0 
#define T1 A1
#define T4 6
#define T3 5


char message[600];
int pos = 0;
int speed = 0;
void clean_buffer() {
  memset(message, 0, 600);
  pos = 0;
}
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  pinMode(T1,OUTPUT);
  pinMode(T2,OUTPUT);
  pinMode(T3,OUTPUT);
  pinMode(T4,OUTPUT);
 
  //Begin serial communication with Arduino and A6
  mySerial.begin(19200);

  Serial.println("Initializing...");
  delay(1000);
  lcd.begin(16,2);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Network");
  lcd.setCursor(0,1);
  lcd.print("connexion...");
  delay(10000);
  lcd.clear();
  lcd.print("May be it's Ok!");
  delay(2000);
  lcd.clear();
  lcd.print("Motor stopped");
  drive_to();
}

void loop()
{
  updateSerial();
  drive_to();
}

void updateSerial()
{  
  while (Serial.available())
  {
    message[pos] = (char)Serial.read();//Forward what Software Serial received to Serial Port
    pos++;
    if (pos > 599) {
      lcd.print("Use shorter command");
      clean_buffer();
    }
  }

Serial.print( strcmp(message, "speed max"));

  while (mySerial.available())
  {
    message[pos] = (char)mySerial.read();//Forward what Software Serial received to Serial Port
    pos++;
    if (pos > 599) {
      lcd.clear();
      lcd.print("Use shorter command");
      clean_buffer();
    }
  }

  if ( String(message).indexOf("speed max") >= 0 || String(message).indexOf("speed 100%") >= 0 ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Command received");
    digitalWrite(T1,LOW);
    digitalWrite(T4,LOW);
    delay(1000);
    lcd.clear();
    lcd.print("Driving at 100%");
   
    speed = 100;
  }
  else if ( String(message).indexOf("speed min") >= 0 || String(message).indexOf("speed 0%") >= 0 ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Command received");
    digitalWrite(T1,LOW);
    digitalWrite(T4,LOW);
    delay(1000);
    lcd.clear();
    lcd.print("Motor stopped");
    
    speed = 0;
  }
  else if ( String(message).indexOf("speed half") >= 0 || String(message).indexOf("speed 50%") >= 0 ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Command received");
    digitalWrite(T1,LOW);
    digitalWrite(T4,LOW);
    delay(1000);
    lcd.clear();
    lcd.print("Driving at half");
    speed = 50;
  }

  else if ( String(message).indexOf("speed") >= 0 ) {
    String level = String(message);
    speed =  level.substring(level.lastIndexOf("speed") + 5, level.lastIndexOf("speed") + 8).toInt();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Command received");
    digitalWrite(T1,LOW);
    digitalWrite(T4,LOW);
    delay(1000);
    lcd.clear();
    String g="Driving at "+String(speed)+"%";
    lcd.print(g);
  }
  clean_buffer();
}


void drive_to() {
  
  int pwm = (speed*255)/100; //max is 255

  Serial.println("PWM ..."+String(pwm));

  if(speed > 0) digitalWrite(T1, HIGH);

  else digitalWrite(T1, LOW);

 analogWrite(T4, pwm);
  }
