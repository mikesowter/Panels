// end of quarter processing

void qtrProc() {
  // here on restart and quarter hour
  if ( day() != oldDay ) {
    getTime();
    setTime(startSeconds);
    if (hour() != 0) {   // restart 
      resetReason.toCharArray(charBuf,resetReason.length()+1);
      diagMess(charBuf);       
    }
    // reset AmpereHours for day
    A1hrs = 0.0;
    A2hrs = 0.0;
    // generate new file name for day
    strcpy(todayName,"/so");
    strcat(todayName,p2d(year()%100));
    strcat(todayName,p2d(month()));
    strcat(todayName,p2d(day()));
    strcat(todayName,".csv");
    // update month and year
    oldYear = year();
    oldMonth = month();
    oldDay = day();
  }
  storeData();
  oldMin = minute();
  oldHour = hour();
  oldQtr = minute()/15;
  // flush fault files
  fd.flush();
  fe.flush();
}
