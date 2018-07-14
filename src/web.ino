

void handleMetrics() {
  htmlStr[0]='\0';
  addCstring("\n# TYPE pmProbe1 guage" );
  addCstring("\npmProbe1 ");
  addCstring(p8d4(lastTemp[0]));
  addCstring("\n# TYPE pmProbe2 guage" );
  addCstring("\npmProbe2 ");
  addCstring(p8d4(lastTemp[1]));
  addCstring("\n# TYPE pmProbe3 guage" );
  addCstring("\npmProbe3 ");
  addCstring(p8d4(lastTemp[2]));
  addCstring("\n# TYPE pmAmps1 guage" );
  addCstring("\npmAmps1 ");
  addCstring(p8d4(amps1));
  addCstring("\n# TYPE pmAmps2 guage" );
  addCstring("\npmAmps2 ");
  addCstring(p8d4(amps2));
  addCstring("\n# TYPE pmVolts1 guage" );
  addCstring("\npmVolts1 ");
  addCstring(p8d4(volts1));
  addCstring("\n# TYPE pmVolts2 guage" );
  addCstring("\npmVolts2 ");
  addCstring(p8d4(volts2));
  addCstring("\n# TYPE pmWifiSignal guage" );
  addCstring("\npmWifiSignal ");
  addCstring(p8d4(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
}

void handleNotFound() {
  server.uri().toCharArray(userText, 14);
  Serial.println(userText);
  if (strncmp(userText,"/reset",6)==0) {
    errMess("User requested restart");
    fd.close();
    fe.close();
    ESP.restart();
  }
  else if (strncmp(userText,"/shutdown",9)==0) {
    errMess("User requested shutdown");
//    uploadDay();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Safe to Shutdown<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/deldiags",9)==0) {
    SPIFFS.remove("/diags.txt");
    fd = SPIFFS.open("/diags.txt", "a");
    dateStamp();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Diags deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (strncmp(userText,"/delerrs",9)==0) {
    SPIFFS.remove("/errmess.txt");
    fe = SPIFFS.open("/errmess.txt", "a");
    dateStamp();
    strcpy(outBuf,"<!DOCTYPE html><html><head><HR>Errors deleted<HR></head></html>");
    server.send ( 200, "text/html", outBuf );
  }
  else if (SPIFFS.exists(userText)) {
    strcpy(htmlStr,"File: ");
    addCstring(userText);
    addCstring("\r\r");
    fh = SPIFFS.open(userText, "r");

    while (fh.available()) {
      int k=fh.readBytesUntil('\r',charBuf,80);
      charBuf[k]='\0';
      addCstring(charBuf);
      yield();
    }
    fh.close();
    server.send ( 200, "text/plain", htmlStr );
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
