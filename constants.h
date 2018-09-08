#ifndef CONSTANTS

#define CONSTANTS

//speed constants
const int SLOW_SPEED = 70;
const int HALF_SPEED = 170;
const int FULL_SPEED = 255;

//Joel constants
const int TARGET_INDEX = 3;
const int TURN_AMOUNT  = 3;
const char LEFT = 'l';
const char RIGHT = 'r';

// Dasch constants
const int BLUETOOTH_LIMITER = 4000;
const bool WHEEL_FORWARDS   = true;
const bool WHEEL_BACKWARDS  = false;




// OLD turn sequences
const char TURN_SEQUENCE[] = {
  LEFT, RIGHT, LEFT, LEFT, LEFT, LEFT, RIGHT, RIGHT, LEFT, RIGHT, RIGHT, RIGHT, LEFT, LEFT
};

const char SECOND_TURN_SEQUENCE[] = {
  RIGHT, RIGHT, RIGHT, RIGHT
};
// end OLD turn sequences




#endif
