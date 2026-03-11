//
//  main.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/8/26.
//

#include "StackLabs.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
    Serial.begin(9600);
    StackLabs::LCD::prepareDisplay(lcd);
    StackLabs::LCD::createCharacters(lcd);
    StackLabs::ColorSensor::setup();
}

void loop() {
    lcd.clear();
    StackLabs::ColorSensor::readColor();
    lcd.setCursor(0, 0); // col, row for some reason
    lcd.print("R: ");
    lcd.print(StackLabs::ColorSensor::getRed());
    lcd.setCursor(10, 0);
    lcd.print("G: ");
    lcd.print(StackLabs::ColorSensor::getGreen());
    lcd.setCursor(0, 1);
    lcd.print("B: ");
    lcd.print(StackLabs::ColorSensor::getBlue());
    lcd.setCursor(10, 1);
    lcd.print("C: ");
    lcd.print(StackLabs::ColorSensor::getClear());
    lcd.setCursor(1, 3);
    lcd.print("CHP: ");
    switch (StackLabs::ColorSensor::calculateMatch()) {
    case 1:
        lcd.print("BLACK");
        break;
    case 2:
        lcd.print("RED");
        break;
    case 3:
        lcd.print("GREEN");
        break;
    case 4:
        lcd.print("BLUE");
        break;
    default:
        lcd.print("NONE");
        break;
    }
    delay(250);
}
