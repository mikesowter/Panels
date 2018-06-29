#include <Arduino.h>

byte storeData() {
  strcpy(fileName,todayName);
  if ( !openFile("a") ) return 0;
  WriteQtr();
  fh.close();
  diagMess(fileName);
  return 1;
}

//----------------- open file for reading or appending

byte openFile(char* s) {
  fh = SPIFFS.open(fileName, s);
  if (!fh) {
    strcpy(outBuf,fileName);
    strcat(outBuf," failed to open");
    diagMess(outBuf);
    return 0;
  }
  return 1;
}

void WriteQtr() {
  fh.print(p2d(hour()));
  fh.print(":");
  fh.print(p2d(minute()));
  fh.print(",");
  fh.print(avgTemp[0]);
  fh.print(",");
  fh.print(avgTemp[1]);
  fh.print(",");
  fh.print(avgTemp[2]);
  fh.print(",");
  fh.println(waterLevel);
  yield();
}
