#include <Arduino.h>
#include <Servo.h>
#include "new_pins.h"
#include "constants.h"
#include "missions.h"
#include "PT6961.h"

/***********************Globals**************************/

int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;
Servo dump;
Servo arm;
Servo eject;
Servo claw;
bool turning = false;
bool pickingUp = false;
/////////////////////////////////
int missionNum = 1;
/////////////////////////////////
char redPath[40];
char neutralPath[40];
char redPickup[40];
char neutralPickup[40];
bool atWall = false;
int neutralSteps;
int redSteps;
int sensorCounter = 0;

int redIndex = 0;
int neutralIndex = 0;

PT6961 display(DIN, CLOCK, CS);

/***********************Setup***************************/

void setup() {
  display.initDisplay();
  display.sendNum(1234, 1);
  // initialize line sensors
  for(int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }

  pinMode(FRONT_SENSOR, INPUT);
  pinMode(FORK_SENSOR, INPUT);

 // initialize motor controllers
  pinMode(WHEEL_DIR_LB, OUTPUT);
  pinMode(WHEEL_DIR_LF, OUTPUT);
  pinMode(WHEEL_DIR_RB, OUTPUT);
  pinMode(WHEEL_DIR_RF, OUTPUT);

  //pinMode(LEDY, OUTPUT);
  //pinMode(LEDG, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);

  //pinMode(WHEEL_STBY, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);


  writeWheelDirection(WHEEL_FORWARDS, WHEEL_FORWARDS);

  dump.attach(DUMP_SERVO);//dump bin
  dump.write(15);//to closed position
  arm.attach(ARM_SERVO);
  arm.write(100);
  claw.attach(CLAW_SERVO);
  claw.write(1);
  eject.attach(EJECT_SERVO);
  eject.write(115);

}

/*****************************General Functions*******************/

int readFrontSensor() {
   return analogRead(FRONT_SENSOR);
}
int readForkSensor() {
   return analogRead(FORK_SENSOR);
}

/*int readRightClaw() {
   return analogRead(R_CLAW_SENSOR);
}
int readLeftClaw() {
   return analogRead(L_CLAW_SENSOR);
}*/

/*******************************Line Following********************/


// Populates the sensors[] variable so that we know amountSeen
void readLine() {
  amountSeen = 0;
  lastLineIndex = -1;
  for(int i = 7; i >= 0; --i) {

    sensors[i] = digitalRead(LINE_SENSOR[i]);

    if(sensors[i] == HIGH) {
      if(lastLineIndex == -1) {
        lastLineIndex = i;
      }
      ++amountSeen;
      firstLineIndex = i;
    }
  }

  sensorCounter = 0;
  for (int i = 0; i < 8; i++ ){
    sensorCounter += sensors[i]<<i;
  }
  
}

bool sensorsCentered() {
  return sensors[3] == HIGH && sensors[4] == HIGH;
}

void writeWheelDirection(bool ldir, bool rdir) {
  digitalWrite(WHEEL_DIR_LF, ldir);
  digitalWrite(WHEEL_DIR_LB, !ldir);
  digitalWrite(WHEEL_DIR_RF, rdir);
  digitalWrite(WHEEL_DIR_RB, !rdir);
}

void writeToWheels(int ls, int rs) {
  if(ls < 0) {
    digitalWrite(WHEEL_DIR_LF, false); //right backwards
    digitalWrite(WHEEL_DIR_LB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_LF, true); //right forwards
    digitalWrite(WHEEL_DIR_LB, false);
  }
  if(rs < 0) {
    digitalWrite(WHEEL_DIR_RF, false); //right backwards
    digitalWrite(WHEEL_DIR_RB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_RF, true); //right forwards
    digitalWrite(WHEEL_DIR_RB, false);
  }

  analogWrite(WHEEL_SPEED_L, abs(ls));
  analogWrite(WHEEL_SPEED_R, abs(rs));
}

bool lineFollow(int ts, int strictness) {
  static bool frontPassed = false;
  if(amountSeen >= TURN_AMOUNT){
    frontPassed = true;
    writeToWheels(ts, ts);
  }
  else {
    int rightSpeed = ts + (lastLineIndex - 4)*strictness;
    int leftSpeed = ts - (firstLineIndex - 3)*strictness;
    writeToWheels(leftSpeed, rightSpeed);
  }

  // Return true if the sensors can see a fork
  if(readFrontSensor() < WALL_CLOSE) {
    atWall = true;
    return true;
  }
  if(readForkSensor() > AT_FORK && frontPassed) {
    frontPassed = false;
    return true;
  }

  return false;
}

/*******************Turning******************************************/

bool turn(int spd, char dir) {
   static int lineCount = 0;
   static bool gotOffLine = false;
   if (dir == F) return true;


   if (dir == L || atWall) {
      writeToWheels(-spd, spd);
   }
   else if (dir == R) {
      writeToWheels(spd, -spd);
   }

   if (amountSeen == 0) gotOffLine = true;

   if (dir == B && !atWall) {
      writeToWheels(-(spd), spd);
      if (sensorsCentered() && gotOffLine) { 
         // if it isn't at a wall,
         //the line sensors have to pass another line to turn completely around
         lineCount++;
         gotOffLine = false;
      }
      if (lineCount == 2) {
         lineCount = 0;
         return true;
      }
   }
   // Return true if the robot is back centered on the line

   if (gotOffLine && sensorsCentered()) {
      atWall = false;
      gotOffLine = false;
      return true;
   }
   return false;

}

bool doTurnSequence(const char sequence[], int index, int maxSteps) {
   index++; // should be acting on next instruction, not current
   if (index == maxSteps) {
      if (lineFollow(FULL_SPEED, 20)) {
         return true;
      }
   }
   else {
      if (turning) {
         if (turn(HALF_SPEED, sequence[index])) {
            turning = false;
            return true;
         }
      }
      else {
         /*if(index + 1 < maxSteps){
         if(sequence[index + 1] == F) {
         turning = lineFollow(FULL_SPEED, 50);
         return false;
         }
         } //should work, but not tested with person*/
         turning = lineFollow(FULL_SPEED, 20);
      }
   }
   return false;
}

/*******************************States******************************/

bool delayState(int ms) {
  static int milliseconds = -1;
  if(milliseconds == -1) {
    milliseconds = millis();
  }
  else if(millis() - milliseconds >= ms) {
    milliseconds = -1;
    return true;
  }
  return false;
}

// State 0
//  Waits until the button on board is pushed, go to next state
bool waitState() {
  writeToWheels(0, 0);
  if(digitalRead(BUTTON_1) == LOW) {
    //get mission number from switches
    if(missionNum == 1) {
      strncpy(redPath, RED_MISSION_1, RED_STEPS_1);
      strncpy(neutralPath, NEUTRAL_MISSION_1, NEUTRAL_STEPS_1);
      strncpy(redPickup, RED_PICKUP_1, RED_STEPS_1);
      strncpy(neutralPickup, NEUTRAL_PICKUP_1, NEUTRAL_STEPS_1);
      redSteps = RED_STEPS_1;
      neutralSteps = NEUTRAL_STEPS_1;
    }
    else if(missionNum == 2) {
      strncpy(redPath, RED_MISSION_2, RED_STEPS_2);
      strncpy(neutralPath, NEUTRAL_MISSION_2, NEUTRAL_STEPS_2);
      strncpy(redPickup, RED_PICKUP_2, RED_STEPS_2);
      strncpy(neutralPickup, NEUTRAL_PICKUP_2, NEUTRAL_STEPS_2);
      redSteps = RED_STEPS_2;
      neutralSteps = NEUTRAL_STEPS_2;
    }
    else if(missionNum == 3) {
      strncpy(redPath, RED_MISSION_3, RED_STEPS_3);
      strncpy(neutralPath, NEUTRAL_MISSION_3, NEUTRAL_STEPS_3);
      strncpy(redPickup, RED_PICKUP_3, RED_STEPS_3);
      strncpy(neutralPickup, NEUTRAL_PICKUP_3, NEUTRAL_STEPS_3);
      redSteps = RED_STEPS_3;
      neutralSteps = NEUTRAL_STEPS_3;
    }
    else if(missionNum == 4) {
      strncpy(redPath, RED_MISSION_4, RED_STEPS_4);
      strncpy(neutralPath, NEUTRAL_MISSION_4, NEUTRAL_STEPS_4);
      strncpy(redPickup, RED_PICKUP_4, RED_STEPS_4);
      strncpy(neutralPickup, NEUTRAL_PICKUP_4, NEUTRAL_STEPS_4);
      redSteps = RED_STEPS_4;
      neutralSteps = NEUTRAL_STEPS_4;
    }
    return true;
  }
  return false;
}


void doPickupSequence(const char sequence[], int pathIndex) {
  static int pickupStateIndex = 0;
  static bool atNextSquare = true;
  static int prevIndex = pathIndex;
  if(prevIndex != pathIndex) { 
    prevIndex = pathIndex;
    atNextSquare = true;
  }
  if(atNextSquare){
    if(sequence[pathIndex] == E) {
      display.sendMessage(PICKUP_EMPTY);
      atNextSquare = false;
      return;
    }
    else if(sequence[pathIndex] == L) {
      display.sendMessage(PICKUP_LEFT);
        atNextSquare = false; //temp
        return;//temp
      /*switch(pickupStateIndex) {
        case 0:
          leftClawArm.write(L_DOWN_POS);
          if(delayState(400)) pickupStateIndex++;
          break;
        case 1:
          if(readLeftClaw() < PERSON_CLOSE) {
            leftClaw.write(L_CLOSE);
            if(delayState(100)) pickupStateIndex++;
          }
          break;
        case 2:
          leftClawArm.write(L_UP_POS);
          if(delayState(400)) pickupStateIndex++;
          break;
        case 3:
          leftClaw.write(L_OPEN);
          if(delayState(100)) {
            pickupStateIndex = 0;
            atNextSquare = false;
            return;
          }
          break;
        default:
          leftClawArm.write(L_UP_POS);
          leftClaw.write(L_OPEN);
          break;
      }*/
    }
    else if(sequence[pathIndex] == R) {
      display.sendMessage(PICKUP_RIGHT);
      atNextSquare = false; //temp
      return; //temp
      /*switch(pickupStateIndex) {
        case 0:
          rightClawArm.write(R_DOWN_POS);
          if(delayState(400)) pickupStateIndex++;
          break;
        case 1:
          if(readRightClaw() < PERSON_CLOSE) {
            rightClaw.write(R_CLOSE);
            if(delayState(100)) pickupStateIndex++;
          }
          break;
        case 2:
          rightClawArm.write(R_UP_POS);
          if(delayState(400)) pickupStateIndex++;
          break;
        case 3:
          rightClaw.write(R_OPEN);
          if(delayState(100)) {
            pickupStateIndex = 0;
            atNextSquare = false;
            return ;
          }
          break;
        default:
          rightClawArm.write(R_UP_POS);
          rightClaw.write(R_OPEN);
          break;
      }*/
    }
  }
  return;
}

bool turnAroundState() {
  return turn(HALF_SPEED, B);
}


bool followRedPathState() {
  if(redIndex == redSteps) return true;
  if(doTurnSequence(redPath, redIndex, redSteps)) redIndex++;
  doPickupSequence(redPickup, redIndex);
  return false;
}

bool followNeutralPathState() {
  if(neutralIndex == neutralSteps) return true;
  if(doTurnSequence(neutralPath, neutralIndex, neutralSteps)) neutralIndex++;
  doPickupSequence(neutralPickup, neutralIndex);
  return false;
}

bool depositPeopleState(){
  /*dump.write(DO_DUMP);
  if(delayState(2000)) {
    dump.write(DONT_DUMP);
    return true;
  }
  */
  writeToWheels(0, 0);
  if(delayState(1000)) {
    return true;
  }
  display.sendMessage(DEPOSITING);
  return false;
}

bool doneState() {
   display.sendMessage(DONE);
   writeToWheels(0, 0);
   return false;
}

bool displayMissionState() {
  if(digitalRead(SWITCH1) == HIGH) {
    if(digitalRead(SWITCH0) == HIGH) {
      missionNum = 4;
    }
    else {
      missionNum = 3;
    }
  }
  else {
    if(digitalRead(SWITCH0) == HIGH) {
      missionNum = 2;
    }
    else {
      missionNum = 1;
    }
  }
  if(digitalRead(BUTTON_2) == LOW) {
    return true;
  }
  return false;
}

void loop() {
  static int state = -1;
  readLine();
  if(digitalRead(BUTTON_2) == LOW) state = 0;
  switch(state) {
    case -1:
      if(displayMissionState()) state++;
      display.sendNum(missionNum, 1);
      break;
    case 0:
      if(waitState())  state++;
      display.sendNum(sensorCounter, 0);
      break;
    case 1:
      if(followRedPathState())  state++;
      break;
    case 2:
      
      if(depositPeopleState())  state++;
      break;
    case 3: 
      if(turnAroundState()) state++;
      break;
    case 4:
      if(followNeutralPathState()) state++;
      break;
    case 5:
      if(depositPeopleState()) state++;
      break;
  default:
      if(doneState()) state = 0;
      break;
  }

}
