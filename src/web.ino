

void handleMetrics() {
  htmlStr[0]='\0';
/*  addCstring("\n# TYPE soProbe1 guage" );
  addCstring("\nsoProbe1 ");
  addCstring(p8d4(lastTemp[0]));
  addCstring("\n# TYPE soProbe2 guage" );
  addCstring("\nsoProbe2 ");
  addCstring(p8d4(lastTemp[1]));
  addCstring("\n# TYPE soProbe3 guage" );
  addCstring("\nsoProbe3 ");
  addCstring(p8d4(lastTemp[2]));  */
  addCstring("\n# TYPE soA1avg guage" );
  addCstring("\nsoA1avg ");
  addCstring(p8d4(A1avg));
  addCstring("\n# TYPE soA1min guage" );
  addCstring("\nsoA1min ");
  addCstring(p8d4(A1min));
  addCstring("\n# TYPE soA1max guage" );
  addCstring("\nsoA1max ");
  addCstring(p8d4(A1max));
  addCstring("\n# TYPE soA2avg guage" );
  addCstring("\nsoA2avg ");
  addCstring(p8d4(A2avg));
  addCstring("\n# TYPE soA2min guage" );
  addCstring("\nsoA2min ");
  addCstring(p8d4(A2min));
  addCstring("\n# TYPE soA2max guage" );
  addCstring("\nsoA2max ");
  addCstring(p8d4(A2max));

  addCstring("\n# TYPE soV1avg guage" );
  addCstring("\nsoV1avg ");
  addCstring(p8d4(V1avg));
  addCstring("\n# TYPE soV1min guage" );
  addCstring("\nsoV1min ");
  addCstring(p8d4(V1min));
  addCstring("\n# TYPE soV1max guage" );
  addCstring("\nsoV1max ");
  addCstring(p8d4(V1max));
  addCstring("\n# TYPE soV2avg guage" );
  addCstring("\nsoV2avg ");
  addCstring(p8d4(V2avg));
  addCstring("\n# TYPE soV2min guage" );
  addCstring("\nsoV2min ");
  addCstring(p8d4(V2min));
  addCstring("\n# TYPE soV2max guage" );
  addCstring("\nsoV2max ");
  addCstring(p8d4(V2max));
 
  addCstring("\n# TYPE soA1hrs guage" );
  addCstring("\nsoA1hrs ");
  addCstring(p8d4(A1hrs));
  addCstring("\n# TYPE soA2hrs guage" );
  addCstring("\nsoA2hrs ");
  addCstring(p8d4(A2hrs));
  
  addCstring("\n# TYPE soWifiSignal guage" );
  addCstring("\nsoWifiSignal ");
  addCstring(p8d4(-WiFi.RSSI()));
  addCstring( "\n" );
  server.send ( 200, "text/plain", htmlStr );
  A1min = amps1;
  A1max = amps1;
  A2min = amps2;
  A2max = amps2;
  V1min = volts1;
  V1max = volts1;
  V2min = volts2;
  V2max = volts2;
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
