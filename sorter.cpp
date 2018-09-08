#include <Arduino.h>
#include <Servo.h>
#include "sorter.h"

//sorter constants (moved from constants.h)
const int ORANGE = 30;
const int PICK_UP = 150;
const int WHITE = 85;


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


Servo arm;
int blue, white;
const char LEFT = 'L';
const char RIGHT = 'R';
const int LEFT_POSITION = 45;
const int RIGHT_POSITION = 135;
const int CENTER_POSITION = 90;
const int SORTER_DELAY = 500;

bool moveSorterArm(const char dir){
  static bool atCenter = true;
  if(atCenter){
    if(dir == LEFT){
      arm.write(LEFT_POSITION);
      atCenter = false;
    } else {
      arm.write(RIGHT_POSITION);
      atCenter = false;
    }
  }
  else{
    if(delayState(SORTER_DELAY)){
      arm.write(CENTER_POSITION);
      atCenter = true;
      return true;
    }
  }
  return false;
}

bool sortBall() {
  if(blue > 3000 && white > 6000){
    return moveSorterArm(LEFT);
  } 
  else{
    return moveSorterArm(RIGHT);
  }
}

bool collectBalls() {
  static bool sorting = false;
  if(sorting) {
    sorting = !sortBall();
  } else {
    sorting = white > 6000;
  }
}
