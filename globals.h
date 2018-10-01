#ifndef GLOBALS
#define GLOBALS

int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;
Servo dump;
Servo arm;
Servo eject;
Servo claw;
bool turning = false;
bool pickingUp = false;
int missionNum = 0;
char redPath[40];
char neutralPath[40];
char redPickup[40];
char neutralPickup[40];
bool atWall = false;
int neutralSteps;
int redSteps;
int sensorCounter = 0;

int pickupIndex = 0;
int redIndex = 0;
int neutralIndex = 0;

PT6961 display(DIN, CLOCK, CS);



#endif
