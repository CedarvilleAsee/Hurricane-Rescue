#ifndef CONSTANTS

#define CONSTANTS

//speed constants
const int SLOW_SPEED = 50;
const int HALF_SPEED = 50;
const int FULL_SPEED = 70;
const int MAX_SPEED = 80;

//Joel constants
const int TARGET_INDEX = 4;
const int TURN_AMOUNT  = 3;

//message constants
const int PICKUP_EMPTY = 0;
const int PICKUP_RIGHT = 1;
const int PICKUP_LEFT = 2;
const int DEPOSITING = 3;
const int DONE = 4;
const int RACQUETBALL = 5;


//direction constants
const char L = 'l';
const char R = 'r';
const char F = 'f';
const char B = 'b';
const char E = '0';

// Dasch constants
const bool WHEEL_FORWARDS   = true;
const bool WHEEL_BACKWARDS  = false;

const int DO_DUMP = 53;//ok
const int DONT_DUMP = 15;//ok

const int PERSON_CLOSE_RIGHT = 90;//ok

const int WALL_CLOSE = 400;
const int AT_FORK = 2500;

const int RIGHT_ARM_UP = 130;//ok
const int RIGHT_ARM_START = 100;//ok
const int RIGHT_ARM_DOWN = 44;//ok

const int LEFT_ARM_UP = 130;
const int LEFT_ARM_START = 100;
const int LEFT_ARM_DOWN = 17;

const int RIGHT_CLAW_OPEN = 100;//ok
const int RIGHT_CLAW_CLOSE = 71;//ok

const int LEFT_CLAW_OPEN = 80;
const int LEFT_CLAW_CLOSE = 52;


#endif
