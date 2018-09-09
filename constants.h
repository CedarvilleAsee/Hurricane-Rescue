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
const char FORWARD = 'f';
const char BACK = 'b';
const char EMPTY = '0';
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

const int DO_DUMP = 50;
const int DONT_DUMP = 100;

const int R_OPEN = 50;
const int R_CLOSE = 100;

const int L_OPEN = 50;
const int L_CLOSE = 100;

const int PERSON_CLOSE = 10;

const int R_UP_POS = 100;
const int R_DOWN_POS = 50;

const int L_UP_POS = 100;
const int L_DOWN_POS = 50;



#endif
