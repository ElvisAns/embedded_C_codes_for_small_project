
#define button_right 5
#define button_left 6

int etat_button_left = HIGH;
int etat_button_right = HIGH;

int LED_left = 7;
int LED_right = 8;



int hasLockedLeft=false;
int hasLockedRight=false;

void setup()
{
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);


  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

} // setup()



void loop()
{
  etat_button_left = digitalRead(button_left);
  etat_button_right = digitalRead(button_right);

  delay(100);

  if (etat_button_left != digitalRead(button_left)) {
    if (!hasLockedRight) {
      hasLockedLeft=true;
      digitalWrite(LED_left,HIGH);
      digitalWrite(LED_right,LOW);
      delay(15000);
      digitalWrite(LED_left,LOW);
      digitalWrite(LED_right,LOW);
      hasLockedLeft=false;
    }
  }

 if (etat_button_right != digitalRead(button_right)) {
     if (!hasLockedLeft) {
      hasLockedRight=true;
      digitalWrite(LED_left,LOW);
      digitalWrite(LED_right,HIGH);
      delay(15000);
      digitalWrite(LED_left,LOW);
      digitalWrite(LED_right,LOW);
      hasLockedRight=false;
    }
  }
}
