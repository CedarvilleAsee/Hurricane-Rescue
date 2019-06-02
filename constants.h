#ifndef CONSTANTS

#define CONSTANTS

//speed constants
const int TURN_SPEED = 65;
const int FULL_SPEED = 100;
const int MAX_SPEED = 80;
const int LINE_STRICTNESS = 20;

const int RED_FAST_SPEED = 200;
const int RED_FAST_STRICTNESS = 20;
const int RED_REALLY_FAST_SPEED = 200;
const int RED_REALLY_FAST_STRICTNESS = 20;

//Joel constants
const int TARGET_INDEX = 4;
const int TURN_AMOUNT  = 4;

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

const int DO_DUMP = 53;
const int DONT_DUMP = 8;

const int PERSON_CLOSE_RIGHT = 1600;//2000right old
const int PERSON_CLOSE_LEFT = 1600;//1600left old

const int WALL_CLOSE = 1000;//2000 wall
const int AT_FORK = 3600;//3600 fork

const int RIGHT_ARM_UP = 159;//157 up
const int RIGHT_ARM_START = 142;//135 start
const int RIGHT_ARM_WAIT = 120;//120 wait
const int RIGHT_ARM_DOWN = 77;//77 down

const int LEFT_ARM_UP = 33;//33 up
const int LEFT_ARM_START = 48;//48 start
const int LEFT_ARM_WAIT = 72;//72 wait
const int LEFT_ARM_DOWN = 115;//115 down

const int RIGHT_CLAW_OPEN = 100;//100 open
const int RIGHT_CLAW_CLOSE = 70;//70 close

const int LEFT_CLAW_OPEN = 100;//85 open
const int LEFT_CLAW_CLOSE = 160;//160;


#endif
