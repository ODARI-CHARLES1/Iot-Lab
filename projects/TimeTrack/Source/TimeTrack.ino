//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);
tmElements_t tm;

void setup() {
  Serial.begin(112500);
  Serial.print("Initialization....");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print("TIME");
}


void loop() {

  if (RTC.read(tm)) {
    lcd.setCursor(4, 1);
    if (tm.Hour < 10) {
      lcd.print("0");
      lcd.print(tm.Hour);
    } else {
      lcd.print(tm.Hour);
    }
    lcd.print(":");
    if (tm.Minute < 10) {
      lcd.print("0");
      lcd.print(tm.Minute);
    } else {
      lcd.print(tm.Minute);
    }
    lcd.print(":");
    if (tm.Second < 10) {
      lcd.print("0");
      lcd.print(tm.Second);
    } else {
      lcd.print(tm.Second);
    }
  }

    else {
      Serial.println("Error reading time.");
    }
    delay(1000);
  }
