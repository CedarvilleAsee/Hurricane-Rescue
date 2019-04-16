int readFrontSensor() {
  return analogRead(FRONT_SENSOR);
}
bool atIntersection() {
  return digitalRead(FORK_SENSOR);
}

int readLaser() {
  VL53L0X_RangingMeasurementData_t measure;

  laser.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  return measure.RangeMilliMeter;
}

bool isAtWall() {
  return readFrontSensor() < WALL_CLOSE;//distance from wall to put out fire
}

bool robotClose() {
  return readLaser() < ROBOT_CLOSE;
}

bool robotObstruct(){ //for when the 1st robot gets stuck at next intersection
  return readLaser() < ROBOT_OBSTRUCT;
}

bool robotGone(){
  return readLaser() > ROBOT_GONE;
}

bool delayState(int ms) {
  static int milliseconds = -1;
  if (milliseconds == -1) {
    milliseconds = millis();
  }
  else if (millis() - milliseconds >= ms) {
    milliseconds = -1;
    return true;
  }
  return false;
}
