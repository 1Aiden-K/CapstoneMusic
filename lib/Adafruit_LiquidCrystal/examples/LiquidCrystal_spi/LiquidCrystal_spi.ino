/***************************************************
  A fork of Adafruit LiquidCrystal library that support 
  i2c / SPI character LCD backpack.
 ****************************************************/

#include "Adafruit_LiquidCrystal.h"

// TO connect via SPI. Data pin is A5, Clock is A3 and Latch is A2
LiquidCrystal lcd(A5, A3, A2);

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
