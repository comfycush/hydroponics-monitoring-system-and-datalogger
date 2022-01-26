#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Sd2Card card;
SdVolume volume;
SdFile root;

//LCD RS pin to digital pin 10
//LCD Enable pin to digital pin 9
//LCD D4 pin to digital pin 8
//LCD D5 pin to digital pin 7
//LCD D6 pin to digital pin 5
//LCD D7 pin to digital pin 3
//10K resistor:
//ends to +5V and ground
//wiper to LCD VO pin (pin 3)
LiquidCrystal lcd(10, 9, 8, 7, 5, 3);
const int chipSelect = 4;

#define ONE_WIRE_BUS 6


OneWire oneWire(ONE_WIRE_BUS);


DallasTemperature sensors(&oneWire);
int n = 0;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  
 if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // Start up the library
  sensors.begin();


  // Setup Serial
  while (!Serial);
  Serial.begin(9600);
  // Open serial communications and wait for port to open:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
    Serial.println("card initialized.");
  }
  File dataFile = SD.open("data.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println("Tanggal, Waktu, Kelembaban Media, Suhu Nutrisi");
    dataFile.close();
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature for Device 1 is: ");
  Serial.println(sensors.getTempCByIndex(0));

  Serial.print("Moisture Sensor Value:");
  Serial.println(analogRead(A0));  
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  lcd.setCursor(0,0);
  lcd.print("S.M:");
  lcd.setCursor(0,1);
  lcd.print("T air:");
  lcd.setCursor(7, 0);
  lcd.print(analogRead(A0));
  lcd.setCursor(7, 1);
  lcd.print(sensors.getTempCByIndex(0));

 
  delay(6000);
  n = n + 1;
  if (n == 10) {
    File dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
      DateTime now = RTC.now();
      dataFile.print(now.year(), DEC);
      dataFile.print('/');
      dataFile.print(now.month(), DEC);
      dataFile.print('/');
      dataFile.print(now.day(), DEC);
      dataFile.print(" (");
      dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
      dataFile.print(") ");
      dataFile.print(", ");
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC );
      dataFile.print(", ");
      dataFile.print(analogRead(A0));
      dataFile.print(" , ");
      dataFile.println(sensors.getTempCByIndex(0));
      dataFile.close();
      n = 0;
    }
  }


 
 DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.println();
}
