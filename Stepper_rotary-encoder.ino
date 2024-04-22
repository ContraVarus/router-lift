#include <AccelStepper.h>
#include <MultiStepper.h>

#include <EncoderStepCounter.h>

#define ENCODER_PIN1 2
#define ENCODER_INT1 digitalPinToInterrupt(ENCODER_PIN1)
#define ENCODER_PIN2 3
#define ENCODER_INT2 digitalPinToInterrupt(ENCODER_PIN2)

const int dirPin = 7;
const int stepPin = 8;
int lastpos = 0;

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);
EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2);

void setup() {
  Serial.begin(9600);
 
  encoder.begin();
  attachInterrupt(ENCODER_INT1, interrupt, CHANGE);
  attachInterrupt(ENCODER_INT2, interrupt, CHANGE);

  stepper.setMaxSpeed(500);
  stepper.setAcceleration(25.0);
}

void interrupt() {
  encoder.tick();
}

signed long position = 0;

void loop() {
  signed char pos = encoder.getPosition();
  if (pos != 0) {
    position += pos;
    encoder.reset();
  }
  if (position != lastpos) {
    stepper.moveTo(position);
    stepper.run();
  }
}
