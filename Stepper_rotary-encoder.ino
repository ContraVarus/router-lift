#include <AccelStepper.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <splash.h>

#include <EncoderStepCounter.h>

#define ENCODER_PIN1 2
#define ENCODER_INT1 digitalPinToInterrupt(ENCODER_PIN1)
#define ENCODER_PIN2 3
#define ENCODER_INT2 digitalPinToInterrupt(ENCODER_PIN2)

#define SCREEN_WIDTH 128 //OLED Definition
#define SCREEN_HEIGHT 64
#define OLED_RESET  4
#define OLED_MOSI 5  //Pin SDA on OLED
#define OLED_CLK  6  // Pin SCL on OLED
#define OLED_DC 7  
#define OLED_CS 8
#define SCREEN_ADDRESS 0x3D

const int dirPin = 10;
const int stepPin = 9;
int lastpos = 0;
signed long position = 0;
signed char pos = 0;
unsigned long lastMoveTime = 0;
unsigned long TimeOut = 2000;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS); //Display Objekt erstellen
EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2, FULL_STEP);
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  encoder.begin();
  attachInterrupt(ENCODER_INT1, interrupt, CHANGE);
  attachInterrupt(ENCODER_INT2, interrupt, CHANGE);

  stepper.setMaxSpeed(400);
  stepper.setAcceleration(100);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); //Nach Start mit leeren Display starten
  display.display();
}

void showDisplay() {
  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print("H");
  display.print(char(0x94));
  display.print("he:");
  display.setCursor(10, 20);
  display.print(position * 0.01, 2);
  display.print(" mm");
  display.display();
}

void interrupt() {
  encoder.tick();
  lastMoveTime = millis();
}

void loop() {
  pos = encoder.getPosition();
  position += pos;
  encoder.reset();

  if (millis() - lastMoveTime >= TimeOut){
    stepper.moveTo(position);   
    stepper.run();
  }
  else {
    stepper.stop();
    showDisplay();
  }
}
