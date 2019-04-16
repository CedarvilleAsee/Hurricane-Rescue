#ifndef TURNING
#define TURNING


bool turn(int spd, char dir) {
   static int lineCount = 0;
   static bool gotOffLine = false;
   if (dir == F) return true;
   if (dir == L) {
      writeToWheels(-spd, spd);
   }
   else if (dir == R || atWall) {
      writeToWheels(spd, -spd);
   }

   if (sensors[2] == LOW && sensors[3] == LOW && sensors[4] == LOW && sensors[5] ==  LOW) gotOffLine = true;
   if (dir == B && !atWall) {
      writeToWheels(-(spd), spd);
      if (sensorsCentered() && gotOffLine) { 
         // if it isn't at a wall,
         //the line sensors have to pass another line to turn completely around
         lineCount++;
         gotOffLine = false;
      }
      if (lineCount == 2) {
         lineCount = 0;
         return true;
      }
   }
   // Return true if the robot is back centered on the line

   if (gotOffLine && sensorsCentered()) {
      atWall = false;
      gotOffLine = false;
      return true;
   }
   return false;

}

bool doTurnSequence(char sequence[], int index, int maxSteps) { //need to be able to change
   int temp = index + 1; // should be acting on next instruction, not current
//check for obstruction here, before decides, if between index 1 and 5 (temp: 2 and 6)
  /* if(temp <= 6 && temp >= 2 && robotObstruct()){ //seems to drive drunkenly
      //switch to the correct array, will be temp -2
      int altInd = temp - 2;
      for(int i = 0 ; i < RACQUET_BALL_STEPS ; i ++){
        sequence[i] = RACQUET_BALL_ALTS[altInd][i];
      }
   }*/
  
   if (temp == maxSteps) {
      if (lineFollow(FULL_SPEED, 20)) {
         return true;
      }
   }
   else {
      if (turning) {
         if (turn(HALF_SPEED, sequence[temp])) {
            turning = false;
            return true;
         }
      }
      else {
         turning = lineFollow(FULL_SPEED, 20);
      }
   }
   return false;
}


#endif
