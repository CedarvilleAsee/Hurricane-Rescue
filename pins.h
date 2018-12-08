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
const int DUMP_SERVO = PA10;
const int ARM_SERVO_LEFT = PA8;
const int CLAW_SERVO_LEFT = PA9;
const int ARM_SERVO_RIGHT = PB8;
const int CLAW_SERVO_RIGHT = PB9;

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
int LINE_SENSOR[] = {PA4, PA5, PA6, PA7, PB3, PA15, PB10, PB11};
//Analog

//afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //makes PB3 work

const int FRONT_SENSOR = PA3;
const int FORK_SENSOR = PA2;

const int CLAW_SENSOR_RIGHT = PA0;
const int CLAW_SENSOR_LEFT  = PA1;

#endif
