#include <Stepper.h>

// Definition der Pins für den Schrittmotor
#define DIR_PIN 3
#define PUL_PIN 2

// Definition der Pins für die Taster
#define BUTTON_PIN_1 7
#define BUTTON_PIN_2 8
#define BUTTON_PIN_3 4
#define BUTTON_PIN_4 4

// Anzahl der Mikroschritte pro Umdrehung (um Faktor 4 erhöht)
#define MICROSTEPS 25600

// Schrittmotor-Objekt erstellen
Stepper motor(MICROSTEPS, DIR_PIN, PUL_PIN);

// Variable zur Speicherung des vorherigen Zustands der Taster
int prevButtonState1 = HIGH;
int prevButtonState2 = HIGH;
int prevButtonState3 = HIGH;
int prevButtonState4 = HIGH;
int stepSize = 0;

void setup() {
  // Initialisierung der seriellen Kommunikation
  Serial.begin(9600);
  
  // Setze die Geschwindigkeit des Motors in Umdrehungen pro Minute (RPM)
  motor.setSpeed(50); // Hier kannst du die Geschwindigkeit ändern, wie du möchtest

  // Konfiguriere die Taster-Pins als Eingänge und aktiviere die internen Pull-up-Widerstände
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  
}

void loop() {
  // Aktueller Zustand der Taster
  int buttonState1 = digitalRead(BUTTON_PIN_1);
  int buttonState2 = digitalRead(BUTTON_PIN_2);
  int buttonState3 = digitalRead(BUTTON_PIN_3);
 // int stepSize = 0;

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
    }
  }
 
  // Speichere den aktuellen Zustand der Taster für den nächsten Durchlauf
  prevButtonState1 = buttonState1;
  prevButtonState2 = buttonState2;
  prevButtonState3 = buttonState3;
}