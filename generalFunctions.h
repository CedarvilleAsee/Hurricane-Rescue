int readFrontSensor() {
   return analogRead(FRONT_SENSOR);
}
int readForkSensor() {
   return analogRead(FORK_SENSOR);
}

bool isAtWall() {
  return readFrontSensor() < WALL_CLOSE;
}

bool atIntersection() {
  return readForkSensor() > AT_FORK;
}

int readRightClaw() {
   return analogRead(CLAW_SENSOR_RIGHT);
}
 
int readLeftClaw() {
   return analogRead(CLAW_SENSOR_LEFT);
}


bool delayState(int ms) {
  static int milliseconds = -1;
  if(milliseconds == -1) {
    milliseconds = millis();
  }
  else if(millis() - milliseconds >= ms) {
    milliseconds = -1;
    return true;
  }
  return false;
}



