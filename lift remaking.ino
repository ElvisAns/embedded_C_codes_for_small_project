#include <ezButton.h>
#include <Timer.h>

#define EN 7
#define IN1 5
#define IN2 6

#define P_FLOW_1 A4
#define P_FLOW_2 A2
#define P_FLOW_3 A0

#define BUTTON_FLOW_1 3
#define BUTTON_FLOW_2_UP 21
#define BUTTON_FLOW_2_DOWN 20
#define BUTTON_FLOW_3 2

#define LEDA 8
#define LEDB 9
#define LEDC 10

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255
#define PIN_HIGH 3
#define PIN_LOW 2

ezButton button1(BUTTON_FLOW_1);
ezButton button2(BUTTON_FLOW_3);
ezButton button3(BUTTON_FLOW_2_UP);
ezButton button4(BUTTON_FLOW_2_DOWN);

int ledState = LOW;

Timer manage;

bool  allowed = false;

int p = 0;

unsigned long timeout;

unsigned long interval = 100;           // interval at which to blink (milliseconds)
unsigned long previousMillis = 0;

bool hasMoved = false;
bool hasMoved_A_UP = false;
bool hasMoved_A_DOWN = false;
bool hasMoved_B = false;

bool count_r = false;

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  button1.setDebounceTime(50); // set debounce time to 50 milliseconds
  button2.setDebounceTime(50); // set debounce time to 50 milliseconds
  button3.setDebounceTime(50); // set debounce time to 50 milliseconds
  button4.setDebounceTime(50); // set debounce time to 50 milliseconds


  pinMode(EN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);


  pinMode(LEDA, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDC, OUTPUT);

  digitalWrite(LEDA, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDC, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN, 0);

  timeout = millis();
}

int pin_value = 0;
int eve = 0;

void loop() {

  unsigned long currentMillis = millis();

  manage.update();

  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first
  button3.loop(); // MUST call the loop() function first
  button4.loop(); // MUST call the loop() function first

  if (button3.isReleased()) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(10);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(EN, 0);
    allowed = !allowed;

    hasMoved = false;
    hasMoved_A_UP = true;
    hasMoved_A_DOWN = false;
    hasMoved_B = false;
    if (!allowed) {
      digitalWrite(LEDA, LOW);
      digitalWrite(LEDB, HIGH);
      digitalWrite(LEDC, LOW);
    }
  }

  if (button4.isReleased()) {

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(10);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(EN, 0);
    allowed = !allowed;

    hasMoved = false;
    hasMoved_A_UP = false;
    hasMoved_A_DOWN = true;
    hasMoved_B = false;
    if (!allowed) {
      digitalWrite(LEDA, LOW);
      digitalWrite(LEDB, HIGH);
      digitalWrite(LEDC, LOW);
    }
  }


  if (button1.isReleased()) {

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(10);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(EN, 0);
    allowed = !allowed;

    hasMoved = false;
    hasMoved_A_UP = false;
    hasMoved_A_DOWN = false;
    hasMoved_B = true;

    if (!allowed) {
      digitalWrite(LEDA, LOW);
      digitalWrite(LEDB, LOW);
      digitalWrite(LEDC, HIGH);
    }

  }

  if (button2.isReleased()) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(10);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(EN, 0);
    allowed = !allowed;
    hasMoved = true;
    hasMoved_A_UP = false;
    hasMoved_A_DOWN = false;
    hasMoved_B = false;
    if (!allowed) {
      digitalWrite(LEDA, LOW);
      digitalWrite(LEDB, LOW);
      digitalWrite(LEDC, LOW);
    }
  }


  if (allowed) {
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }

      digitalWrite(LEDA, ledState);
      digitalWrite(LEDB, ledState);
      digitalWrite(LEDC, ledState);

    }
  }


  if (allowed) {
    Serial1.flush();
    int ard_command = 0;
    int pin_num = 0;
    char get_char = ' ';
    // wait for incoming data
    if (Serial1.available() < 1) return;
    get_char = Serial1.read();
    if (get_char != START_CMD_CHAR) return;
    ard_command = Serial1.parseInt(); // read the command
    pin_num = Serial1.parseInt(); // read the pin
    pin_value = Serial1.parseInt();  // read the value
    if (pin_num == 11) {
      if (ard_command == CMD_ANALOGWRITE) {
        interval = map(pin_value, 0, 255, 500, 100);
        analogWrite( EN , pin_value );
        count_r = true;
        timeout = millis();
        if (pin_value > 70) {
          eve = manage.after((map(pin_value, 0, 255, 15000, 5000)) , doAfter);
        }

      }
    }
  }


}

void doAfter() {

  manage.stop(eve);
  count_r = false;
  allowed = false;

  if (hasMoved) {
    digitalWrite(LEDA, LOW);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDC, LOW);
    hasMoved = false;
  }

  if (hasMoved_A_UP) {
    digitalWrite(LEDA, HIGH);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDC, LOW);
    hasMoved_A_UP = false;

  }

  if (hasMoved_A_DOWN) {
    digitalWrite(LEDA, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDC, HIGH);
    hasMoved_A_DOWN = false;

  }


  if (hasMoved_B) {
    digitalWrite(LEDA, LOW);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDC, LOW);
  }

  analogWrite(EN, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
