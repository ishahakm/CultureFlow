/**
 *  @file XYStage_LimitSwitch_TwoSwitch__Control.cpp
 *  Arduino code for XYStage Movement, Limit Switch, and TwoSwitch control.
 *  @author Adiel Hernandez
 */

/**
 *  Imports needed. The Adafruit_MotorShieldV2 library and AccelStepper Library is used.
 */
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

/**
 *  Global Variables. Constains mostly motor values and digital pin values.
 */
const int delayRPM = 255; /**< RPM for the stepper motors. */
const int rows = 8; /**< Amount of rows on well plate. */
const int columns = 12; /**< Amount of columns on well plate. */
const int numLocations = ((rows * columns) / 3) + 1; /**< Amount of triplet wells on well plate. (Plus 1 for the eject location). */
const float maximumSpeed = 255.0; /**< Max Speed of the Stepper Motors. */
const float acceleration = 2000.0; /**< Acceleration for the Stepper Motors.*/
const int stepsPerRevolution = 200; /**< Steps per revolution for the Stepper Motor. */
const int stepsPerWell = 225; /**< Amount of steps needed from center of well to the next. */
const int stepsToEject = 1000; /**< Amount of steps to eject entire well plate. */
const int limitSwitchT = 41; /**< Digital Pin Number for Top Stepper Motor Limit Switch. */
const int limitSwitchB = 43; /**< Digital Pin Number for Bottom Stepper Motor Limit Switch. */
const int TwoSwitchA = 31; /**< Digital Pin Number for TwoSwitchA. */
const int TwoSwitchB = 33; /**< Digital Pin Number for TwoSwitchB. */
const int TwoSwitchC = 35; /**< Digital Pin Number for TwoSwitchC. */

//Function Decalartions.
void forwardstep1();
void backwardstep1();
void forwardstep2();
void backwardstep2();
void populateLocations(long (&locations)[numLocations][2], int choice = 1);
void moveGen(Adafruit_StepperMotor *motor, int delay, int myDirection, int stepType);
void moveRight();
void moveLeft();
void moveDown();
void moveUp();
int moveNext();
int moveLast();
int set();
int eject();
void reset();

// Create an Adafruit Motor Shield Object and stepper motor objects.
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotorT = AFMS.getStepper(stepsPerRevolution, 1);
Adafruit_StepperMotor *myMotorB = AFMS.getStepper(stepsPerRevolution, 2);

// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepperB(forwardstep1, backwardstep1);
AccelStepper stepperT(forwardstep2, backwardstep2);

// Create MultiStepper object.
MultiStepper steppers;

// Declarations for positioning.
// position[0] is for top motor.
// position[1] is for bottom motor.
// For Top Motor: Negative is to Left. Positive to Right.
// For Bottom Motor: Positive is Down. Negative is Up.
int myPosition = 0; /**< Position variable to keep track of which triplet well we are on. */
long locations[numLocations][2]; /**< Three dimensional array for schematic of positions if MultiStepper is used. */
long positions[2]; /**< Array for movement needed for using both motors in MultiStepper. */
int collectionPattern = 1; /**< Collection Pattern variable to choose between snake pattern XY Stage movement (default = 1) or Top down XY Stage movement (!= 1). 

/**
 *  Arduino Setup. This only runs once.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  //Start the Arduino Motor Shield.
  AFMS.begin();

  // Set Motor Speeds not used in MultiStepper and AccelStepper.
  // Only for independent motor control.
  myMotorT -> setSpeed(delayRPM);
  myMotorB -> setSpeed(delayRPM);

  //Setting Stepper Motors configuratins beforehand.
  stepperT.setMaxSpeed(maximumSpeed);
  stepperT.setAcceleration(acceleration);

  stepperB.setMaxSpeed(maximumSpeed);
  stepperB.setAcceleration(acceleration);

  // Adding both steppers to a MultiStepper manager.
  steppers.addStepper(stepperT);
  steppers.addStepper(stepperB);

  // Populate locations based on the well plate type.
  populateLocations(locations);

  // Multistepper intitialitze positions.
  positions[0] = 0;
  positions[1] = 0;

  // Setting Pin Modes for TwoSwitches.
  pinMode(TwoSwitchA, OUTPUT);
  pinMode(TwoSwitchB, OUTPUT);
  pinMode(TwoSwitchC, OUTPUT);

  // Setting Pin Modes for Limit Switches.
  pinMode(limitSwitchB, INPUT_PULLUP);
  pinMode(limitSwitchT, INPUT_PULLUP);

  // Sets the XY Stage at the Origin.
  //reset();

  //code to set system initially
  //Serial.println("XY Stage Control: Send Your Commands!");

}


/**
 *  Arduino Loop Function, this runs continously. Essentially this looks for incoming
 *  commands which correspond to a certain function for motor movement or Limit Switching.
 */
void loop() {
  //ascii to decimal for XY Stage
  //? = 63 Request Identifier
  //Z = 90 Reset/Set function
  //N = 78 Next function
  //L = 76 Previous/Last function
  //E = 69 Done/Eject function
  //X = 88 Set collection pattern
  //U = 85 Move Left one step
  //I = 73 Move Right one step
  //O = 79 Move Down one step
  //P = 80 Move Up one step
  //M = 77 Set Origin 

  //Ascii to Decimal for TwoSwitches
  //TwoSwitch A: Collection(A) = 65, Recirculation(S) = 83
  //TwoSwitch B: Collection(D) = 68, Recirculation(F) = 70
  //TwoSwitch C: Collection(G) = 71, Recirculation(H) = 72

  //Here we're looping non-stop checking the serial buffer each time.
  //If you send several serial commands while the system is in one loop
  //it will not catch on the next loop. If you send just one then it will.
  //The way it's currently set up it reads the whole buffer at once so
  //something like 8378 will not be recognized as S and then N afterwards.

//  while(!digitalRead(limitSwitchT)){
//    Serial.println("It is pressed! TOP");
//  }
//
//  while(!digitalRead(limitSwitchB)){
//    Serial.println("It is pressed! BOT");
//  }

  int incomingByte;
  if (Serial.available() > 0){
    incomingByte = Serial.read();
    //Serial.println(incomingByte);

//////////////////////////////////////////////
    //? Request Unique ID for Serial Connection.
    if (incomingByte == 63){
      //Serial.println("Requested/Sending Unique Identifier!");
      Serial.write("2xCS");
    }

    //Toggles the collection pattern and resets stage.
    if (incomingByte == 88){
      if (collectionPattern == 1){
        populateLocations(locations, 0);
        collectionPattern = 0;
      }
      else if (collectionPattern == 0){
        populateLocations(locations);
        collectionPattern = 1;
      }
    }

    //S(et)
    if (incomingByte == 90) {
      //Serial.println("Setting XY Stage!");
      //unsigned long startTime = millis();
      reset();
      //unsigned long endTime = millis();
      //Serial.println(endTime - startTime);
    }

    //N(ext)
    if (incomingByte == 78) {
      //Serial.println("Moving to Next Position!");
      //unsigned long startTime = millis();
      myPosition = moveNext();
      //unsigned long endTime = millis();
      //Serial.println(myPosition);
      //Serial.println(endTime - startTime);
      //Serial.println(myPosition);
    }

    //P(revious)
    if (incomingByte == 76) {
      //Serial.println("Moving to Previous Position!");
      //unsigned long startTime = millis();
      myPosition = moveLast();
      //unsigned long endTime = millis();
      //Serial.println(myPosition);
      //Serial.println(endTime - startTime);
      //Serial.println(myPosition);
    }

    //D(one)
    if (incomingByte == 69) {
      //Serial.println("Finished Collecting!");
      eject();
    }

    //Move Up one step function. (P)
    if (incomingByte == 85) {
      positions[0] = positions[0] + 5;
      positions[1] = positions[1];
      steppers.moveTo(positions);
      steppers.runSpeedToPosition();
    }

    //Move Down one step function. (O)
    if (incomingByte == 73) {
      positions[0] = positions[0] - 5;
      positions[1] = positions[1];
      steppers.moveTo(positions);
      steppers.runSpeedToPosition();
    }

    //Move Right one step function. (I)
    if (incomingByte == 79) {
      positions[0] = positions[0];
      positions[1] = positions[1] + 5;
      steppers.moveTo(positions);
      steppers.runSpeedToPosition();
    }

    //Move Left one step function. (U)
    if (incomingByte == 80) {
      positions[0] = positions[0];
      positions[1] = positions[1] - 5;
      steppers.moveTo(positions);
      steppers.runSpeedToPosition();
    }

    //Set origin.
    if (incomingByte == 77){
      stepperB.setCurrentPosition(0);
      stepperT.setCurrentPosition(0);

      myPosition = 0;  // Reset position variable to zero.
      positions[0] = locations[myPosition][1];
      positions[1] = locations[myPosition][0];
    }
//////////////////////////////////////////////
    // Two Switch Collection Protocol.
    if (incomingByte == 65) {
      //Serial.println("Setting TwoSwitch A to Collection.");
      // code to switch the TwoSwitch so fluid will be collected on well plate.
      digitalWrite(TwoSwitchA, LOW);
    }

    if (incomingByte == 83) {
      //Serial.println("Setting TwoSwitch A to Recirculation");
      // code to switch the TwoSwitch so fluid will be recirculated to the chip.
      digitalWrite(TwoSwitchA, HIGH);
    }

    if (incomingByte == 68) {
      //Serial.println("Setting TwoSwitch B to Collection");
      // code to switch the TwoSwitch so fluid will be collected on well plate.
      digitalWrite(TwoSwitchB, LOW);
    }

    if (incomingByte == 70) {
      //Serial.println("Setting TwoSwitch B to Recirculation");
      // code to switch the TwoSwitch so fluid will be recirculated to the chip.
      digitalWrite(TwoSwitchB, HIGH);
    }

    if (incomingByte == 71) {
      //Serial.println("Setting TwoSwitch C to Collection");
      // code to switch the TwoSwitch so fluid will be collected on well plate.
      digitalWrite(TwoSwitchC, LOW);
    }

    if (incomingByte == 72) {
      //Serial.println("Setting TwoSwitch C to Recirculation");
      // code to switch the TwoSwitch so fluid will be reciruclated to the chip.
      digitalWrite(TwoSwitchC, HIGH);
    }
  }
}

//*******************************************************************
//FUNCTIONS

// You can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// Wrappers for the first motor!
/**
 *  Wrapper Forward Step function for Stepper Motor 1. To be used with the AccelStepper Library.
 */
void forwardstep1() {
  myMotorB->onestep(FORWARD, SINGLE);
}
/**
 *  Wrapper Backward Step function for Stepper Motor 1. To be used with the AccelStepper Library.
 */
void backwardstep1() {
  myMotorB->onestep(BACKWARD, SINGLE);
}
/**
 *  Wrapper Forward Step function for Stepper Motor 2. To be used with the AccelStepper Library.
 */
void forwardstep2() {
  myMotorT->onestep(FORWARD, SINGLE);
}
/**
 *  Wrapper Backward Step function for Stepper Motor 2. To be used with the AccelStepper Library.
 */
void backwardstep2() {
  myMotorT->onestep(BACKWARD, SINGLE);
}

/**
 *  This function populates the location matrix with the layout
 *  for moving to any location on the well plate. Gives the user the 
 *  choice for a snake pattern Choice = 1, or top to bottom pattern Choice != 1. 
 */
void populateLocations(long (&locations)[numLocations][2], int choice = 1){
  if (choice == 1){
    //Location for Triplet Well #1
    locations[0][0] = 0;
    locations[0][1] = 0;

    //Location for Triplet Well #2
    locations[1][0] = -225;
    locations[1][1] = 0;

    //Location for Triplet Well #3
    locations[2][0] = -450;
    locations[2][1] = 0;

    //Location for Triplet Well #4
    locations[3][0] = -675;
    locations[3][1] = 0;

    //Location for Triplet Well #5
    locations[4][0] = -900;
    locations[4][1] = 0;

    //Location for Triplet Well #6
    locations[5][0] = -1125;
    locations[5][1] = 0;

    //Location for Triplet Well #7
    locations[6][0] = -1350;
    locations[6][1] = 0;

    //Location for Triplet Well #8
    locations[7][0] = -1575;
    locations[7][1] = 0;

    //Location for Triplet Well #9
    locations[8][0] = -1575;
    locations[8][1] = 675;

    //Location for Triplet Well #10
    locations[9][0] = -1350;
    locations[9][1] = 675;

    //Location for Triplet Well #11
    locations[10][0] = -1125;
    locations[10][1] = 675;

    //Location for Triplet Well #12
    locations[11][0] = -900;
    locations[11][1] = 675;

    //Location for Triplet Well #13
    locations[12][0] = -675;
    locations[12][1] = 675;

    //Location for Triplet Well #14
    locations[13][0] = -450;
    locations[13][1] = 675;

    //Location for Triplet Well #15
    locations[14][0] = -225;
    locations[14][1] = 675;

    //Location for Triplet Well #16
    locations[15][0] = 0;
    locations[15][1] = 675;

    //Location for Triplet Well #17
    locations[16][0] = 0;
    locations[16][1] = 1350;

    //Location for Triplet Well #18
    locations[17][0] = -225;
    locations[17][1] = 1350;

    //Location for Triplet Well #19
    locations[18][0] = -450;
    locations[18][1] = 1350;

    //Location for Triplet Well #20
    locations[19][0] = -675;
    locations[19][1] = 1350;

    //Location for Triplet Well #21
    locations[20][0] = -900;
    locations[20][1] = 1350;

    //Location for Triplet Well #22
    locations[21][0] = -1125;
    locations[21][1] = 1350;

    //Location for Triplet Well #23
    locations[22][0] = -1350;
    locations[22][1] = 1350;

    //Location for Triplet Well #24
    locations[23][0] = -1575;
    locations[23][1] = 1350;

    //Location for Triplet Well #25
    locations[24][0] = -1575;
    locations[24][1] = 2025;

    //Location for Triplet Well #26
    locations[25][0] = -1350;
    locations[25][1] = 2025;

    //Location for Triplet Well #27
    locations[26][0] = -1125;
    locations[26][1] = 2025;

    //Location for Triplet Well #28
    locations[27][0] = -900;
    locations[27][1] = 2025;

    //Location for Triplet Well #29
    locations[28][0] = -675;
    locations[28][1] = 2025;

    //Location for Triplet Well #30
    locations[29][0] = -450;
    locations[29][1] = 2025;

    //Location for Triplet Well #31
    locations[30][0] = -225;
    locations[30][1] = 2025;

    //Location for Triplet Well #32
    locations[31][0] = 0;
    locations[31][1] = 2025;

    //Location for Eject
    locations[32][0] = 900;
    locations[32][1] = 2025;

  }
  else {
    int tripletCol = 1;
    for (int i = 0; i < numLocations - 1; i++){
      if (i % rows == 0){
        for (int j = 0; j < rows; j++){
          switch (tripletCol){
            case 1: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 2: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 3: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 4: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 5: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 6: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 7: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 8: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 9: locations[i+j][0] = -stepsPerWell * j;
                    locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                    break;
            case 10: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 11: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 12: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 13: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 14: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 15: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            case 16: locations[i+j][0] = -stepsPerWell * j;
                     locations[i+j][1] = (tripletCol - 1) * (3 * stepsPerWell);
                     break;
            default: break;
          }
        }
        tripletCol = tripletCol + 1;
      }
    }
    locations[numLocations - 1][0] = 900;
    locations[numLocations - 1][1] = 2025;
  }
}


/**
 *  Generic Move Function for Stepper Motors. This function takes in a pointer to the motor
 *  that has to be moved, the speed, the direction, and step type. All of this is used to
 *  move the correct motor accordingly.
 *  @param *motor a Adafruit_StepperMotor object pointer for the motor to be moved.
 *  @param delay the speed or RPM to move the motor at.
 *  @param myDirection the direction to move the motor in FORWARD or BACKWARD.
 *  @param stepType the step type to move the motor in: SINGLE, DOUBLE, INTERLEAVE, or MICROSTEP.
 */
void moveGen(Adafruit_StepperMotor *motor, int delay, int myDirection, int stepType){
  //steps - total number of steps
  //myDirection - FORWARD 1, BACKWARD 2, BRAKE 3, RELEASE 4.
  //Stepper Motor Nema 17 1.8 degrees/step. 200 Step/Revolution
  //Delay is handled by setting the RPM.
  //Step Types include SINGLE, DOUBLE, INTERLEAVE, MICROSTEP.
  if (motor == myMotorT){
    motor -> setSpeed(delay);
    motor -> step(stepsPerWell, myDirection, stepType);
  }
  else if (motor == myMotorB){
    motor -> setSpeed(delay);
    motor -> step(stepsPerWell, myDirection, stepType);
  }

}

/**
 *  MOVE RIGHT FUNCTION. Moves the top motor to the right. With SINGLE step type.
 */
void moveRight(){
  if (myMotorT != NULL){
    moveGen(myMotorT, delayRPM, FORWARD, SINGLE);
  }
}

/**
 *  MOVE LEFT FUNCTION. Moves the top motor to the left. With SINGLE step type.
 */
void moveLeft(){
  if (myMotorT != NULL){
    moveGen(myMotorT, delayRPM, BACKWARD, SINGLE);
  }
}

/**
 *  MOVE DOWN FUNCTION. Moves the bottom motor to the left. With SINGLE step type.
 */
void moveDown(){
  if (myMotorB != NULL){
    moveGen(myMotorB, delayRPM, BACKWARD, SINGLE);
  }
}

/**
 *  MOVE UP FUNCTION. Moves the bottom motor to the right. With SINGLE step type.
 */
void moveUp(){
  if (myMotorB != NULL){
    moveGen(myMotorB, delayRPM, FORWARD, SINGLE);
  }
}

/**
 *  MOVE NEXT FUNCTION. Moves the motor accordingly so it goes to the next well plate
 *  given it is not on the last possible triplet well location. Moves in a snake pattern.
 *  Decrements myPosition by 1.
 */
int moveNext(){
    if (myPosition < 31){
    positions[0] = locations[myPosition + 1][1];
    positions[1] = locations[myPosition + 1][0];
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();
    myPosition = myPosition + 1;
    //Serial.println(myPosition);
  }
  return myPosition;
}

/**
 *  MOVE NEXT FUNCTION. Moves the motor accordingly so it goes to the previous well plate
 *  given it is not on the first possible triplet well location. Moves in a snake pattern.
 *  Increments myPosition by 1.
 */
int moveLast(){

  if (myPosition > 0){
    positions[0] = locations[myPosition - 1][1];
    positions[1] = locations[myPosition - 1][0];
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();
    myPosition = myPosition - 1;
    //Serial.println(myPosition);
  }
  return myPosition;
}

// SETS TO A USER SPECIFIED POSITION.
// NOT SURE THIS FEATURE WILL BE NECCESARY AS OF YET.
int set(){
  /* code here */
  return myPosition;
}

/**
 *  EJECT WELL PLATE FUNCTION. Moves the motor accordingly so it goes to the eject well plate
 *  position. It moves the well plate down from its current location. Sets myPosition to 32.
 */
int eject() {
   positions[0] = locations[32][1];
   positions[1] = locations[32][0];
   steppers.moveTo(positions);
   steppers.runSpeedToPosition();

  myPosition = 32;

  return myPosition;
}

/**
 *  RESET FUNCTION. Moves the motor accordingly so it goes to the origin well plate position.
 *  Moves the motors simultaneously until both limit switches are activated and then moves
 *  a predetermined amount to origin.
 *
 */
void reset() {

  //Start Homing procedure of Stepper Motors at startup.
  //Homing Procedure Top Steper Motor. MULTI STEPPER.
 while (digitalRead(limitSwitchT) || digitalRead(limitSwitchB)){
   // Serial.println(digitalRead(limitSwitchT));
   // Serial.println(digitalRead(limitSwitchB));
   if (!digitalRead(limitSwitchT) && digitalRead(limitSwitchB)){
     positions[1] = positions[1] - 1;
     //Serial.println("Did if");
   }
   else if (!digitalRead(limitSwitchB) && digitalRead(limitSwitchT)){
     positions[0] = positions[0] + 1;
     //Serial.println("Did else if");
   }
   else {
     positions[0] = positions[0] + 1;
     positions[1] = positions[1] - 1;
     //Serial.println("Did else");

   }
   steppers.moveTo(positions);
   steppers.runSpeedToPosition();

  }

  //CODE TO MOVE THE WELL PLATE TO WHERE THE OUTLETS ARE ON TOP LEFT THREE WELLS.
  positions[0] = positions[0] - 4310;
  positions[1] = positions[1] + 2600;

  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  stepperB.setCurrentPosition(0);
  stepperT.setCurrentPosition(0);

  myPosition = 0;  // Reset position variable to zero.
  positions[0] = locations[myPosition][1];
  positions[1] = locations[myPosition][0];
}
