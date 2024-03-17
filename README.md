# router-lift
Arduino Sketch for a fully digitalized router-lift. 

Currently used Hardware:
Arduino Uno
Microstep Driver TB6600
OLED Display 0,9''
2 Buttons for move up and downwards
1 Button to select speed of lift (not router)

Display is connected via I2C. Buttons are just simple added by connecting GND to one of the digital inputs

What is already working:
Two buttons for up and downward of router are working. As long as you press adjustment is performed. Once button isn't pressed anymore motor will stop. This setup is designed for a SFU1605. On the serial console the output shows the difference from start of the sketch until now in Millimeters with 2 Decimal.


Work in Progress:
Setting german fonts for display and adjust height
Create end-buttons which stops motor directly

