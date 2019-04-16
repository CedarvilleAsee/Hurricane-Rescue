#ifndef GLOBALS
#define GLOBALS

//Line following
int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;

//Servos
Servo dump;
Servo rightArm;
Servo rightClaw;
Servo leftArm;
Servo leftClaw;

//booleans
bool turning = false;
bool pickingUp = false;
bool atWall = false;
int  missionNum = 1;

//mission globals
char redPath[40];
char neutralPath[40];
char redPickup[40];
char neutralPickup[40];
int  neutralSteps;
int  redSteps;

int mil = 0;

int sensorCounter = 0;

int pickupIndex = 0;
int racquetballIndex =0;
PT6961 display(DIN, CLOCK, CS);
Adafruit_VL53L0X laser = Adafruit_VL53L0X();

char currentPath[RACQUET_BALL_STEPS]; 

#endif
