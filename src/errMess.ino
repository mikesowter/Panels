/*-------- display code ----------*/

void errMess(const char* mess) {
  fe.print(dateStamp());
  fe.print(" ");
  fe.print(timeStamp());
  fe.println(mess);
}

void diagMess(const char* mess) {
  fd.print(dateStamp());
  fd.print(" ");
  fd.print(timeStamp());
  fd.println(mess);
}

char* dateStamp() {
  // digital display of the date
  strcpy(dateStr,p2d(year()%100));
  strcat(dateStr,p2d(month()));
  strcat(dateStr,p2d(day()));
  return dateStr;
}

char* timeStamp() {
  // digital display of the time
  strcpy(timeStr,p2d(hour()));
  strcat(timeStr,":");
  strcat(timeStr,p2d(minute()));
  strcat(timeStr,":");
  strcat(timeStr,p2d(second()));
  strcat(timeStr," ");
  return timeStr;
}

// convert integer into a 2 char string dd
char* p2d(byte b) {
  d2Str[0]=b/10+'0';
  d2Str[1]=b%10+'0';
  return d2Str;
}

// convert float into an 8 char string ddd.dddd
char* p8d4(float f) {
  if (f > 999.9999) {
    diagMess("p8d4 overflow");
    return (char*)"999.9999";
  }
  int left = (int)f;
  int rank = 100;   //three places left of dp
  byte ptr = 0;
  bool started = false;
  for ( int n=0;n<3;n++ ) {
    if ( left/rank != 0 || n==1 ) started = true;
    if (started) d8Str[ptr++] = left/rank +'0';
    left %= rank;
    rank /= 10;
  }
  d8Str[ptr++] = '.';
  rank=1000;         //four places right of dp
  int right = (int)(10000.0*f)-10000*(int)f;
  for ( int n=0;n<4;n++ ) {
    d8Str[ptr++] = right/rank +'0';
    right %= rank;
    rank /= 10;
  }
  d8Str[ptr]='\0';
  return d8Str;
}
