#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

int show = -1;

#define button_accepter 5
#define button_refuser 6
#define button_demande 7
#define button_attente 8

int etat_button_attente = HIGH;
int etat_button_demande = HIGH;
int etat_button_accepter = HIGH;
int etat_button_refuser = HIGH;

int LED_DEMANDE = 9;
int LED_ACCEPT = 10;
int LED_REFUSER = 11;
int LED_ATTENTE = 12;

int counter = 0;

void setup()
{
  int error;

  for (int i = 5; i < 9; i++)
    pinMode(i, INPUT_PULLUP);

  for (int i = 9; i < 13; i++)
    pinMode(i, OUTPUT);

  for (int i = 9; i < 13; i++)
    digitalWrite(i, LOW);

  Serial.begin(115200);
  Serial.println("LCD...");

  // wait on Serial to be available on Leonardo
  while (!Serial)
    ;

  Serial.println("Does: check for LCD");

  // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
    lcd.begin(20, 4); // initialize the lcd

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.setBacklight(255);
} // setup()

int hasDisplayed = false;
int hasNotDisplayed = true;

int hasToWait = false;
int hasAsked = false;
int hasAgree = false;
int hasRefused = false;
String response = "";

void loop()
{
  etat_button_attente = digitalRead(button_attente);
  etat_button_accepter = digitalRead(button_accepter);
  etat_button_refuser = digitalRead(button_refuser);
  etat_button_demande = digitalRead(button_demande);

  delay(100);

  if (etat_button_attente != digitalRead(button_attente)) {
    if (hasAsked) {
      hasToWait = true;
      hasDisplayed = false;
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Veuillez entendre!");
      hasAsked = false;
      digitalWrite(LED_DEMANDE, HIGH);
      digitalWrite(LED_ACCEPT, LOW);
      digitalWrite( LED_ATTENTE, HIGH);
      digitalWrite(LED_REFUSER, LOW);
      delay(3000);

    }
  }

  else if (etat_button_accepter != digitalRead(button_accepter)) {
    if (hasAsked) {
      hasAgree = true;
      hasDisplayed = false;
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Entrez svp!");
      hasAsked = false;
      delay(3000);
      digitalWrite(LED_DEMANDE, HIGH);
      digitalWrite(LED_ACCEPT, HIGH);
      digitalWrite( LED_ATTENTE, LOW);
      digitalWrite(LED_REFUSER, LOW);
    }
  }

  else if (etat_button_refuser != digitalRead(button_refuser)) {
    if (hasAsked) {
      hasRefused = true;
      hasDisplayed = false;
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Pas d'entree");
      hasAsked = false;
      delay(3000);
      digitalWrite(LED_DEMANDE, HIGH);
      digitalWrite(LED_ACCEPT, LOW);
      digitalWrite( LED_ATTENTE, LOW);
      digitalWrite(LED_REFUSER, HIGH);
    }
  }

  else if (etat_button_demande != digitalRead(button_demande)) {
    hasAsked = true;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Demande en cours...");

    hasDisplayed = false;
  }

  else if (!hasAsked) {
    hasToWait = false;
    hasAgree = false;
    hasRefused = false;
    if (!hasDisplayed) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Audience ici!");
      hasDisplayed = true;
    }
    digitalWrite(LED_DEMANDE, LOW);
    digitalWrite(LED_ACCEPT, LOW);
    digitalWrite( LED_ATTENTE, LOW);
    digitalWrite(LED_REFUSER, LOW);
  }

}
