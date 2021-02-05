/*
 * BasicLCDClock.ino
 * Pretty much the RTC example but printing to LCD
 * instead of Serial.
 */

#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h> //for temperature/humidity sensor
#include <DHT.h>
#include <DHT_U.h>
#include "digits.h"

#define DHTPIN 6
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
sensors_event_t event;

// initialize the LiquidCrystal library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, p4 = 5, p5 = 4, p6 = 3, p7 = 2;
LiquidCrystal lcd(rs, en, p4, p5, p6, p7);

int tZoneAdj = -5; //eastern timezone
int tZoneAdjHour = 0;

void setup()  {
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  lcd.begin(16,2);
  setupDigits();
  dht.begin();
  Serial.begin(9600);
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0,0);
  if (timeStatus() == timeSet) {
    tZoneAdjHour = hour() + tZoneAdj;
    if(tZoneAdjHour < 0)
      tZoneAdjHour += 24;
    digitalClockDisplay();

    dht.temperature().getEvent(&event);
    lcd.setCursor(9,1);
    lcd.print(int(event.temperature*1.8+32));
    lcd.print((char)223); //degree symbol
    lcd.print(" ");
    lcd.print(int(event.relative_humidity));
    lcd.print("%");
    
  } else {
    lcd.print("Error, set time");
    while(1){};
  }
  delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  /*
  lcd.print(hour());
  printDigits(minute());
  printDigits(second());
  lcd.setCursor(0,1);
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year()); 
  */

  
  
  printDigits(tZoneAdjHour, 0);
  if(second() %2)
    printColon(2);
  else
    printBlank(2);
  printDigits(minute(), 3);

  printDate(weekday(), month(), day(), 8,0);

  Serial.print(tZoneAdjHour);
  Serial.print(":");
  Serial.println(minute());
}

void setupDigits(){
  //sends the custom character data to the lcd
  lcd.createChar(0, d1);
  lcd.createChar(1, d2);
  lcd.createChar(2, d3);
  lcd.createChar(3, d4);
  lcd.createChar(4, d5);
  lcd.createChar(5, d6);
  lcd.createChar(6, d7);
  lcd.createChar(7, d8);
}

void printDigits(int digits, int pos){
  // prints the big fancy digits

  int ones = digits % 10;
  int tens = digits / 10;
  int digit = tens;
  

  switch(tens){
    case 0: lcd.setCursor(pos, 0); lcd.write(byte(3)); lcd.setCursor(pos, 1); lcd.write(byte(0)); break; 
    case 1: lcd.setCursor(pos, 0); lcd.write(byte(1)); lcd.setCursor(pos, 1); lcd.write(byte(1)); break; 
    case 2: lcd.setCursor(pos, 0); lcd.write(byte(2)); lcd.setCursor(pos, 1); lcd.print("L"); break; 
    case 3: lcd.setCursor(pos, 0); lcd.write(byte(2)); lcd.setCursor(pos, 1); lcd.write(byte(7)); break; 
    case 4: lcd.setCursor(pos, 0); lcd.write(byte(0)); lcd.setCursor(pos, 1); lcd.write(byte(1)); break; 
    case 5: lcd.setCursor(pos, 0); lcd.write(byte(4)); lcd.setCursor(pos, 1); lcd.write(byte(7)); break; 
    case 6: lcd.setCursor(pos, 0); lcd.write(byte(4)); lcd.setCursor(pos, 1); lcd.write(byte(0)); break; 
    case 7: lcd.setCursor(pos, 0); lcd.write(byte(6)); lcd.setCursor(pos, 1); lcd.write(byte(1)); break; 
    case 8: lcd.setCursor(pos, 0); lcd.write(byte(5)); lcd.setCursor(pos, 1); lcd.write(byte(5)); break; 
    case 9: lcd.setCursor(pos, 0); lcd.write(byte(5)); lcd.setCursor(pos, 1); lcd.write(byte(7)); break; 
  }

  switch(ones){
    case 0: lcd.setCursor(pos+1, 0); lcd.write(byte(3)); lcd.setCursor(pos+1, 1); lcd.write(byte(0)); break; 
    case 1: lcd.setCursor(pos+1, 0); lcd.write(byte(1)); lcd.setCursor(pos+1, 1); lcd.write(byte(1)); break; 
    case 2: lcd.setCursor(pos+1, 0); lcd.write(byte(2)); lcd.setCursor(pos+1, 1); lcd.print("L"); break; 
    case 3: lcd.setCursor(pos+1, 0); lcd.write(byte(2)); lcd.setCursor(pos+1, 1); lcd.write(byte(7)); break; 
    case 4: lcd.setCursor(pos+1, 0); lcd.write(byte(0)); lcd.setCursor(pos+1, 1); lcd.write(byte(1)); break; 
    case 5: lcd.setCursor(pos+1, 0); lcd.write(byte(4)); lcd.setCursor(pos+1, 1); lcd.write(byte(7)); break; 
    case 6: lcd.setCursor(pos+1, 0); lcd.write(byte(4)); lcd.setCursor(pos+1, 1); lcd.write(byte(0)); break; 
    case 7: lcd.setCursor(pos+1, 0); lcd.write(byte(6)); lcd.setCursor(pos+1, 1); lcd.write(byte(1)); break; 
    case 8: lcd.setCursor(pos+1, 0); lcd.write(byte(5)); lcd.setCursor(pos+1, 1); lcd.write(byte(5)); break; 
    case 9: lcd.setCursor(pos+1, 0); lcd.write(byte(5)); lcd.setCursor(pos+1, 1); lcd.write(byte(7)); break; 
  }
}

void printColon(int pos){
  // a big fancy colon
   lcd.setCursor(pos, 0);
   lcd.write(0b10100101);
   lcd.setCursor(pos, 1);
   lcd.write(0b10100101);
  }
void printBlank(int pos){
  // lack of a big fancy colon
   lcd.setCursor(pos, 0);
   lcd.print(" ");
   lcd.setCursor(pos, 1);
   lcd.print(" ");
  }

void printDate(int weekdayVal, int monthVal, int dayVal, int xpos, int ypos){
  lcd.setCursor(xpos,ypos);
  String weekDays[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

  lcd.print(weekDays[weekdayVal]);
  lcd.print(" ");

  if(monthVal < 10)
    lcd.print("0");
  lcd.print(monthVal);
  
  lcd.print("/");
  
  if(dayVal < 10)
    lcd.print("0");
  lcd.print(dayVal);
}
