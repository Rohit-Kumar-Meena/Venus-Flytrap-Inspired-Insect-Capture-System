#include <AccelStepper.h>

#define MotorInterfaceType 4

// Stepper motor pins
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// Limit switch pin
#define LIMIT_SWITCH_PIN 12

// L298N driver pins
#define ENA 9
#define INA1 A1
#define INA2 A2

AccelStepper myStepper(MotorInterfaceType, IN1, IN3, IN2, IN4);

String command;

bool limitTriggered() {
  return (digitalRead(LIMIT_SWITCH_PIN) == LOW);
}

void stopDCMotor() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  analogWrite(ENA, 0);
}

void rotateDCMotorForward(int speedValue) {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  analogWrite(ENA, speedValue);
}

void rotateDCMotorBackward(int speedValue) {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  analogWrite(ENA, speedValue);
}

void homeMotor() {

  myStepper.setMaxSpeed(1200);
  myStepper.setAcceleration(1000);

  myStepper.moveTo(-100000);

  while (!limitTriggered()) {
    myStepper.run();
  }

  myStepper.setCurrentPosition(0);
}

void forwardSequence() {

  myStepper.setMaxSpeed(1900);
  myStepper.setAcceleration(1200);

  myStepper.moveTo(1200);

  while (myStepper.distanceToGo() != 0) {
    myStepper.run();
  }

  rotateDCMotorForward(40);

  myStepper.setMaxSpeed(250);
  myStepper.setAcceleration(200);

  myStepper.moveTo(1900);

  while (myStepper.distanceToGo() != 0) {
    myStepper.run();
  }

  stopDCMotor();
}

void reverseSequence() {

  rotateDCMotorBackward(40);

  myStepper.setMaxSpeed(1900);
  myStepper.setAcceleration(1200);

  myStepper.moveTo(1000);

  while (myStepper.distanceToGo() != 0) {
    myStepper.run();
  }

  stopDCMotor();
}

void setup() {

  Serial.begin(9600);

  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);

  pinMode(ENA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);

  stopDCMotor();

  delay(1000);

  homeMotor();
}

void loop() {

  if (Serial.available()) {

    command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "f") {
      forwardSequence();
    }

    else if (command == "r") {
      reverseSequence();
    }
  }
}
