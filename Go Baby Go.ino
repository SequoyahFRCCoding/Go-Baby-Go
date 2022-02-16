#include <Servo.h>

#define PIN_X A0
#define PIN_Y A1
#define PIN_SWITCH 5
#define PIN_RC_2 3
#define PIN_RC_4 2
#define PIN_OUT_RC_1 11
#define PIN_OUT_RC_2 12

volatile int pwm2time = 0;
volatile int pwm4time = 0;
volatile int useJoystick = 0;
volatile int pwm2, pwm4;
Servo rc2;
Servo rc4;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RC_2, INPUT);
  pinMode(PIN_RC_4, INPUT);
  pinMode(PIN_OUT_RC_2, OUTPUT);
  pinMode(PIN_OUT_RC_1, OUTPUT);
  pinMode(PIN_SWITCH, INPUT);
  rc2.attach(PIN_OUT_RC_2);
  rc4.attach(PIN_OUT_RC_1);
  attachInterrupt(digitalPinToInterrupt(PIN_RC_2), int2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_RC_4), int4, CHANGE);
}

void int2() {
  if (digitalRead(PIN_RC_2) == HIGH) {
    pwm2time = micros();
  } else {
    pwm2 = micros() - pwm2time;
    if (!useJoystick) {
      rc2.writeMicroseconds(pwm2);
    }
  }
}

void int4() {
  if (digitalRead(PIN_RC_4) == HIGH) {
    pwm4time = micros();
  } else {
    pwm4 = micros() - pwm4time;
    if (!useJoystick) {
      rc4.writeMicroseconds(pwm4);
    }
  }
}

void loop() {
  useJoystick = pulseIn(PIN_SWITCH, HIGH) > 1500;
  if (useJoystick) {
    int x = map(analogRead(PIN_X), 0, 1023, 2000, 1000);
    int y = map(analogRead(PIN_Y), 0, 1023, 2000, 1000);
    x = map(x, 1000, 2000, 1300, 1700);
    y = map(y, 1000, 2000, 1300, 1700);
    Serial.println("X = " + String(x) + " Y = " + String(y));
    rc2.writeMicroseconds(y);
    rc4.writeMicroseconds(x);
  } else {
    Serial.println("X = " + String(pwm2) + " Y = " + String(pwm4));
  }
}
