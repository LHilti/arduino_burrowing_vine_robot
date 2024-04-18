#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

const int chipSelect = 4; // Change this to your SD Card's Chip Select pin
const int inputPinx = 2; // Change this to your first input pin x
const int inputPiny = 3; // Change this to your second input pin y
const int inputPina = 21; // Change this to your third input pin a
const int inputPinb = 20; // Change this to your fourth input pin b
const int inputPinc = 19; // Change this to your fifth input pin c
const int inputPinr = 18; // Change this to your fifth input pin button r

volatile unsigned long startTime;
volatile bool writeFlag = false;
volatile char triggeredBy = ' ';

float sensorxadj = 2.57; //determined by Testing
float sensoryadj = 2.34; //determined by Testing

double VolumeX;
double VolumeY;

// Initialize the LCD screen
const int rs = 7, en = 5, d4 = 16, d5 = 15, d6 = 17, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(inputPinx, INPUT);
  pinMode(inputPiny, INPUT);
  pinMode(inputPina, INPUT_PULLUP);
  pinMode(inputPinb, INPUT_PULLUP);
  pinMode(inputPinc, INPUT_PULLUP);
  pinMode(inputPinr, INPUT_PULLUP);

  Serial.begin(9600);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Card initialized.");
  
  attachInterrupt(digitalPinToInterrupt(inputPinx), trigger1, RISING);
  attachInterrupt(digitalPinToInterrupt(inputPiny), trigger2, RISING);
  attachInterrupt(digitalPinToInterrupt(inputPina), trigger3, FALLING);
  attachInterrupt(digitalPinToInterrupt(inputPinb), trigger4, FALLING);
  attachInterrupt(digitalPinToInterrupt(inputPinc), trigger5, FALLING);
  attachInterrupt(digitalPinToInterrupt(inputPinr), trigger6, FALLING);

  startTime = millis();

  // Initialize the LCD screen
  lcd.begin(16, 2);
  lcd.print("Ready");
}

void loop() {
  if (writeFlag) {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    
    if (dataFile) {
      dataFile.print(millis() - startTime);
      dataFile.print(", ");
      dataFile.print(VolumeX);
      dataFile.print(", ");
      dataFile.print(VolumeY);
      dataFile.print(", ");
      dataFile.println(triggeredBy);
      
      dataFile.close();
      Serial.print(triggeredBy);
      Serial.print(", ");
      Serial.print(VolumeX);
      Serial.print(", ");
      Serial.println(VolumeY);
    } else {
      Serial.println("Error opening datalog.txt");
    }
    
    writeFlag = false;
  }

  // Display the states of the first two inputs on the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Input X: ");
  lcd.print(VolumeX);
  lcd.setCursor(0, 1);
  lcd.print("Input Y: ");
  lcd.print(VolumeY);
}

void trigger1() {
  writeFlag = true;
  triggeredBy = 'x';
  VolumeX= VolumeX + sensorxadj;
}

void trigger2() {
  writeFlag = true;
  triggeredBy = 'y';
  VolumeY= VolumeY + sensoryadj;
}

void trigger3() { 
  writeFlag = true;
  triggeredBy = 'a';
}

void trigger4() {
  writeFlag = true;
  triggeredBy = 'b';
}

void trigger5() {
  writeFlag = true;
  triggeredBy = 'c';
}

void trigger6() {
  writeFlag = true;
  triggeredBy = 'r';
  VolumeX=0;
  VolumeY=0;
}
