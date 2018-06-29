// end of quarter processing

void qtrProc() {

  if (numSamp[0] < 3) return;  // for boot too close to quarter change
  // average readings
  avgTemp[0] = sumTemp[0]/(float)numSamp[0];
  avgTemp[1] = sumTemp[1]/(float)numSamp[1];
  avgTemp[2] = sumTemp[2]/(float)numSamp[2];

  diagMess("");

  if ( day() == oldDay ) {
    storeData();  //store latest averages in PP file
    inputPtr = (NUM_DAYS-1)*96+4*hour()+minute()/15;  //always write to last day space
    Data[inputPtr].pool = (int)(100.0*avgTemp[0]);
    Data[inputPtr].pump = (int)(100.0*avgTemp[1]);
    Data[inputPtr].air  = (int)(100.0*avgTemp[2]);
    Data[inputPtr].level = (int)(100.0*waterLevel);
  }

  t0=millis();
  watchDog = 0;
  yield();

  // reset for new quarter hour
  for (i=0;i<numProbes;i++) {
    sumTemp[i] = 0.0;
    numSamp[i] = 0;
  }

  if ( day() != oldDay ) {
//    uploadDay();
    getTime();
    setTime(startSeconds);
    timeStamp();

    //  move data back one day for days 0-(NUM_DAYS-2)
    for (i=0;i<(NUM_DAYS-1)*96;i++) {
      Data[i].pool = Data[i+96].pool;
      Data[i].pump = Data[i+96].pump;
      Data[i].air = Data[i+96].air;
      Data[i].level = Data[i+96].level;
    }
    // and clear new day NUM_DAYS-1
    for (i=(NUM_DAYS-1)*96;i<NUM_DAYS*96;i++) {
      Data[i].pool = 0;
      Data[i].pump = 0;
      Data[i].air = 0;
      Data[i].level = 0;
    }
    inputPtr = (NUM_DAYS-1)*96;  // write midnight value to array
    Data[inputPtr].pool = (int)(100.0*avgTemp[0]);
    Data[inputPtr].pump = (int)(100.0*avgTemp[1]);
    Data[inputPtr].air = (int)(100.0*avgTemp[2]);
    Data[inputPtr].level = (int)(100.0*waterLevel);
    // generate new file name for day
    strcpy(todayName,"/PP");
    strcat(todayName,p2d(year()%100));
    strcat(todayName,p2d(month()));
    strcat(todayName,p2d(day()));
    strcat(todayName,".csv");
    storeData();
    // update month and year
    oldYear = year();
    oldMonth = month();
    oldDay = day();
  }
  oldHour = hour();
  oldQtr = minute()/15;
}
