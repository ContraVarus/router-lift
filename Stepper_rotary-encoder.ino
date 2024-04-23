#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <splash.h>

#include <SPI.h>
#include <Wire.h>

#include <AccelStepper.h>
#include <MultiStepper.h>

#include <EncoderStepCounter.h>

#define ENCODER_PIN1 2
#define ENCODER_INT1 digitalPinToInterrupt(ENCODER_PIN1)
#define ENCODER_PIN2 3
#define ENCODER_INT2 digitalPinToInterrupt(ENCODER_PIN2)

#define SCREEN_WIDTH 128 //OLED Definition
#define SCREEN_HEIGHT 64
#define OLED_RESET  11
#define OLED_MOSI 12  //Pin SDA on OLED
#define OLED_CLK  13  // Pin SCL on OLED
#define OLED_DC 10  
#define OLED_CS 9
#define SCREEN_ADDRESS 0x3D

const int dirPin = 7;
const int stepPin = 8;
int lastpos = 0;
signed long position = 0;
signed char pos = 0;
bool motorrun = 0;

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);
EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2, FULL_STEP);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS); 

void setup() {
  Serial.begin(9600);
 
  encoder.begin();
 
  attachInterrupt(ENCODER_INT1, interrupt, CHANGE);
  attachInterrupt(ENCODER_INT2, interrupt, CHANGE);

  stepper.setMaxSpeed(100);
  stepper.setAcceleration(50.0);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void interrupt() {
  encoder.tick();
}

void showDisplay() {
  display.clearDisplay();
  display.setCursor(10, 10);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println("Height: ");
  display.setCursor(10, 40);
  display.print(pos * 0.025, 1);
  display.print(" mm");
  display.display();
}

void loop() {
  pos = encoder.getPosition();
  position += pos;
  encoder.reset();
  stepper.moveTo(position);
  stepper.run();
}
