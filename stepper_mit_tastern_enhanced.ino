#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <splash.h>

#include <Wire.h>

#include <Stepper.h>

// Definition der Pins für den Schrittmotor
#define DIR_PIN 3
#define PUL_PIN 2

// Definition der Pins für die Taster
#define BUTTON_PIN_1 7
#define BUTTON_PIN_2 8
#define BUTTON_PIN_3 4

//Definition für OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//Display Objekt erstellen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Anzahl der Mikroschritte pro Umdrehung
#define MICROSTEPS 1600

// Schrittmotor-Objekt erstellen
Stepper motor(MICROSTEPS, DIR_PIN, PUL_PIN);

// Variable zur Speicherung des vorherigen Zustands der Taster
int prevButtonState1 = HIGH;
int prevButtonState2 = HIGH;
int prevButtonState3 = HIGH;
int stepSize = 0;
bool SpeedMode = true;
unsigned long Startzeit;
unsigned long GesicherteZeit = 0;

void setup() {
  // Initialisierung der seriellen Kommunikation
  Serial.begin(9600);
  
  // Konfiguriere die Taster-Pins als Eingänge und aktiviere die internen Pull-up-Widerstände
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever

}
}
void loop() {
  // Aktueller Zustand der Taster
  int buttonState1 = digitalRead(BUTTON_PIN_1);
  int buttonState2 = digitalRead(BUTTON_PIN_2);
  int buttonState3 = digitalRead(BUTTON_PIN_3); //Wert von 1 kommt zurück wenn Button auf HIGH steht
  
if (buttonState3 == LOW) {
  Startzeit = millis();
if (Startzeit - GesicherteZeit > 50)
{
  SpeedMode = !SpeedMode;
}
 GesicherteZeit = Startzeit;
 digitalWrite(buttonState3, SpeedMode);
 Serial.println(SpeedMode);

// Setze die Geschwindigkeit des Motors in Umdrehungen pro Minute (RPM)
if (SpeedMode == 1) {
    motor.setSpeed(160); // Hier kannst du die Geschwindigkeit ändern, wie du möchtest
}
else if (SpeedMode == 0) {
    motor.setSpeed(40);
}
}

  // Wenn der erste Taster gedrückt wird und der vorherige Zustand nicht gedrückt war
  if (buttonState1 == LOW && prevButtonState1 == HIGH) {
    // Solange der erste Taster gedrückt ist
    while (digitalRead(BUTTON_PIN_1) == LOW) {
      // Drehung um 100 Mikrostufen im Uhrzeigersinn
      motor.step(100);
      // Inkrementiere die Schrittgröße
      stepSize++;
      // Kurze Verzögerung für eine angemessene Geschwindigkeit
      delay(10);
        // Ausgabe der Schrittgröße in Millimeter über die serielle Konsole
     Serial.print("Step size in millimeters: ");
     Serial.println(stepSize * 0.01953125, 2);
     display.clearDisplay();
     display.setCursor(0,20);
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE);
     display.print("Hoehe: ");
     display.print(stepSize * 0.01953125, 2);
     display.println("mm");
     display.display();
    }
  }

  // Wenn der zweite Taster gedrückt wird und der vorherige Zustand nicht gedrückt war
  if (buttonState2 == LOW && prevButtonState2 == HIGH) {
    // Solange der zweite Taster gedrückt ist
    while (digitalRead(BUTTON_PIN_2) == LOW) {
      // Drehung um 100 Mikrostufen gegen den Uhrzeigersinn
      motor.step(-100);
      // Dekrementiere die Schrittgröße
      stepSize--;
      // Kurze Verzögerung für eine angemessene Geschwindigkeit
      delay(10);
     // Ausgabe der Schrittgröße in Millimeter über die serielle Konsole
     Serial.print("Step size in millimeters: ");
     Serial.println(stepSize * 0.01953125, 2);
     Serial.print("Step size in millimeters: ");
     Serial.println(stepSize * 0.01953125, 2);
     display.clearDisplay();
     display.setCursor(0,20);
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE);
     display.print("Hoehe: ");
     display.print(stepSize * 0.01953125, 2);
     display.println("mm");
     display.display();
    }
  }
 
  // Speichere den aktuellen Zustand der Taster für den nächsten Durchlauf
  prevButtonState1 = buttonState1;
  prevButtonState2 = buttonState2;
  prevButtonState3 = buttonState3;
}
