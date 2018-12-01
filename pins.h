#ifndef PINS
#define PINS

//display
const int CS    = PC13;
const int CLOCK = PC14;
const int DIN   = PC15;

//mission switches
const int SWITCH0 = PA12;
const int SWITCH1 = PA15; 

//servos
const int DUMP_SERVO = PA8;
const int EJECT_SERVO = PA10;
const int ARM_SERVO = PA9;
const int CLAW_SERVO = PB6;
const int RACQUET_BALL_SERVO = PB7;

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
//int LINE_SENSOR[] = {PA6, PA7, PB9, PA4, PA5, PA11, PB10, PB11};//0 to 7
int LINE_SENSOR[] = {PA3, PA4, PA5, PA6, PA7, PA11, PB3, PA12};
//Analog
//afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //makes PB3 work

const int FRONT_SENSOR = PA0;
const int FORK_SENSOR = PA2;
const int CLAW_SENSOR = PA1;

#endif
