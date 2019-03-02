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
bool doPickup = false;

//mission globals
int missionNum = 1;
char redPath[40];
char neutralPath[40];
char redPickup[40];
char neutralPickup[40];
int neutralSteps;
int redSteps;


int sensorCounter = 0;

int pickupIndex = 0;
int redIndex = 0;
int neutralIndex = 0;
int racquetballIndex =0;
PT6961 display(DIN, CLOCK, CS);



#endif
