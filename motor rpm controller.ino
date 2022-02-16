#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Timer.h>

const int rs = 12, en = 11, d4 = 6, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int PWM_A = 3;
const int PWM_B = 5;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A1, A0 , 4, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Timer timing;

const int IR_Sensor = 2;

volatile unsigned long  turns = 0;
unsigned long lastMillis = 0;
unsigned long Now = 0;



void setup() {
  // put your setup code here, to run once:


  lcd.begin(16, 2);
  pinMode(PWM_B, OUTPUT);

  runMotor(0);

  lcd.print("JADO & ETIENNE");
  lcd.setCursor(0, 1);
  lcd.print("SPEED CONTROL");
  delay(2000);
  lcd.clear();

  attachInterrupt(digitalPinToInterrupt(IR_Sensor), count_speed, FALLING);

  timing.every(10000, compute_rpm);

}

volatile unsigned long  rpm = 0;

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();

  timing.update();

  if (key) {

    switch (key) {


      case 'A' :
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">>Speed Max");
        delay(1000);
        lcd.clear();

        runMotor(255);
        break;

      case 'B' :
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">>Speed 75%");
        delay(1000);
        lcd.clear();

        runMotor(192);
        break;
      case 'C' :
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">>Speed 50%");
        delay(1000);
        lcd.clear();

        runMotor(127);
        break;
      case 'D' :
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">>Speed 25%");
        delay(1000);
        lcd.clear();

        runMotor(64);
        break;
      case '0' :
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">>Motor stopped");
        delay(1000);
        lcd.clear();

        runMotor(0);
        break;
      default :
        runMotor(1);
        break;
    }
  }

  lcd.setCursor(0, 1);

  if (rpm < 10) {
    lcd.print(">>" + String(rpm) + "   RPM");
  }
  else if (rpm < 100) {
    lcd.print(">>" + String(rpm) + "  RPM");
  }
  else if (rpm < 1000) {
    lcd.print(">>" + String(rpm) + " RPM");
  }
  else {
    lcd.print(">>" + String(rpm) + "RPM");
  }

}

void count_speed () {
  turns++;
}

int kt = 0;
int moy[6] = {0, 0, 0, 0, 0, 0};
bool hasCounted = false;

void  compute_rpm() {
  if (!hasCounted) rpm = turns * 6;
  hasCounted = true;

  if (kt > 5) kt = 0;

  moy[kt] = turns;

  if (kt == 5) {
    unsigned long calc = moy[0] + moy[1] + moy[2];
    calc = calc / 3;
    rpm = calc * 2;
    turns = 0;
  }
  kt++;
}

void runMotor(int speed) {
  timing.update();
  if (speed == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">>Speed Uknown");
    delay(1000);
    lcd.clear();
    speed = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print(">>SPEED CONTROL");
  digitalWrite(PWM_B, LOW);
  delay(100);
  analogWrite(PWM_B, speed);
}
