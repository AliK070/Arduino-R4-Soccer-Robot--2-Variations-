/*
  BLE Motor Control using Dabble App
  Board: Arduino UNO R4 WiFi (RA4M1 core)

  This code uses BLE to receive directional commands from Dabble app
  and controls motors using an H-bridge motor driver (L298N or similar).

  Author: AliK07
  Last updated: April 2025
*/

#include <Dabble.h> //https://thestempedia.com/docs/dabble/introduction/

// Define motor control pins
#define ain1 14  // Front right motor direction 1
#define ain2 12  // Front right motor direction 2
#define bin1 26  // Back right motor direction 1
#define bin2 27  // Back right motor direction 2
#define pwmM 13  // Front motor speed (PWM)
#define pwmM_2 25 // Back motor speed (PWM)

#define ain1_2 32  // Front left motor direction 1
#define ain2_2 33  // Front left motor direction 2
#define bin1_2 18  // Back left motor direction 1
#define bin2_2 19  // Back left motor direction 2

// Fixed speed
const int fixedSpeed = 200;

void setup() {
  Serial.begin(115200);   
  Dabble.begin("R4_BLE_Car"); 

  // Initialize all pins as outputs and start with motors off
  pinMode(ain1, OUTPUT); pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT); pinMode(bin2, OUTPUT);
  pinMode(pwmM, OUTPUT); pinMode(pwmM_2, OUTPUT);

  pinMode(ain1_2, OUTPUT); pinMode(ain2_2, OUTPUT);
  pinMode(bin1_2, OUTPUT); pinMode(bin2_2, OUTPUT);

  stopMotors(); 
}

void loop() {
  Dabble.processInput(); // NOTE TO ALI: Always call this in loop!!!!! + handle Gamepad commands from Dabble
  if (GamePad.isUpPressed()) {
    moveForward();
  } else if (GamePad.isDownPressed()) {
    moveBackward();
  } else if (GamePad.isLeftPressed()) {
    moveLeft();
  } else if (GamePad.isRightPressed()) {
    moveRight();
  } else {
    stopMotors(); // No button → stop
  }
}

void setMotorSpeed(int speed) {
  analogWrite(pwmM, speed);
  analogWrite(pwmM_2, speed);
}

//Stops all motors
void stopMotors() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW); digitalWrite(bin2, LOW);
  digitalWrite(ain1_2, LOW); digitalWrite(ain2_2, LOW);
  digitalWrite(bin1_2, LOW); digitalWrite(bin2_2, LOW);
  setMotorSpeed(0);
}

//Forward
void moveForward() {
  digitalWrite(ain1, HIGH); digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW);  digitalWrite(bin2, HIGH);
  digitalWrite(ain1_2, HIGH); digitalWrite(ain2_2, LOW);
  digitalWrite(bin1_2, HIGH); digitalWrite(bin2_2, LOW);
  setMotorSpeed(fixedSpeed);
}

//Backward
void moveBackward() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, HIGH);
  digitalWrite(bin1, HIGH); digitalWrite(bin2, LOW);
  digitalWrite(ain1_2, LOW); digitalWrite(ain2_2, HIGH);
  digitalWrite(bin1_2, LOW); digitalWrite(bin2_2, HIGH);
  setMotorSpeed(fixedSpeed);
}

//Turn Left
void moveLeft() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, HIGH);
  digitalWrite(bin1, LOW); digitalWrite(bin2, HIGH);
  digitalWrite(ain1_2, HIGH); digitalWrite(ain2_2, LOW);
  digitalWrite(bin1_2, LOW); digitalWrite(bin2_2, HIGH);
  setMotorSpeed(fixedSpeed);
}

//Turn Right
void moveRight() {
  digitalWrite(ain1, HIGH); digitalWrite(ain2, LOW);
  digitalWrite(bin1, HIGH); digitalWrite(bin2, LOW);
  digitalWrite(ain1_2, LOW); digitalWrite(ain2_2, HIGH);
  digitalWrite(bin1_2, HIGH); digitalWrite(bin2_2, LOW);
  setMotorSpeed(fixedSpeed);
}
