//
//  main.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/8/26.
//

#include "StackLabs.h"
#include "LCD.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
    StackLabs::LCD::prepareDisplay(lcd);
    StackLabs::LCD::createCharacters(lcd);

    lcd.setCursor(3, 0);
    lcd.print("CHIP INVENTORY");

    lcd.setCursor(1, 1);
    lcd.write((byte)3);
    lcd.setCursor(2, 1);
    lcd.print("  5: 00");
    lcd.setCursor(11, 1);
    lcd.write((byte)0);
    lcd.setCursor(12, 1);
    lcd.print(" 10: 00");

    lcd.setCursor(1, 2);
    lcd.write((byte)0);
    lcd.setCursor(2, 2);
    lcd.print(" 25: 00");
    lcd.setCursor(11, 2);
    lcd.write((byte)0);
    lcd.setCursor(12, 2);
    lcd.print(" 50: 00");

    lcd.setCursor(0, 3);
    lcd.print("[       MENU       ]");
}

void loop() {

}


