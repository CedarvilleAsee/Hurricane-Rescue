#include <Arduino.h>
//#include <Servo.h>
#include "C:\Users\mathc\Documents\Arduino\hardware\Arduino_STM32\STM32F1\libraries\Servo\src\Servo.h"                

#include "new_pins.h"
#include "constants.h"
#include "missions.h"


int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;

bool turning = false;
bool pickingUp = false;
/////////////////////////////////
char redPath[40] = {F, R};
int redSteps = 2;
/////////////////////////////////
char neutralPath[40];
char redPickup[40];
char neutralPickup[40];

int neutralSteps;
int missionNum = 1;

int redIndex = 0;
int neutralIndex = 0;

void setup() {

  // initialize line sensors
  for(int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }

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

  /*
   * The following output configurations set both motors
   * to move forward. The two Robot solution doesn't require
   * backwards movement, so the wheels should be permanently
   * forward.
   *
   * - Note: when the new boards are installed, this code will be
   *        changed because the board is going to tie these to high
   *  - Issues here the right wheel is only spinning backwards
   */
  writeWheelDirection(WHEEL_FORWARDS, WHEEL_FORWARDS);
  //digitalWrite(WHEEL_STBY  , HIGH);

  Serial.begin(115200);
 
}


void testWheel(bool wheel, int ts) {
  if(wheel)
    writeToWheels(ts, 0);
  else
    writeToWheels(0, ts);
}

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
}

/*int readFrontRight(){//gets data val from right infraread sensor IMH
  return analogRead(FRONT_R_SENSOR);
}
int readFrontLeft(){//gets data val from left infraread sensor IMH
  return analogRead(FRONT_L_SENSOR);
}
int readRightClaw() {
   return analogRead(R_CLAW_SENSOR);
}
int readLeftClaw() {
   return analogRead(L_CLAW_SENSOR);
}*/
bool twoConsecutiveAtMiddle() {
  return twoConsecutive() && firstLineIndex >= TARGET_INDEX;
}


bool twoConsecutive() {
  int lowCount = 0;
  bool consecutive = false;
  for(int i = 0; i < 7; i++) {
    if(sensors[i] == HIGH && sensors[i + 1] == HIGH) {
      consecutive = true;
    }

    if(sensors[i] == LOW) {
      lowCount++;
    }
  }
  if(sensors[7] == LOW) {
    lowCount++;
  }
  return lowCount == 6 && consecutive;
}
void writeWheelDirection(bool ldir, bool rdir) {
  digitalWrite(WHEEL_DIR_LF, ldir);
  digitalWrite(WHEEL_DIR_LB, !ldir);
  digitalWrite(WHEEL_DIR_RF, rdir);
  digitalWrite(WHEEL_DIR_RB, !rdir);
}


void writeToWheels(int ls, int rs) {
  analogWrite(WHEEL_SPEED_L, abs(ls));
  analogWrite(WHEEL_SPEED_R, abs(rs));
}

bool lineFollow(int ts, int strictness) {
  static bool seenLine = false;
  int offset = firstLineIndex - TARGET_INDEX;
  int rightSpeed = ts - offset*strictness;
  int leftSpeed = ts + offset*strictness;
  writeToWheels(leftSpeed, rightSpeed);

  // Return true if the sensors can see a fork
  /*if(readFrontRight() < 30) {
    atWall = true;
    return true;
  }*/
  if(amountSeen > TURN_AMOUNT) seenLine = true;
  if(seenLine) {
    if(delayState(300)) {
      seenLine = false;
      return true;
    }
  }
  //return amountSeen > TURN_AMOUNT;
}

bool turn(int spd, char dir) {
  static int lineCount = 0;
  if(dir == L){
    writeToWheels(-spd, spd);
  }else if(dir == R){
    writeToWheels(spd, -spd);
  }else if(dir == F) {
    writeToWheels(spd, spd);
  }

  /*if(dir == B && !atWall) {
    if(twoConsecutiveAtMiddle()) { // if it isn't at a wall, the line sensors have to pass another line to turn completely around
      lineCount++;
    }
    if(lineCount == 2){
      lineCount = 0;
      return true;
    }
  }*/
  // Return true if the robot is back centered on the line
  else return twoConsecutiveAtMiddle();
}

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
//  Waits until the button on board is pushed. When it is pushed then go to next state
/*bool waitState() {
  writeToWheels(0, 0);
  if(digitalRead(BUTTON1) == LOW) {
    if(mission == 1) {
      strcpy(redPath, RED_MISSION_1);
      strcpy(neutralPath, NEUTRAL_MISSION_1);
      strcpy(redPickup, RED_PICKUP_1);
      strcpy(neutralPickup, NEUTRAL_PICKUP_1);
      redSteps = RED_STEPS_1;
      neutralSteps = NEUTRAL_STEPS_1;
    }
    else if(mission == 2) {
      strcpy(redPath, RED_MISSION_2);
      strcpy(neutralPath, NEUTRAL_MISSION_2);
      strcpy(redPickup, RED_PICKUP_2);
      strcpy(neutralPickup, NEUTRAL_PICKUP_2);
      redSteps = RED_STEPS_2;
      neutralSteps = NEUTRAL_STEPS_2;
    }
    else if(mission == 3) {
      strcpy(redPath, RED_MISSION_3);
      strcpy(neutralPath, NEUTRAL_MISSION_3);
      strcpy(redPickup, RED_PICKUP_3);
      strcpy(neutralPickup, NEUTRAL_PICKUP_3);
      redSteps = RED_STEPS_3;
      neutralSteps = NEUTRAL_STEPS_3;
    }
    else if(mission == 4) {
      strcpy(redPath, RED_MISSION_4);
      strcpy(neutralPath, NEUTRAL_MISSION_4);
      strcpy(redPickup, RED_PICKUP_4);
      strcpy(neutralPickup, NEUTRAL_PICKUP_4);
      redSteps = RED_STEPS_4;
      neutralSteps = NEUTRAL_STEPS_4;
    }
    return true;
  }
  return false;
}


void doPickupSequence(const char sequence[], int pathIndex) {
  static int pickupStateIndex = 0;
  static bool nextSquare = true;
  static int prevIndex = pathIndex;
  if(prevIndex != pathIndex) {
    prevIndex = pathIndex;
    nextSquare = true;
  }
  if(nextSquare){
    if(sequence[pathIndex] == EMPTY) {
      nextSquare = false;
      return;
    }
    else if(sequence[pathIndex] == L) {
      switch(pickupStateIndex) {
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
            nextSquare = false;
            return;
          }
          break;
        default:
          leftClawArm.write(L_UP_POS);
          leftClaw.write(L_OPEN);
          break;
      }
    }
    else if(sequence[pathIndex] == R) {
      switch(pickupStateIndex) {
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
            nextSquare = false;
            return ;
          }
          break;
        default:
          rightClawArm.write(R_UP_POS);
          rightClaw.write(R_OPEN);
          break;
      }
    }
  }
  return;
}*/


bool doTurnSequence(const char sequence[], int index, int maxSteps) {
  index++; // should be acting on next instruction, not current
  if(index == maxSteps) {
    if(lineFollow(HALF_SPEED, 50)){
      return true;
    }
  }
  else {
    if(turning) {
      if(turn(HALF_SPEED, sequence[index])) {
        turning = false;
        return true;
      }
    } else {
      /*if(index + 1 < maxSteps){
        if(sequence[index + 1] == F) {
          turning = lineFollow(FULL_SPEED, 50);
          return false;
        }
      } //should work, but not tested with person*/
      turning = lineFollow(HALF_SPEED, 50);
    }
  }
  return false;
}

bool followRedPathState() {
  if(redIndex == redSteps) return true;
  if(doTurnSequence(redPath, redIndex, redSteps)) redIndex++;
  //doPickupSequence(redPickup, redIndex);
  return false;
}

/*bool followNeutralPathState() {
  if(neutralIndex == neutralSteps) return true;
  if(doTurnSequence(neutralPath, neutralIndex, neutralSteps)) neutralIndex++;
  doPickupSequence(neutralPickup, neutralIndex);
  return false;
}



bool depositPeopleState(){
  dump.write(DO_DUMP);
  if(delayState(2000)) {
    dump.write(DONT_DUMP);
    return true;
  }
  return false;
}*/
bool doneState() {

}
void loop() {
  static int state = 0;
  readLine();
  switch(state)
  {
    case 0:
      if(digitalRead(BUTTON_1) == LOW)  state++;
      break;
    case 1:
      if(followRedPathState())  state = 0;

      break;
    /*case 2:
      if(depositPeopleState())  state++;
      break;
    case 3:
      if(followNeutralPathState()) state++;
      ;
      break;
    case 4:
      if(depositPeopleState()) state++;
      break;*/
  default:
      if(doneState()) state = 0;
      break;
  }

}
