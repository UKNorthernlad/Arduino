/*
 Example Arduino sketch for
 SainSmart IIC/I2C/TWI Serial 2004 20x4 LCD Module Shield For Arduino UNO MEGA R3.
 http://www.sainsmart.com/sainsmart-iic-i2c-twi-serial-2004-20x4-lcd-module-shield-for-arduino-uno-mega-r3.html
 
 This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
 https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/NewliquidCrystal_1.3.4.zip
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR      0x3F // I2C address.
#define BACKLIGHT_PIN 3
#define En_pin        2
#define Rw_pin        1
#define Rs_pin        0
#define D4_pin        4
#define D5_pin        5
#define D6_pin        6
#define D7_pin        7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);

void setup(){

  lcd.begin(20,4);
  //lcd.home();
  //lcd.setCursor(4,1);
  //lcd.clear();
  //delay(1000); 
  //lcd.setBacklight(HIGH);
  //lcd.setBacklight(LOW);
  lcd.print("Arduino I2C LCD.");
};



void loop(){

}


