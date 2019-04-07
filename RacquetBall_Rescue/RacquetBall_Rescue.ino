#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
#include "constants.h"
#include "missions.h"
#include "PT6961.h"
#include "Adafruit_VL53L0X.h"
#include "globals.h"
#include "generalFunctions.h"
#include "display.h"
#include "lineFollowing.h"
#include "turning.h"
#include "subStates.h"


int difference;

//Don't need setup for the nextgen
void setup() {
  oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oledDisplay.display();
  oledDisplay.clearDisplay();
  oledDisplay.setTextSize(2);
  oledDisplay.setTextColor(WHITE);

  display.initDisplay();
  display.sendNum(1234, 1);




  // initialize line sensors
  for (int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }


  pinMode(FRONT_SENSOR, INPUT);
  pinMode(FORK_SENSOR, INPUT);

  // initialize motor controllers
  pinMode(WHEEL_DIR_LB, OUTPUT);
  pinMode(WHEEL_DIR_LF, OUTPUT);
  pinMode(WHEEL_DIR_RB, OUTPUT);
  pinMode(WHEEL_DIR_RF, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);

  //pinMode(WHEEL_STBY, OUTPUT); //commented out by Ian

  pinMode(BUTTON_1, INPUT_PULLUP);


  writeWheelDirection(WHEEL_FORWARDS, WHEEL_FORWARDS);
  dump.attach(DUMP_SERVO);
  dump.write(DONT_DUMP);
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //makes PB3 work

  //nextion
  Serial.begin(9600);
  laser.begin();

}

void loop() {
  static int state = 0;
  readLine();
  switch (state) {
    case 0:
      if (waitState()) state++; // picking mission
      break;
    case 1:
      displayWaiting();
      if (waitStateStart())  state++; //waits for first robot to leave
      break;
    case 2://goes into position
      while (!atIntersection()) {
        writeToWheels(40, 40);
      }
      writeToWheels(0, 0);
      while (!sensorsCentered()) {
        readLine();
        writeToWheels(-40, 40);
      }
      writeToWheels(-40, -40);
      delay(1000);
      writeToWheels(0, 0);
      state++;
      break;
    case 3:
      displayWaiting2();
      if (waitStateReturn())  state++; //wait for first robot to come back
      break;
    case 4:
      if (moveToFire())  state++; //probs done V('_')V
      break;
    case 5:
      if (putOutFire()) state++; //need to make
      break;
    default:
      if (doneState()) state = -1;
      break;
  }


}
