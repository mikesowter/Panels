

void handleMetrics() {
  htmlStr[0]='\0';
  addCstring("# TYPE spPoolTemp guage" );
  addCstring("\nspPoolTemp ");
  addCstring(p8d(lastTemp[0]));
  addCstring("\n# TYPE spPumpTemp guage" );
  addCstring("\nspPumpTemp ");
  addCstring(p8d(lastTemp[1]));
  addCstring("\n# TYPE spAirTemp guage" );
  addCstring("\nspAirTemp ");
  addCstring(p8d(lastTemp[2]));
  addCstring("\n# TYPE spWaterLevel guage" );
  addCstring("\nspWaterLevel ");
  addCstring(p8d(waterLevel));
  addCstring("\n# TYPE spWifiSignal guage" );
  addCstring("\nspWifiSignal ");
  addCstring(p8d(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);
  Serial.println(userText);
  if (strncmp(userText,"/reset",6)==0) {
    errMess("User requested restart");
    ESP.restart();
  }
  else if (strncmp(userText,"/shutdown",9)==0) {
    errMess("User requested shutdown");
//    uploadDay();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Safe to Shutdown<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/remdiags",9)==0) {
    SPIFFS.remove("/diags.txt");
    fd = SPIFFS.open("/diags.txt", "a");
    dateStamp();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Diags deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/remerrs",9)==0) {
    SPIFFS.remove("/errmess.txt");
    fd = SPIFFS.open("/errmess.txt", "a");
    dateStamp();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Errors deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (SPIFFS.exists(userText)) {
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Sending File: \"");
    strcat(outBuf,userText);
    strcat(outBuf,"\"<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
    strcpy(fileName,userText);
//    uploadFile();
    delay(5);
  }
  else if (strncmp(userText,"/favicon.ico",12)==0) {
  }
  else if (strncmp(userText,"/apple",6)==0) {
  }
  else if (strncmp(userText,"/dir",4)==0) {
    listFiles();
  }
  else if (strncmp(userText,"/diags",6)==0) {
    listDiags();
  }
  else {
    strcpy(outBuf,userText);
    strcat(outBuf," is not a valid option");
    errMess(outBuf);
    helpPage();
  }
}

void addCstring(const char* s) {
  // find end of htmlStr
  uint16_t p;
  for (p=0;p<HTML_SIZE;p++) {
    if ( p>HTML_SIZE-32) {
      diagMess("HTML_SIZE exceeded");
      break;
    }
    if (htmlStr[p]=='\0') {
      break;    // p now points to end of old string
    }
  }
  uint16_t q=0;
  for (;p<HTML_SIZE;p++) {
    htmlStr[p]=s[q];
//    if (s[q]!='\0') Serial.print(s[q]);
    if (s[q++]=='\0') break;
  }
  htmlLen = p;
}

float scaleCheck(uint16_t n) {
  float f = (float)n/100.0;
  if ( f == 0.0 ) {
    return 25.0;
  }
  else if ( f > 40.0) return 40.0;
  else if ( f < 0.0) return 0.0;
  return f;
}
