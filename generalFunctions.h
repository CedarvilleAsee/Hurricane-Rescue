int readFrontSensor() {
   return analogRead(FRONT_SENSOR);
}
int readForkSensor() {
   return analogRead(FORK_SENSOR);
}

int readClaw() {
   return analogRead(CLAW_SENSOR);
}

bool atIntersection() {
  return readForkSensor() > AT_FORK;
}

/*
int readRightClaw() {
   return analogRead(L_CLAW_SENSOR);
}
 
int readLeftClaw() {
   return analogRead(L_CLAW_SENSOR);
}
*/

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



