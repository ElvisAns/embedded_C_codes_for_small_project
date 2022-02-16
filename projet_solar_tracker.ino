#include <Servo.h>

#define LED1 3
#define LED2 5

#define ServoM 10

#define LDR_A A0
#define LDR_B A1

Servo controlPanneau;

int values_left[181], values_right[181], max_right = 0, max_left = 0, max_point_left = 0, max_point_right = 0;

int max_angle_point = 0;

void setup() {

  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  controlPanneau.attach(10);

  for (int x = 0; x <= 179; x++) {
    int state1 = x < 40 ? HIGH : LOW;

    digitalWrite(LED1, state1);
    digitalWrite(LED2, !state1);

    controlPanneau.write(x);
    delay(50);
    values_left[x] = analogRead(LDR_A);
    values_right[x] = analogRead(LDR_B);

    if (x > 0) {
      max_right = min(values_right[x - 1], max_right);
      max_left = min(values_left[x - 1], max_left);
    }
  }

  max_point_right = 90;
  max_point_left = 90;

  for (int k = 179 ; k >= 0; k--) {
    if (values_right[k] == max_right) {
      max_point_right = k;
    }
    if (values_left[k] == max_left) {
      max_point_left = k;
    }
  }

  max_angle_point = max(max_point_right,max_point_left);

  if (max_angle_point <= 90) { //we take LDR_B
    max_angle_point = max_point_right;
  }
  else { //take LDR_A
    max_angle_point = max_point_left;
  }


  controlPanneau.write(max_angle_point);

}


unsigned long reading1[10];

unsigned long reading2[10];


unsigned long l1 = 0;
unsigned long l2 = 0;

void loop() {


  for (int x = 0; x < 10; x++) {

    l1 = analogRead(LDR_A);
    l2 = analogRead(LDR_B);

    reading1[x] = l1;
    reading2[x] = l2;

    delay(20);

  }

  for (int x = 0; x < 10; x++) {
    l1 += reading1[x];
    l2 += reading2[x];
  }

  l1 = l1 / 10;
  l2 = l2 / 10;


  Serial.println();
  Serial.println(l1);
  Serial.println(l2);
  Serial.println();
  Serial.println();
  Serial.println();





  if (l1 > l2 & (l1 - l2 > 20) ) {
    int state1 = max_angle_point < 90 ? HIGH : LOW;
    digitalWrite(LED1, state1);
    digitalWrite(LED2, !state1);
    max_angle_point++;
    max_angle_point = max_angle_point < 0 ? 0 : max_angle_point;
    max_angle_point = max_angle_point > 180 ? 180 : max_angle_point;
    controlPanneau.write(max_angle_point);
    Serial.println(String(max_angle_point) + "deg");

  }

  else if (l2 > l1 & (l2 - l1 > 20)) {
    int state1 = max_angle_point < 90 ? HIGH : LOW;

    digitalWrite(LED1, state1);
    digitalWrite(LED2, !state1);
    max_angle_point--;
    max_angle_point = max_angle_point < 0 ? 0 : max_angle_point;
    max_angle_point = max_angle_point > 180 ? 180 : max_angle_point;
    controlPanneau.write(max_angle_point);
    Serial.println(String(max_angle_point) + "deg");
  }

}
