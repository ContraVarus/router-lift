#include <Adafruit_SSD1306.h>

#include <Adafruit_GFX.h>

#include <Adafruit_GrayOLED.h>

#include <Adafruit_SPITFT.h>

#include <Adafruit_SPITFT_Macros.h>

#include <gfxfont.h>

#include <splash.h>

#include <Stepper.h>

#define DIR_PIN 6 // Definition der Pins für den Schrittmotor
#define PUL_PIN 5

#define BUTTON_PIN_1 7 // Definition der Pins für die Taster
#define BUTTON_PIN_2 8
#define BUTTON_PIN_3 4

#define EndBUTTON_PIN_2 11 // Definition der Pins für die beiden Endschalter
#define EndBUTTON_PIN_1 10

#define WLS_PIN_1 12
#define WLS_SENSOR1 9

#define MICROSTEPS 1600 //Anzahl der Mikroschritte des Motors pro Umdrehung

Stepper motor(MICROSTEPS, DIR_PIN, PUL_PIN); // Schrittmotor-Objekt erstellen

#define SCREEN_WIDTH 128 //OLED Definition
#define SCREEN_HEIGHT 64
#define OLED_RESET - 1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET); //Display Objekt erstellen

int prevButtonState1 = HIGH; // Variable zur Speicherung des vorherigen Zustands der Taster
int prevButtonState2 = HIGH;
int prevButtonState3 = HIGH;
int stepSize = 0;
bool SpeedMode = false;
unsigned long Startzeit;
unsigned long GesicherteZeit = 0;
int prevEndButtonState2 = HIGH;
int prevEndButtonState1 = HIGH;
int prevWLS_ButtonState1 = HIGH;
int WLS_SENSOR = HIGH;

void setup() {

  Serial.begin(9600); // Initialisierung der seriellen Kommunikation

  motor.setSpeed(40); //Motor mit langsamem Speed initialisieren

  pinMode(BUTTON_PIN_1, INPUT_PULLUP); // Konfiguriere die Taster-Pins als Eingänge und aktiviere die internen Pull-up-Widerstände
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(EndBUTTON_PIN_2, INPUT_PULLUP); //Endschalter
  pinMode(EndBUTTON_PIN_1, INPUT_PULLUP);
  pinMode(WLS_PIN_1, INPUT_PULLUP);
  pinMode(WLS_SENSOR1, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); //Nach Start mit leeren Display starten
  display.display();

}

void showDisplay() {
  display.clearDisplay();
  display.setCursor(10, 10);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println("Height: ");
  display.setCursor(10, 40);
  display.print(stepSize * 0.15625, 1);
  display.print(" mm");
  display.display();
}
void loop() {
  // Aktueller Zustand der Taster
  int buttonState1 = digitalRead(BUTTON_PIN_1);
  int buttonState2 = digitalRead(BUTTON_PIN_2);
  int buttonState3 = digitalRead(BUTTON_PIN_3); //Wert von 1 kommt zurück wenn Button auf HIGH steht
  int EndButtonState2 = digitalRead(EndBUTTON_PIN_2);
  int EndButtonState1 = digitalRead(EndBUTTON_PIN_1);
  int WLS_ButtonState1 = digitalRead(WLS_PIN_1);
  int WLS_SENSOR = digitalRead(WLS_SENSOR1);

  if (buttonState3 == LOW) { // Wenn der dritte Taster gedrückt wird, Speedchange
    Startzeit = millis();
    if (Startzeit - GesicherteZeit > 50) {
      SpeedMode = !SpeedMode;
    }
    GesicherteZeit = Startzeit;
    digitalWrite(buttonState3, SpeedMode);

    if (SpeedMode == true) {
      motor.setSpeed(320); // Motorgeschwindigkeit
    } else {
      motor.setSpeed(40);
    }
  }
  if (buttonState1 == LOW && prevButtonState1 == HIGH && EndButtonState2 == HIGH) { // Wenn der erste Taster gedrückt wird und der vorherige Zustand nicht gedrückt war
    while (digitalRead(BUTTON_PIN_1) == LOW && (digitalRead(EndBUTTON_PIN_2) == HIGH)) { // Solange der erste Taster gedrückt ist und der Endschalter nicht ausgelöst wurde
      motor.step(100); // Drehung um 200 Mikrostufen im Uhrzeigersinn
      ++stepSize; // Inkrementiere die Schrittgröße
      delay(10); // Kurze Verzögerung für eine angemessene Geschwindigkeit
      showDisplay(); // Ausgabe der Schrittgröße in Millimeter über OLED Display
    }
  }
  if (buttonState2 == LOW && prevButtonState2 == HIGH && EndButtonState1 == HIGH) { // Zweite Taster gedrückt und der vorherige Zustand nicht gedrückt war und der Endestopp nicht ausgelöst hat
    while (digitalRead(BUTTON_PIN_2) == LOW && (digitalRead(EndBUTTON_PIN_1) == HIGH)) { // Solange der zweite Taster gedrückt ist und Endschalter nicht betätigt
      motor.step(-100); // Drehung um x Mikrostufen gegen den Uhrzeigersinn
      --stepSize; // Dekrementiere die Schrittgröße
      delay(10); // Kurze Verzögerung für eine angemessene Geschwindigkeit
      showDisplay(); // Ausgabe der Schrittgröße in Millimeter über OLED Display
    }
  }
  if (WLS_ButtonState1 == LOW && prevWLS_ButtonState1 == HIGH) { //Motor dreht solange nach oben bis der Werkzeuglängenmesser auslöst
    display.clearDisplay();
    display.setCursor(30, 8);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Werkzeug");
    display.setCursor(20, 24);
    display.println("Laengenmessung");
    display.setCursor(28, 40);
    display.println("Gestartet");
    display.display();
    while (digitalRead(WLS_SENSOR1) == HIGH && (digitalRead(EndBUTTON_PIN_1) == HIGH)) {
      motor.setSpeed(80);
      motor.step(50);
      delay(10);
    }
  }
  if (WLS_ButtonState1 == HIGH && WLS_SENSOR == LOW) {
    {
      display.clearDisplay();
      display.setCursor(30, 8);
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.println("Werkzeug");
      display.setCursor(20, 24);
      display.println("Laengenmessung");
      display.setCursor(22, 40);
      display.println("Abgeschlossen");
      display.display();
    }
  }

  prevButtonState1 = buttonState1;
  prevButtonState2 = buttonState2;
  prevButtonState3 = buttonState3; // Speichere den aktuellen Zustand der Taster für den nächsten Durchlauf
  prevEndButtonState1 = EndButtonState1;
  prevEndButtonState2 = EndButtonState2;
  prevWLS_ButtonState1 = WLS_ButtonState1;
}
