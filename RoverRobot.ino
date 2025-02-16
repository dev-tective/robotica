#include "Rover_module.h"
#include <SoftwareSerial.h>

SoftwareSerial HM10(A2, A3);

int leftEn1 = 5;
int leftEn2 = 4;
int leftPWM = 3;
int rightPWM = 9;
int rightEn1 = 7;
int rightEn2 = 8;

Rover rover(leftPWM, leftEn1, leftEn2, rightPWM, rightEn1, rightEn2);

String str_btData = "";

void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
}

void loop() {
  monitorSerial();
}

void monitorSerial() {
  if (HM10.available()) {
    char c = HM10.read();
    
    if (c == '\n' || c == '\r') {
      detectCommand(str_btData);
      str_btData = "";
      return;
    }

    str_btData += c;
  }
}

void detectCommand(String message) {
  int separatorIndex = message.indexOf(":");

  if (separatorIndex != -1) {
    String command = message.substring(0, separatorIndex);
    String parameters = message.substring(separatorIndex + 1);

    int separatorParameters = parameters.indexOf(",");

    if (separatorParameters != -1) {
      int left = parameters.substring(0, separatorParameters).toInt();
      int right = parameters.substring(separatorParameters + 1).toInt();

      if (command.equalsIgnoreCase("SPEED")) {
        runCar(left, right);
        return;
      }
    }

    if (command.equalsIgnoreCase("SPIN")) {
      spin(parameters);
      return;
    }
  }

  Serial.println("Error: Comando no válido");
}

void runCar(int left, int right) {
  rover.Speed(left, right);
}

void spin(String side) {
  if (side.equalsIgnoreCase("RIGHT")) {
    rover.Speed(255, 0);
    return;
  }

  if (side.equalsIgnoreCase("LEFT")) {
    rover.Speed(0, 255);
    return;
  } 

  Serial.println("Error: Comando de giro no válido");
}
