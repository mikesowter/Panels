// end of quarter processing

void qtrProc() {

/*  if (numSamp[0] < 3) return;  // for boot too close to quarter change
//  average readings
  avgTemp[0] = sumTemp[0]/(float)numSamp[0];
  avgTemp[1] = sumTemp[1]/(float)numSamp[1];
  avgTemp[2] = sumTemp[2]/(float)numSamp[2];  */

  diagMess("qtr processing");

  t0=millis();
  watchDog = 0;
  yield();

  if ( day() != oldDay ) {
    delay(5000);
    getTime();
    setTime(startSeconds);

    // generate new file name for day
    strcpy(todayName,"/pm");
    strcat(todayName,p2d(year()%100));
    strcat(todayName,p2d(month()));
    strcat(todayName,p2d(day()));
    strcat(todayName,".csv");
//    storeData();
    // update month and year
    oldYear = year();
    oldMonth = month();
    oldDay = day();
  }
  oldHour = hour();
  oldQtr = minute()/15;
  // flush fault files
  fd.flush();
  fe.flush();
}
