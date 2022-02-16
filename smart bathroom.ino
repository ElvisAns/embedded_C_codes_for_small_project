


#include <ezButton.h>

//to compile the code you have to download the ezButton library which actually 
//help us to manage the PIR events
#define LED 4
#define PIR 19 //Analog pin A5 is used as digital Pin which is declared as digital Pin 19
#define PIR2 15 //Analog pin A1 is used as digital Pin which is declared as digital Pin 15
#define PUMP 6
int hasEnterred = false;

ezButton pir(PIR2);

void setup() {
  // put your setup code here, to run once:
  pir.setDebounceTime(10); //10ms

  pinMode(LED, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(PUMP, OUTPUT);

  digitalWrite(LED, LOW);
  digitalWrite(PUMP, HIGH);
}
unsigned long state = 1;
void loop() {

  pir.loop();

  if (pir.isPressed()) {
    hasEnterred = !hasEnterred;
  }

  // put your main code here, to run repeatedly:

  while (hasEnterred) {
    pir.loop();

    int pir1_state = digitalRead(PIR);
    digitalWrite(LED, HIGH);
    digitalWrite(PUMP, !pir1_state);

    if (pir.isPressed()) {
      delay(500);
      hasEnterred = false;
      digitalWrite(LED, LOW);
      digitalWrite(PUMP, HIGH);
      return;
    }

  }


}
