#include <ArduinoBLE.h>

// Motor control pins
#define ain1 14
#define ain2 12
#define bin1 26
#define bin2 27
#define pwmM 13
#define pwmM_2 25

#define ain1_2 32
#define ain2_2 33
#define bin1_2 18
#define bin2_2 19

const int fixedSpeed = 200;

// BLE service and characteristic
BLEService motorService("180A"); // Custom Motor Control Service UUID
BLEStringCharacteristic commandChar("2A57", BLEWrite, 20); // Custom Command Characteristic

void setMotorSpeed(int speed) {
  analogWrite(pwmM, speed);
  analogWrite(pwmM_2, speed);
}

void stopMotors() {
  digitalWrite(ain1, LOW); digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW); digitalWrite(bin2, LOW);
  digitalWrite(ain1_2, LOW); digitalWrite(ain2_2, LOW);
  digitalWrite(bin1_2, LOW); digitalWrite(bin2_2, LOW);
  setMotorSpeed(0);
}

void handleCommand(String direction) {
  direction.toLowerCase();
  if (direction == "forward") {
    digitalWrite(ain1, HIGH); digitalWrite(ain2, LOW);
    digitalWrite(bin1, LOW); digitalWrite(bin2, HIGH);
    digitalWrite(ain1_2, HIGH); digitalWrite(ain2_2, LOW);
    digitalWrite(bin1_2, HIGH); digitalWrite(bin2_2, LOW);
  } else if (direction == "backward") {
    digitalWrite(ain1, LOW); digitalWrite(ain2, HIGH);
    digitalWrite(bin1, HIGH); digitalWrite(bin2, LOW);
    digitalWrite(ain1_2, LOW); digitalWrite(ain2_2, HIGH);
    digitalWrite(bin1_2, LOW); digitalWrite(bin2_2, HIGH);
  } else if (direction == "right") {
    digitalWrite(ain1, HIGH); digitalWrite(ain2, LOW);
    digitalWrite(bin1, HIGH); digitalWrite(bin2, LOW);
    digitalWrite(ain1_2, LOW); digitalWrite(ain2_2, HIGH);
    digitalWrite(bin1_2, HIGH); digitalWrite(bin2_2, LOW);
  } else if (direction == "left") {
    digitalWrite(ain1, LOW); digitalWrite(ain2, HIGH);
    digitalWrite(bin1, LOW); digitalWrite(bin2, HIGH);
    digitalWrite(ain1_2, HIGH); digitalWrite(ain2_2, LOW);
    digitalWrite(bin1_2, LOW); digitalWrite(bin2_2, HIGH);
  } else {
    stopMotors();
    return;
  }
  setMotorSpeed(fixedSpeed);
}

void setup() {
  Serial.begin(115200);

  // Pin setup
  pinMode(ain1, OUTPUT); pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT); pinMode(bin2, OUTPUT);
  pinMode(pwmM, OUTPUT); pinMode(pwmM_2, OUTPUT);
  pinMode(ain1_2, OUTPUT); pinMode(ain2_2, OUTPUT);
  pinMode(bin1_2, OUTPUT); pinMode(bin2_2, OUTPUT);

  stopMotors();

  // BLE setup
  if (!BLE.begin()) {
    Serial.println("BLE init failed!");
    while (1);
  }

  BLE.setLocalName("ArduinoR4Motor");
  BLE.setAdvertisedService(motorService);
  motorService.addCharacteristic(commandChar);
  BLE.addService(motorService);
  commandChar.writeValue("stop");
  BLE.advertise();

  Serial.println("BLE Motor Control Ready");
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (commandChar.written()) {
        String cmd = commandChar.value();
        Serial.print("Received: ");
        Serial.println(cmd);
        handleCommand(cmd);
      }
    }

    Serial.println("Disconnected");
    stopMotors();
  }
}
