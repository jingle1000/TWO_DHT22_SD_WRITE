#include <SPI.h>
#include <SD.h>
#include <DHT_U.h>
#include <DHT.h>


#define DHTPIN A0
#define DHTPIN2 A1
#define DHTTYPE DHT22
#define BUTTONPIN 6
DHT dht1(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
File myFile;

const int ledDone = 3;
const int ledRec = 2;
const int ledBad= 5;
float temp1;
float temp2;
float timeCounter = 0;
const float recInterval = 60000;
const int blinkTime = 1000;
int buttonState = 0;

void setup() {
  pinMode(BUTTONPIN, INPUT);
  pinMode(ledDone, OUTPUT);
  pinMode(ledRec, OUTPUT);
  pinMode(ledBad, OUTPUT);
  Serial.begin(9600);
  Serial.println("Initializing SD");
  if (!SD.begin(4)) {
    Serial.println("Initialization Failed...");
    while(true) {
      digitalWrite(ledBad, HIGH);
      delay(blinkTime);
      digitalWrite(ledBad, LOW);
      delay(blinkTime);
    }
  }
  Serial.println("Initialization Done");
  Serial.println("Begin Writing Data");
  myFile = SD.open("DHT22.txt", FILE_WRITE);
  lineSep();
  
}

void loop() {
    buttonState = digitalRead(BUTTONPIN);
    temp1 = sensorOneTemp();
    temp2 = sensorTwoTemp();
    printTemp(temp1, temp2, timeCounter);
    logTemp(temp1, temp2, timeCounter);
    timeCounter += 1;
    checkEnd(buttonState);
    delay(recInterval - blinkTime);
}

float sensorOneTemp() {
  float temp = dht1.readTemperature();
  while(isnan(temp)) {
    digitalWrite(ledBad, HIGH);
    temp = dht1.readTemperature();
  }
  digitalWrite(ledBad, LOW);
  return temp;
}

float sensorTwoTemp() {
  float temp = dht2.readTemperature();
  while(isnan(temp)) {
    digitalWrite(ledBad, HIGH);
    temp = dht1.readTemperature();
  }
  digitalWrite(ledBad, LOW);
  return temp;
}

void lineSep() {
  Serial.println("----------------------");
}

void printTemp(float temp1, float temp2, int timeCounter) {
    Serial.print("Time: ");
    Serial.println(timeCounter);
    Serial.print("Sensor1: ");
    Serial.println(temp1);
    Serial.print("Sensor2: ");
    Serial.println(temp2);
    lineSep();
}

void checkEnd(int buttonState) {
  if (buttonState == 1) {
    Serial.println("Done...");
    myFile.close();
    digitalWrite(ledDone, HIGH);
    while(1);
  }
  else {
    //continue
  }
}

void logTemp(float temp1, float temp2, int timeCounter) {
  String toPrint = String(timeCounter) + ", " + String(temp1) + ", " + String(temp2);
  if (myFile) {
    myFile.println(toPrint);
    blinkFunction();
  }
  else {
    Serial.println("File not found...");
  }
  Serial.println("Data printed to SD card");
}

void blinkFunction() {
  digitalWrite(ledRec, HIGH);
  delay(blinkTime);
  digitalWrite(ledRec, LOW);
}
