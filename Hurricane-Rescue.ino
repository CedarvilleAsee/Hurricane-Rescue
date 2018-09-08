#include <Arduino.h>
#include <Servo.h>
#include "sorter.h"
#include "new_pins.h"
#include "constants.h"

// Already defined in sorter.cpp
/*bool delayState(int ms) {
  static int milliseconds = -1;
  if(milliseconds == -1) {
    milliseconds = millis();
  }
  else if(millis() - milliseconds >= ms) {
    milliseconds = -1;
    return true;
  }
  return false;
}*/

//testing

int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;
int printVar = 0;
bool otherPrintVar = false;

// For bluetooth sender
int iterations = 0;

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

  pinMode(LEDY, OUTPUT);
  pinMode(LEDG, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);

  pinMode(WHEEL_STBY, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);

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
  writeWheelDirection(true);
  digitalWrite(WHEEL_STBY  , HIGH);

  Serial.begin(115200);
  Serial3.begin(115200);
  Serial3.write("Does this thing really work?!");
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

void writeWheelDirection(bool dir) {
  if(dir) {
    digitalWrite(WHEEL_DIR_LB, LOW );
    digitalWrite(WHEEL_DIR_LF, HIGH);
    digitalWrite(WHEEL_DIR_RF, HIGH);
    digitalWrite(WHEEL_DIR_RB, LOW );
  } else {
    digitalWrite(WHEEL_DIR_LB, HIGH);
    digitalWrite(WHEEL_DIR_LF, LOW);
    digitalWrite(WHEEL_DIR_RF, LOW);
    digitalWrite(WHEEL_DIR_RB, HIGH);
  }
}

void writeToWheels(int ls, int rs) {
  analogWrite(WHEEL_SPEED_L, ls);
  analogWrite(WHEEL_SPEED_R, rs);
}

bool lineFollow(int ts, int strictness) {
  int offset = firstLineIndex - TARGET_INDEX;
  int rightSpeed = ts - offset*strictness;
  int leftSpeed = ts + offset*strictness;
  writeToWheels(leftSpeed, rightSpeed);

  //return true if line follow state is done
  return amountSeen > TURN_AMOUNT;
}

bool turn(int ts, int strictness, char direction) {
  if(direction == LEFT){
    writeToWheels(ts - strictness, ts + strictness);
  }else{
    writeToWheels(ts + strictness, ts - strictness);
  }

  // FIXME: need to check "amountSeen < 3" this is an arbitrary value dependant on the type of turn
  return firstLineIndex >= TARGET_INDEX  && amountSeen < 3;
}

// Old robot code we probably don't need
bool findLine(int ts) {
  writeToWheels(ts, ts);
  return amountSeen >= 2;
}

// State 0
//  Waits until the button on board is pushed. When it is pushed then go to next state
bool waitState() {
  writeToWheels(0, 0);
  if(digitalRead(BUTTON1) == LOW) {
    return true;
  }
  return false;
}

bool doTurnSequence(const char sequence[], int index) {
  static bool turning = false;
  otherPrintVar = turning;
  if(turning) {
    if(turn(100, 100, sequence[index])) {
      turning = false;
      return true;
    }
  } else {
    turning = lineFollow(150, 26);
  }
  return false;
}

// State 1
//  Starts the first turn sequence and returns true after
bool followTrackState() {
  static int state = 0;

  printVar = state;
  bool isFinished = false;

  // FIXME: old code that turns the robot off only after 14 turns
  switch(state) {
    case 14:
      isFinished = amountSeen > TURN_AMOUNT;
      break;
  }

  // Will be true after the individual turn is finished, then state++ so the next turn goes
  if(doTurnSequence(TURN_SEQUENCE, state)) state++;

  return isFinished;
}

void loop() {
  static int state = 0;
  readLine();
  switch(state) {
    case 0:
      if(waitState())  state++;
      break;
    case 1:
      if(followTrackState())  state++;
      break;
    //case 2:
      //if(Next state is done)  state++;
      //break;
    case 4:
      //if(Last state is done) state = 0;
      break;
  }

  iterations++;
  if(iterations == BLUETOOTH_LIMITER) {
    iterations = 0;
    Serial3.print("[ ");
    for(int i = 0; i < 8; i++) {
      Serial3.print(sensors[i]);
      Serial3.print(" ");
    }
    Serial3.println("]");
    //Serial3.print("[ ");
    //Serial3.print(firstLineIndex);
    //Serial3.print(", ");
    //Serial3.print(amountSeen);
    //Serial3.print(" ]");
    //Serial3.print(" - ");
    //Serial3.print(printVar);
    //Serial3.print(", ");
    //Serial3.println(otherPrintVar);
  }
}
