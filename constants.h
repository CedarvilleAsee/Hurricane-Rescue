#ifndef CONSTANTS

#define CONSTANTS

//speed constants
const int SLOW_SPEED = 50;
const int HALF_SPEED = 50;
const int FULL_SPEED = 50;
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


//direction constants
const char L = 'l';
const char R = 'r';
const char F = 'f';
const char B = 'b';
const char E = '0';

// Dasch constants
const bool WHEEL_FORWARDS   = true;
const bool WHEEL_BACKWARDS  = false;

const int DO_DUMP = 100;
const int DONT_DUMP = 15;

const int PERSON_CLOSE = 2000;

const int WALL_CLOSE = 400; //300 for speed 50
const int AT_FORK = 2500;

const int ARM_UP = 130;
const int ARM_MIDDLE = 100;
const int ARM_DOWN = 17;

const int CLAW_OPEN = 80;
const int CLAW_CLOSE = 52;

#endif
