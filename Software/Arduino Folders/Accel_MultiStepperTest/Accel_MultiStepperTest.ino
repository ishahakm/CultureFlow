// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library 
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support 
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *myStepper2 = AFMS.getStepper(200, 2);


// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  myStepper2->onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  myStepper2->onestep(BACKWARD, SINGLE);
}


// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

MultiStepper steppers;

void setup()
{  
  AFMS.begin(); 
 
   
  stepper1.setMaxSpeed(255.0);
  stepper1.setAcceleration(2000.0);
    
  stepper2.setMaxSpeed(255.0);
  stepper2.setAcceleration(2000.0);
 
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);

  long positions[2];
  positions[0] = 0;
  positions[1] = 500;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  
//  delay(2000);
//  
//  stepper1.setCurrentPosition(0);
//  stepper2.setCurrentPosition(0);
//
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition();

}

void loop()
{


}
