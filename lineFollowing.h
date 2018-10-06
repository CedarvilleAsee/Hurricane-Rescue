#ifndef LINEFOLLOWING
#define LINEFOLLOWING

// Populates the sensors[] variable so that we know amountSeen
void readLine() {
  amountSeen = 0;
  lastLineIndex = -1;
  for(int i = 7; i >= 0; --i) {

    sensors[i] = digitalRead(LINE_SENSOR[i]);

    if(sensors[i] == HIGH) {
      if(lastLineIndex == -1) {
        lastLineIndex = i;
      }
      ++amountSeen;
      firstLineIndex = i;
    }
  }

  sensorCounter = 0;
  for (int i = 0; i < 8; i++ ){
    sensorCounter += sensors[i]<<i;
  }
  //display.sendNum(sensorCounter);
}

bool sensorsCentered() {
  return sensors[3] == HIGH && sensors[4] == HIGH;
}

void writeWheelDirection(bool ldir, bool rdir) {
  digitalWrite(WHEEL_DIR_LF, ldir);
  digitalWrite(WHEEL_DIR_LB, !ldir);
  digitalWrite(WHEEL_DIR_RF, rdir);
  digitalWrite(WHEEL_DIR_RB, !rdir);
}

void writeToWheels(int ls, int rs) {
  if(ls < 0) {
    digitalWrite(WHEEL_DIR_LF, false); //right backwards
    digitalWrite(WHEEL_DIR_LB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_LF, true); //right forwards
    digitalWrite(WHEEL_DIR_LB, false);
  }
  if(rs < 0) {
    digitalWrite(WHEEL_DIR_RF, false); //right backwards
    digitalWrite(WHEEL_DIR_RB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_RF, true); //right forwards
    digitalWrite(WHEEL_DIR_RB, false);
  }

  analogWrite(WHEEL_SPEED_L, abs(ls));
  analogWrite(WHEEL_SPEED_R, abs(rs));
}

bool lineFollow(int ts, int strictness) {

  static bool frontPassed = false;
  if(amountSeen >= TURN_AMOUNT){
    if(frontPassed == false) {
      pickupIndex++;
    }
    frontPassed = true;
    writeToWheels(ts, ts);
  }
  else {
    int rightSpeed = ts + (lastLineIndex - 4)*strictness;
    int leftSpeed = ts - (firstLineIndex - 3)*strictness;
    writeToWheels(leftSpeed, rightSpeed);
  }

  // Return true if the sensors can see a fork
  if(readFrontSensor() < WALL_CLOSE) {
    atWall = true;
    return true;
  }
  if(atIntersection() && frontPassed) {
    frontPassed = false;
    return true;
  }

  return false;
}

#endif
