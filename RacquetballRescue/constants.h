#ifndef CONSTANTS

#define CONSTANTS

//speed constants
const int HALF_SPEED = 70;
const int FULL_SPEED = 120;
const int MAX_SPEED = 80;
const int LINE_STRICTNESS = 20;

//Joel constants
const int TARGET_INDEX = 4;
const int TURN_AMOUNT  = 3;

//message constants
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

const int DO_DUMP = 70;//ok
const int DONT_DUMP = 147;//ok

const int WALL_CLOSE = 2800;

const int ROBOT_CLOSE = 150; //<150
const int ROBOT_GONE = 500;// >500
const int ROBOT_OBSTRUCT = 630; // <350

#endif
