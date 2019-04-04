/*
Table of Contants
1. move function
2. moveRight function
3. moveLeft function
4. moveDown function
5. moveUp function
6. next function - moves to the next position based on 96-well plate sampling sequence
7. previous function - opposite of next
8. reset function
9. set function - moves to a user specified position on 96-well plate
10. done function
11. setup function (Arduino)
12. main loop
*/

// #include <Wire.h>
// #include <Adafruit_MotorShield.h>
// #include "utility/Adafruit_PWMServoDriver.h"

//INTIAL SETUP
const int MS1PinT = 26; // Digital Pin Number for Top Stepper Motor MS1.
const int MS2PinT = 27; // Digital Pin Number for Top Stepper Motor MS2.
const int MS1PinB = 4; // Digital Pin Number for Bottom Stepper Motor MS1.
const int MS2PinB = 5; // Digital Pin Number for Bottom Stepper Motor MS2.
const int directionTop = 22; // Digital Pin Number for Top Stepper Motor Direction.
const int stepTop = 23; // Digital Pin Number for Top Stepper Motor.
const int directionBottom = 24; // Digital Pin Nuumber for Bottom Stepper Motor Direction.
const int stepBottom = 25; // Digital Pin Number for Bottom Stepper Motor.
const int myDelay = 500;
const int stepsT = 1000;
const int stepsB = 1000;
const int limitSwitchT = 10; // Digital Pin Number for Top Stepper Motor Limit Switch.
const int limitSwitchB = 11; // Digital Pin Number for Bottom Stepper Motor Limit Switch.
const int TwoSwitchA = 32; // Digital Pin Number for TwoSwitchA
const int TwoSwitchB = 33; // Digital Pin Number for TwoSwitchB
const int TwoSwitchC = 34; // Digital Pin Number for TwoSwitchC

int myPosition = 1;

//GENERIC MOVE FUNCTION
void move(int myDelay, int steps, int stepPin, int directionPin, int myDirection){
  //myDelay - delay between motor steps (200steps/rev)
  //steps - total number of steps
  //stepPin - connected to "step" on the motor drivers (EZ Driver)
  //directionPin - connected to "dir" on motor drivers (EZ Driver)
  //myDirection #which is CW/CCW? (Define motoz Z-axis for reference)

  digitalWrite(directionPin, myDirection);
  int counter;
  for(counter = steps; counter > 0; counter--){
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(myDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(myDelay);
  }
}

//MOVE RIGHT FUNCTION
void moveRight(int myDelay, int stepsT, int stepPinT, int directionPinT){
  bool myDirectionT = 0;
  move(myDelay, stepsT, stepPinT, directionPinT, myDirectionT);
}

//MOVE LEFT FUNCTION
void moveLeft(int myDelay, int stepsT, int stepPinT, int directionPinT){
  bool myDirectionT = 1;
  move(myDelay, stepsT, stepPinT, directionPinT, myDirectionT);
}

//MOVE DOWN FUNCTION
void moveDown(int myDelay, int stepsB, int stepPinB, int directionPinB){
  bool myDirectionB = 0;
  move(myDelay, stepsB, stepPinB, directionPinB, myDirectionB);
}

//MOVE UP FUNCTION
void moveUp(int myDelay, int stepsB, int stepPinB, int directionPinB){
  bool myDirectionB = 1;
  move(myDelay, stepsB, stepPinB, directionPinB, myDirectionB);
}

//MOVES TO THE NEXT POSITION. Returns the current position after moving one position.
int next(int myDelay, int stepsT, int stepsB, int stepPinT, int stepPinB, int directionPinT, int directionPinB, int myPosition) {
  if (myPosition == 32){
    Serial.println("You have reached the end, can't go further!");
    return 32;
  }
  int right[3] = {8,16,24};
  int down[14] = {1,2,3,4,5,6,7,17,18,19,20,21,22,23};
  int up[14] = {9,10,11,12,13,14,15,25,26,27,28,29,30,31};

  for (int i = 0; i < 32; i++){ //Might only need to loop up until myPosition + 1?
    if (myPosition == right[i]){
      for (int j = 0; j < 3; j++){
        moveRight(myDelay, stepsT, stepPinT, directionPinT);
      }
      myPosition = myPosition + 1;
      break;
    }

    if (myPosition == down[i]){
      moveDown(myDelay, stepsB, stepPinB, directionPinB);
      myPosition = myPosition + 1;
      break;
    }

    if (myPosition == up[i]){
      moveUp(myDelay, stepsB, stepPinB, directionPinB);
      myPosition = myPosition + 1;
      break;
    }
  }

  return myPosition;
}

//MOVES TO THE PREVIOUS POSITION. Returns the current position after moving back one position.
int previous(int myDelay, int stepsT, int stepsB, int stepPinT, int stepPinB, int directionPinT, int directionPinB, int myPosition){
  if (myPosition == 1){
    Serial.println("You are at the beginning can't go back a step!");
    return 1;
  }
  int left[3] = {9,17,25};
  int down[14] = {10,11,12,13,14,15,16,26,27,28,29,30,31,32};
  int up[14] = {2,3,4,5,6,7,8,18,19,20,21,22,23,24};

  for (int i = 0; i < 32; i++){  //Might only need to loop up until myPosition + 1?
    if (myPosition == left[i]){
      for (int j = 0; j < 3; j++){
        moveLeft(myDelay, stepsT, stepPinT, directionPinT);
      }
      myPosition = myPosition - 1;
      break;
    }

    if (myPosition == down[i]){
      moveDown(myDelay, stepsB, stepPinB, directionPinB);
      myPosition = myPosition - 1;
      break;
    }

    if (myPosition == up[i]){
      moveUp(myDelay, stepsB, stepPinB, directionPinB);
      myPosition = myPosition - 1;
      break;
    }
  }

  return myPosition;
}

// SETS TO A USER SPECIFIED POSITION.
void set(int myDelay, int stepsT, int stepsB, int stepPinT, int stepPinB, int directionPinT, int directionPinB, int myPosition){

}

void done(int myDelay, int stepsT, int stepsB, int stepPinT, int stepPinB, int directionPinT, int directionPinB, int myPosition) {
  /* code */
}

// SETS POSITION BACK AT ORIGIN (TOP LEFT OF WELL PLATE)
void reset(int myDelay, int stepsT, int stepsB, int stepPinT, int stepPinB, int directionPinT, int directionPinB) {

  // Start Homing procedure of Stepper Motors at startup.
  //Homing Procedure Top Steper Motor.
  // while (digitalRead(limitSwitchT)) {  // Do this until the switch is activated
  //   digitalWrite(directionPinT, HIGH);      // (HIGH = anti-clockwise / LOW = clockwise)
  //   digitalWrite(stepPinT, HIGH);
  //   delay(5);                       // Delay to slow down speed of Stepper
  //   digitalWrite(stepPinT, LOW);
  //   delay(5);
  // }
  //
  // while (!digitalRead(limitSwitchT)) { // Do this until the switch is not activated
  //   digitalWrite(directionPinT, LOW);
  //   digitalWrite(stepPinT, HIGH);
  //   delay(10);                       // More delay to slow even more while moving away from switch
  //   digitalWrite(stepPinT, LOW);
  //   delay(10);
  // }

  // while (digitalRead(limitSwitchB)) {  // Do this until the switch is activated
  //   digitalWrite(directionPinB, HIGH);      // (HIGH = anti-clockwise / LOW = clockwise)
  //   digitalWrite(stepPinB, HIGH);
  //   delay(5);                       // Delay to slow down speed of Stepper
  //   digitalWrite(stepPinB, LOW);
  //   delay(5);
  // }
  //
  // while (!digitalRead(limitSwitchB)) { // Do this until the switch is not activated
  //   digitalWrite(directionPinB, LOW);
  //   digitalWrite(stepPinB, HIGH);
  //   delay(10);                       // More delay to slow even more while moving away from switch
  //   digitalWrite(stepPinB, LOW);
  //   delay(10);
  // }
  //
  // myPosition = 1;  // Reset position variable to one.
}

//NEXT SITE FUNCTION
/*
 * The next site function keeps lists that tell the system whether it should do one of the folowing
 * Move the top motor in direction LOW
 * Move the top motor in direction HIGH
 * Move the bottom motor in direction LOW
 * Move the bottom motor in direction HIGH
 *
 * THIS FUNCTION MUST BE CALIBRATED VIA THE NUMBER OF STEPS WHICH WILL DIFFER IF
 * LITERALLY ANYTHING IS CHANGED ON THE XYSTAGE PULLEY/MOTOR SYSTEM
 */

// int next(int myDelay, int myPosition, int stepsT, int stepsB, int stepPinT, int stepPinB, int directionPinT, int directionPinB)
// {
//   int right[12] = {1,2,3,9,10,11,17,18,19,25,26,27};
//   int left[12] = {5,6,7,13,14,15,21,22,23,29,30,31};
//   int down[7] = {4,8,12,16,20,24,28};
//
//   bool myDirectionT;
//   bool myDirectionB;
//
//   //Movement of the top motor (L/R)
//   int i;
//   for (i = 0; i < 12; i++)
//   {
//     if (myPosition == right[i]){
//       myDirectionT = 0;
//       move(myDelay, stepsT, stepPinT, directionPinT, myDirectionT);
//       myPosition = myPosition + 1;
//     }
//     if (myPosition == left[i]){
//       myDirectionT = 1;
//       move(myDelay, stepsT, stepPinT, directionPinT, myDirectionT);
//       myPosition = myPosition + 1;
//     }
//   }
//
//   //Movement of the bottom motor (Down only. Up is in Previous)
//   for (i = 0; i < 7; i++) {
//     if (myPosition == down[i]){
//       myDirectionB = 0;
//       move(myDelay, stepsB, stepPinB, directionPinB ,myDirectionB);
//       myPosition = myPosition + 1;
//     }
//   }
//   if (myPosition == 32){
//     //do nothing
//   }
//   return myPosition;
// }
//
// int previous(){
//   //sees where we are and moves back down the list
//   //if at 8,12,16,20,24,28,32 the next move is up
//   //if at 0 do nothing
//   return 0;
// }

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  pinMode(TwoSwitchA, OUTPUT);
  pinMode(TwoSwitchB, OUTPUT);
  pinMode(TwoSwitchC, OUTPUT);

  pinMode(limitSwitchB, INPUT_PULLUP);
  pinMode(limitSwitchT, INPUT_PULLUP);

  pinMode(directionTop, OUTPUT);
  pinMode(stepTop, OUTPUT);
  pinMode(directionBottom, OUTPUT);
  pinMode(stepBottom,OUTPUT);

  digitalWrite(directionTop, LOW);
  digitalWrite(stepTop, LOW);
  digitalWrite(directionBottom, LOW);
  digitalWrite(stepBottom, LOW);

  /* Configure type of Steps on EasyDriver(s):
  // MS1 MS2
  //
  // LOW LOW = Full Step //
  // HIGH LOW = Half Step //
  // LOW HIGH = A quarter of Step //
  // HIGH HIGH = An eighth of Step //
  */

  // digitalWrite(MS1PinT, LOW);      // Configures to Full Steps
  // digitalWrite(MS2PinT, LOW);    // Configures to Full Steps
  // digitalWrite(MS1PinB, LOW);      // Configures to Full Steps
  // digitalWrite(MS2PinB, LOW);    // Configures to Full Steps

  // Sets the XY Stage at the Origin.
  //reset(myDelay, stepsT, stepsB, stepTop, stepBottom, directionTop, directionBottom);

  //code to set system initially
  Serial.println("XY Stage Control: Send Your Commands!");

}

//MAIN LOOP
void loop() {
  //ascii to decimal for XY Stage
  //R = 82 Reset/Set function
  //N = 78 Next function
  //L = 76 Previous/Last function
  //E = 69 Done/Eject function

  //Ascii to Decimal for TwoSwitches
  //TwoSwitch A: Collection(A) = 65, Recirculation(S) = 83
  //TwoSwitch B: Collection(D) = 68, Recirculation(F) = 70
  //TwoSwitch C: Collection(G) = 71, Recirculation(H) = 72

  //Here we're looping non-stop checking the serial buffer each time.
  //If you send several serial commands while the system is in one loop
  //it will not catch on the next loop. If you send just one then it will.
  //The way it's currently set up it reads the whole buffer at once so
  //something like 8378 will not be recognized as S and then N afterwards.

  int incomingByte;
  if (Serial.available()>0){
    incomingByte = Serial.read();
    Serial.println(incomingByte);

//////////////////////////////////////////////

    //S(et)
    if (incomingByte == 82) {
      Serial.println("Setting XY Stage!");
      reset(myDelay, stepsT, stepsB, stepTop, stepBottom, directionTop, directionBottom);
    }

    //N(ext)
    if (incomingByte == 78) {
      Serial.println("Moving to Next Position!");
      myPosition = next(myDelay, stepsT, stepsB, stepTop, stepBottom, directionTop, directionBottom, myPosition);
      Serial.println(myPosition);
    }

    //P(revious)
    if (incomingByte == 76) {
      Serial.println("Moving to Previous Position!");
      myPosition = previous(myDelay, stepsT, stepsB, stepTop, stepBottom, directionTop, directionBottom, myPosition);
      Serial.println(myPosition);
    }

    //D(one)
    if (incomingByte == 69) {
      Serial.println("Finished Collecting!");
    }
//////////////////////////////////////////////
    // Two Switch Collection Protocol.
    if (incomingByte == 65) {
      Serial.println("Setting TwoSwitch A to Collection.");
      // code to switch the TwoSwitch so fluid will be collected on well plate.
      digitalWrite(TwoSwitchA, LOW);
    }

    if (incomingByte == 83) {
      Serial.println("Setting TwoSwitch A to Recirculation");
      // code to switch the TwoSwitch so fluid will be recirculated to the chip.
      digitalWrite(TwoSwitchA, HIGH);
    }

    if (incomingByte == 68) {
      Serial.println("Setting TwoSwitch B to Collection");
      // code to switch the TwoSwitch so fluid will be collected on well plate.
      digitalWrite(TwoSwitchB, LOW);
    }

    if (incomingByte == 70) {
      Serial.println("Setting TwoSwitch B to Recirculation");
      // code to switch the TwoSwitch so fluid will be recirculated to the chip.
      digitalWrite(TwoSwitchB, HIGH);
    }

    if (incomingByte == 71) {
      Serial.println("Setting TwoSwitch C to Collection");
      // code to switch the TwoSwitch so fluid will be collected on well plate.
      digitalWrite(TwoSwitchC, LOW);
    }

    if (incomingByte == 72) {
      Serial.println("Setting TwoSwitch C to Recirculation");
      // code to switch the TwoSwitch so fluid will be reciruclated to the chip.
      digitalWrite(TwoSwitchC, HIGH);
    }
  }
}
