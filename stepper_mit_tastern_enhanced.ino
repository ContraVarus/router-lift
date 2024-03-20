#include <Adafruit_SSD1306.h>         //Displaylibrary
#include <Adafruit_GFX.h>             //Displaylibrary
#include <Adafruit_GrayOLED.h>        //Displaylibrary
#include <Adafruit_SPITFT.h>          //Displaylibrary
#include <Adafruit_SPITFT_Macros.h>   //Displaylibrary
#include <gfxfont.h>                  //Displaylibrary
#include <splash.h>                   //Displaylibrary
#include <Wire.h>                     //Library for I2C
#include <Stepper.h>                  //Library for Stepper

#define DIR_PIN 3                     // Definition der Pins für den Schrittmotor
#define PUL_PIN 2                     // Definition der Pins für den Schrittmotor

#define BUTTON_PIN_1 7                // Definition der Pins für die Taster
#define BUTTON_PIN_2 8                // Definition der Pins für die Taster
#define BUTTON_PIN_3 4                // Definition der Pins für die Taster

#define BUTTON_PIN_4 12               // Definition für Endstoppschalter PIN
#define BUTTON_PIN_5 13               // Definition für Endstoppschalter PIN

#define SCREEN_WIDTH 128              //Definition für OLED Display
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C           //I2C-Adress of Display

#define MICROSTEPS 3200               // Anzahl der Mikroschritte pro Umdrehung

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   //Display Objekt erstellen

Stepper motor(MICROSTEPS, DIR_PIN, PUL_PIN);    // Schrittmotor-Objekt erstellen

int prevButtonState1 = HIGH;
int prevButtonState2 = HIGH;
int prevButtonState3 = LOW;           // Variable zur Speicherung des vorherigen Zustands der Taster
int prevButtonState4 = HIGH;
int prevButtonState5 = HIGH;
int stepSize = 0;
bool SpeedMode = false;
unsigned long Startzeit;
unsigned long GesicherteZeit = 0;

void setup() {
  // Initialisierung der seriellen Kommunikation
  Serial.begin(9600);
   
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);                       // Konfiguriere die Taster-Pins als Eingänge und aktiviere die internen Pull-up-Widerstände
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);
  pinMode(BUTTON_PIN_5, INPUT_PULLUP);

  motor.setSpeed(40);                                       //Anfangsgeschwindigkeit nach Programmstart auf langsam festsetzen

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  display.clearDisplay();
  display.print("Wullis Woodwork");
  display.display();
}
}
void showDisplay() {
  display.clearDisplay();
  display.setCursor(0,20);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print("Hoehe: ");
  display.print(stepSize * 0.01953125, 2);
  display.println("mm");
  display.display();
}
void loop() {
 int buttonState1 = digitalRead(BUTTON_PIN_1);              // Aktueller Zustand der Taster
 int buttonState2 = digitalRead(BUTTON_PIN_2);
 int buttonState3 = digitalRead(BUTTON_PIN_3);              //Wert von 1 kommt zurück wenn Button auf HIGH steht
 int buttonState4 = digitalRead(BUTTON_PIN_4);              //Wert von 1 kommt wenn Mikroschalter nicht gedrückt ist, bei Druck kommt Wert 1
 int buttonState5 = digitalRead(BUTTON_PIN_5);
 
 digitalRead(buttonState4);
  if (buttonState3 == LOW) {
  Startzeit = millis();
  if (Startzeit - GesicherteZeit > 50)
  {
  SpeedMode = !SpeedMode;
  }
  GesicherteZeit = Startzeit;
  digitalWrite(buttonState3, SpeedMode);
  if (SpeedMode == true) {
  motor.setSpeed(800);}                                      // Motorgeschwindigkeit für Fast-Mode (SpeedMode=1)
  else {
  motor.setSpeed(100);  }                                     // Motorgeschwindigkeit für Slow-Mode (SpeedMode=0)
}
 if (buttonState1 == LOW && prevButtonState1 == HIGH) {     // Wenn der erste Taster gedrückt wird und der vorherige Zustand nicht gedrückt war
 while (digitalRead(BUTTON_PIN_1) == LOW) {                 // Solange der erste Taster gedrückt ist
 motor.step(200);                                           // Drehung um 200 Mikrostufen im Uhrzeigersinn
 ++stepSize;                                                // Inkrementiere die Schrittgröße
 delay(10);                                                 // Kurze Verzögerung für eine angemessene Geschwindigkeit
 showDisplay();
 }
}
 if (buttonState2 == LOW && prevButtonState2 == HIGH) {    // Wenn der zweite Taster gedrückt wird und der vorherige Zustand nicht gedrückt war
 while (digitalRead(BUTTON_PIN_2) == LOW) {                // Solange der zweite Taster gedrückt ist
 motor.step(-200);                                         // Drehung um 200 Mikrostufen gegen den Uhrzeigersinn
 --stepSize;                                               // Dekrementiere die Schrittgröße
 delay(10);                                                // Kurze Verzögerung für eine angemessene Geschwindigkeit
 showDisplay();  
 }
}
prevButtonState1 = buttonState1;
prevButtonState2 = buttonState2;                            // Speichere den aktuellen Zustand der Taster für den nächsten Durchlauf
prevButtonState3 = buttonState3;
}
