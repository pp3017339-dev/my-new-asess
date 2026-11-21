#include <LiquidCrystal.h>

// Initialize the library with the interface pins: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Set up the LCD's number of columns and rows (16 columns, 2 rows)
  lcd.begin(16, 2);

  // Print Name on the first line (Column 0, Row 0)
  lcd.setCursor(0, 0);
  lcd.print("Name: Rahul");

  // Print USN on the second line (Column 0, Row 1)
  lcd.setCursor(0, 1);
  lcd.print("USN: 1RV23CS001");
}

void loop() {
  // Static text does not need to be repeated in the loop
}
