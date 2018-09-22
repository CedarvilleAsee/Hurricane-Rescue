//display
const int CS    = PC13;
const int CLOCK = PC14;
const int DIN   = PC15;

//old servos
    #define DUMP_SERVO    PA8
    #define EJECT_SERVO   PA10
    #define ARM_SERVO     PA9
    #define CLAW_SERVO    PB6

// Buttons
const int BUTTON_2 = PB4;//stopbutton
const int BUTTON_1 = PB5;//gobutton

// Wheels
const int WHEEL_DIR_LB = PB14;//BIN1
const int WHEEL_DIR_LF = PB15;//BIN2
const int WHEEL_DIR_RB = PB13;//AIN1
const int WHEEL_DIR_RF = PB12;//AIN2

const int WHEEL_SPEED_L = PB1; //PWMB
const int WHEEL_SPEED_R = PB0; //PWMA

//const int WHEEL_STBY = 33;

// Line sensors
//int LINE_SENSOR[] = {PA5, PA6, PA7, PA3, PA15, PB10, PB11, PA11};
int LINE_SENSOR[] = {PA6, PA7, PB9, PA4, PA5, PA11, PB10, PB11};//least to most
//Analog

const int FRONT_SENSOR = PA0;
const int FORK_SENSOR = PA2; //or PA4?

// Sorter light
//const int SENSOR_A   = 86;
//const int SENSOR_CLK = 87;

// Servos
//const int L_DUMP   = 9;
//const int L_TUBE   = 11;
//const int R_TUBE   = 6;
//const int SORTER   = 4;
//const int SORTER = 13;
//const int RIGHT_ARM = 12;
//const int LEFT_ARM = 11;
//const int RIGHT_BIN = 10;
//const int LEFT_BIN = 9;
//const int RIGHT_ELBOW = 6;
//const int LEFT_ELBOW = 5;


