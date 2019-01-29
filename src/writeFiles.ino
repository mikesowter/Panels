#include <Arduino.h>

byte storeData() {
  strcpy(fileName,todayName);
  if ( !openFile("a") ) return 0;
  WriteQtr();
  fh.close();
  return 1;
}

//----------------- open file for reading or appending

byte openFile(const char* s) {
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
  fh.println();
  fh.print(p2d(hour()));
  fh.print(":");
  fh.print(p2d(minute()));
  fh.print(",A1:");
  fh.print(A1min);
  fh.print(",");
  fh.print(A1max);
  fh.print(",A2:");
  fh.print(A2min);
  fh.print(",");
  fh.print(A2max);
  fh.print(",Ah:"); 
  fh.print(A1hrs);
  fh.print(",");
  fh.print(A2hrs);
  fh.print(",V:");
  fh.print(V1min);
  fh.print(",");
  fh.print(V1max);
  yield();
}
