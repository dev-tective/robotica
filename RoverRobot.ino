#include "Rover_module.h"
#include <SoftwareSerial.h>

SoftwareSerial HM10(A2, A3);  // RX = A2, TX = A3

/*
int leftEn = 4;
int leftPWM = 5;
int rightPWM = 6;
int rightEn = 7;
*/

int leftEn1 = 5;
int leftEn2 = 4;
int leftPWM = 3;
int rightPWM = 9;
int rightEn1 = 7;
int rightEn2 = 8;

int ledPin = 12;
int ledVcc = 13;
Rover rover(leftPWM, leftEn1, leftEn2, rightPWM, rightEn1, rightEn2);

String str_btData = "";

byte modo_de_trabajo = 0;  //0=VEHICULO CONTROL | 1=SEGUIDOR DE LINEA

void setup() {
  //vehicle.Direction(LOW, LOW);
  pinMode(ledPin, OUTPUT);
  pinMode(ledVcc, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledVcc, HIGH);

  Serial.begin(9600);  //Puerto serial
  HM10.begin(9600);    //Bluetooth
}

void loop() {
  monitorSerial();
  if (modo_de_trabajo == 1) { seguidorDeLinea(); }
}

void monitorSerial() {
  if (HM10.available()) {
    char c = HM10.read();
    //Si el caracter es salto de linea o nulo es un final de mensaje
    if (c == '\n' || c == NULL || c == '\r') {
      if (str_btData.equals("LED_ON")) {
        encender(ledPin);
        Serial.print("Robot:");
        Serial.println(str_btData);
      }
      if (str_btData.equals("LED_OFF")) {
        apagar(ledPin);
        Serial.print("Robot:");
        Serial.println(str_btData);
      }
      if (modo_de_trabajo == 0) {  //control remoto
        if (str_btData.length() >= 5) {
          if (str_btData.substring(0, 5).equals("SPEED")) {
            //Leer la data y limpiar
            int l_speed, r_speed;
            int index = str_btData.indexOf(',');
            if (index > -1) {
              l_speed = str_btData.substring(6, index).toInt();
              r_speed = str_btData.substring(index + 1).toInt();  //, str_btData.length()).toInt();
              rover.Speed(l_speed, r_speed);
            }
            Serial.println(String() + l_speed + " - " + r_speed);
          }
        }
      }

      str_btData = "";  //Limpiamos cualquier mensaje recibido.
    } else {
      str_btData += c;
      //Serial.print("caracter recibido: ");
      //Serial.println(str_btData);
    }
  }
}

void seguidorDeLinea() {
}

void encender(int pin) {
  digitalWrite(pin, HIGH);
}

void apagar(int pin) {
  digitalWrite(pin, LOW);
}

void setPWM(int pin, int val) {
  analogWrite(pin, val);
}
