
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

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

//INTIAL SETUP
const int stepsPerRevolution = 200;
const int delayRPM = 255;
const int stepsT = 100;
const int stepsB = 100;
const int limitSwitchT = 11; // Digital Pin Number for Top Stepper Motor Limit Switch.
const int limitSwitchB = 30; // Digital Pin Number for Bottom Stepper Motor Limit Switch.
const int TwoSwitchA = 32; // Digital Pin Number for TwoSwitchA
const int TwoSwitchB = 33; // Digital Pin Number for TwoSwitchB
const int TwoSwitchC = 34; // Digital Pin Number for TwoSwitchC

// Create an Adafruit Motor Shield Object and stepper motor objects.
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotorB = AFMS.getStepper(stepsPerRevolution, 1);
Adafruit_StepperMotor *myMotorT = AFMS.getStepper(stepsPerRevolution, 2);

int myPosition = 1;

//GENERIC MOVE FUNCTION
void move(Adafruit_StepperMotor *motor, int delay, int myDirection, int stepType){
  //steps - total number of steps
  //myDirection - FORWARD 1, BACKWARD 2, BRAKE 3, RELEASE 4.
  //Stepper Motor Nema 17 1.8 degrees/step. 200 Step/Revolution
  //Delay is handled by setting the RPM.
  //Step Types include SINGLE, DOUBLE, INTERLEAVE, MICROSTEP.
  if (motor == myMotorT){
    motor -> setSpeed(delay);
    motor -> step(stepsT, myDirection, stepType);
  }
  else if (motor == myMotorB){
    motor -> setSpeed(delay);
    motor -> step(stepsB, myDirection, stepType);
  }

}

//MOVE RIGHT FUNCTION
void moveRight(){
  if (myMotorT != NULL){
    move(myMotorT, delayRPM, FORWARD, SINGLE);
  }
}

//MOVE LEFT FUNCTION
void moveLeft(){
  if (myMotorT != NULL){
    move(myMotorT, delayRPM, BACKWARD, SINGLE);
  }
}

//MOVE DOWN FUNCTION
void moveDown(){
  if (myMotorB != NULL){
    move(myMotorB, delayRPM, FORWARD, SINGLE);
  }
}

//MOVE UP FUNCTION
void moveUp(){
  if (myMotorB != NULL){
    move(myMotorB, delayRPM, BACKWARD, SINGLE);
  }
}

//MOVES TO THE NEXT POSITION. Returns the current position after moving one position.
int next() {
  if (myPosition == 32){
    //Serial.println("You have reached the end, can't go further!");
    return 32;
  }
  int right[3] = {8,16,24};
  int down[28] = {1,2,3,4,5,6,7,9,10,11,12,13,14,15,17,18,19,20,21,22,23,25,26,27,28,29,30,31};

  for (int i = 0; i < 32; i++){ //Might only need to loop up until myPosition + 1?
    if (myPosition == right[i]){
      for (int k = 0; k < 7; k++){
        if (k == 3){
          for (int j = 0; j < 3; j++){
            moveRight();
          }
        }
        moveUp();
      }
      myPosition = myPosition + 1;
      break;
    }

    if (myPosition == down[i]){
      moveDown();
      myPosition = myPosition + 1;
      break;
    }

  }

  return myPosition;
}

//MOVES TO THE PREVIOUS POSITION. Returns the current position after moving back one position.
int previous(){
  if (myPosition == 1){
    //Serial.println("You are at the beginning can't go back a step!");
    return 1;
  }
  int left[3] = {9,17,25};
  int up[28] = {2,3,4,5,6,7,8,10,11,12,13,14,15,16,18,19,20,21,22,23,24,26,27,28,29,30,31,32};

  for (int i = 0; i < 32; i++){  //Might only need to loop up until myPosition + 1?
    if (myPosition == left[i]){
      for (int k = 0; k < 7; k++){
        if (k == 3){
          for (int j = 0; j < 3; j++){
            moveLeft();
          }
        }
        moveDown();
      }
      myPosition = myPosition - 1;
      break;
    }

    if (myPosition == up[i]){
      moveUp();
      myPosition = myPosition - 1;
      break;
    }
  }

  return myPosition;
}

// SETS TO A USER SPECIFIED POSITION.
void set(){

}

// USER DONE WITH COLLECTION MOVES WELL PLATE TO EJECT.
void done() {
  /* code */
}

// SETS POSITION BACK AT ORIGIN (TOP LEFT OF WELL PLATE)
void reset() {

  // Start Homing procedure of Stepper Motors at startup.
  // Homing Procedure Top Steper Motor.
  // while (digitalRead(limitSwitchT)) {  // Do this until the switch is activated
  //   myMotorT -> onestep(FORWARD,SINGLE);
  //   delay(5);                       // Delay to slow down speed of Stepper
  // }
  //
  // while (!digitalRead(limitSwitchT)) { // Do this until the switch is not activated
  //   myMotorT -> onestep(BACKWARD, SINGLE);
  //   delay(10);                       // More delay to slow even more while moving away from switch
  // }
  //
  // while (digitalRead(limitSwitchB)) {  // Do this until the switch is activated
  //   myMotorB -> onestep(FORWARD, SINGLE);
  //   delay(5);                       // Delay to slow down speed of Stepper
  //   Serial.println("Switch not activated");
  // }
  //
  // while (!digitalRead(limitSwitchB)) { // Do this until the switch is not activated
  //   myMotorB -> onestep(BACKWARD, SINGLE);
  //   delay(10);                       // More delay to slow even more while moving away from switch
  //   Serial.println("Switch activated");
  // }

   // CODE TO MOVE THE WELL PLATE TO WHERE THE OUTLETS ARE ON TOP LEFT THREE WELLS.

  // myPosition = 1;  // Reset position variable to one.
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  //Start the Arduino Motor Shield.
  AFMS.begin();

  pinMode(TwoSwitchA, OUTPUT);
  pinMode(TwoSwitchB, OUTPUT);
  pinMode(TwoSwitchC, OUTPUT);

  pinMode(limitSwitchB, INPUT_PULLUP);
  pinMode(limitSwitchT, INPUT_PULLUP);

  // Sets the XY Stage at the Origin.
  //reset();

  //code to set system initially
  //Serial.println("XY Stage Control: Send Your Commands!");

}

//MAIN LOOP
void loop() {
  //ascii to decimal for XY Stage
  //? = 63 Request Port
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
  if (Serial.available() > 0){
    incomingByte = Serial.read();
    //Serial.println(incomingByte);

//////////////////////////////////////////////
    //? Request Unique ID for Serial Connection.
    if (incomingByte == 63){
      //Serial.println("Requested/Sending Unique Identifier!");
      Serial.write("Collectador");
    }

    //S(et)
    if (incomingByte == 82) {
      //Serial.println("Setting XY Stage!");
      reset();
    }

    //N(ext)
    if (incomingByte == 78) {
      //Serial.println("Moving to Next Position!");
      myPosition = next();
      //Serial.println(myPosition);
    }

    //P(revious)
    if (incomingByte == 76) {
      //Serial.println("Moving to Previous Position!");
      myPosition = previous();
      //Serial.println(myPosition);
    }

    //D(one)
    if (incomingByte == 69) {
      //Serial.println("Finished Collecting!");
      //done()
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
      //Serial.println("Setting TwoSwitch C to Recirculation");
      // code to switch the TwoSwitch so fluid will be reciruclated to the chip.
      digitalWrite(TwoSwitchC, HIGH);
    }
  }
}
