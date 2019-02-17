#ifndef CONSTANTS

#define CONSTANTS

//speed constants
const int HALF_SPEED = 50;
const int FULL_SPEED = 70;
const int MAX_SPEED = 80;
const int LINE_STRICTNESS = 20;

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

const int DO_DUMP = 53;//53 dump
const int DONT_DUMP = 15;//15 dont dump

const int PERSON_CLOSE_RIGHT = 30;//30right
const int PERSON_CLOSE_LEFT = 2600;//2600left

const int WALL_CLOSE = 300;//300 wall
const int AT_FORK = 2500;//1800 fork

const int RIGHT_ARM_UP = 157;//157 up
const int RIGHT_ARM_START = 135;//135 start
const int RIGHT_ARM_WAIT = 120;//120 wait
const int RIGHT_ARM_DOWN = 77;//77 down

const int LEFT_ARM_UP = 35;//35 up
const int LEFT_ARM_START = 55;//55 start
const int LEFT_ARM_WAIT = 72;//72 wait
const int LEFT_ARM_DOWN = 117;//117 down

const int RIGHT_CLAW_OPEN = 100;//100 open
const int RIGHT_CLAW_CLOSE = 75;//75 close

const int LEFT_CLAW_OPEN = 115;//115 open
const int LEFT_CLAW_CLOSE = 75;//85 close


#endif
