/***************************************************
  A fork of Adafruit LiquidCrystal library that support
  i2c / SPI character LCD backpack.
 ****************************************************/

#include "Adafruit_LiquidCrystal.h"

// TO connect via I2C. Data pin is D0, Clock is D1
// By default, no jumpers are soldered, giving an address of 0.
LiquidCrystal lcd(0);

void setup() {
  // set up the LCD's number of rows and columns:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);

  lcd.setBacklight(HIGH);
  delay(500);
  lcd.setBacklight(LOW);
  delay(500);
}
