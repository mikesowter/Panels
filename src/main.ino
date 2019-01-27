#include <main.h>

void setup(void) {
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println();
  Serial.println("solar panels monitor - 24 Jan 19");
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("local IP address: ");
  localIP=WiFi.localIP();
  Serial.print(localIP);
  long rssi = WiFi.RSSI();
  Serial.print("   signal strength: ");
  Serial.print(rssi);
  Serial.println(" dBm");
  init_OTA();

  udp.begin(localPort);
  // Resolve servers
  WiFi.hostByName(ntpServerName, timeServerIP);
  WiFi.hostByName(ftpServerName, fileServerIP);
  // Set epoch and timers
  getTime();
  setTime(startSeconds);
  Serial.println(timeStamp());
  //setTime(23,59,30,23,1,2017);
  startMillis = millis();
  oldMin = minute();
  oldQtr = oldMin/15;
  oldHour = hour();
  oldDay = day();
  oldMonth = month();
  oldYear = year();

  //if(!SPIFFS.format()||!SPIFFS.begin())     //use to format SPIFFS drive
  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS.begin failed");
  }
  SPIFFS.info(fs_info);
  Serial.print(fs_info.totalBytes);
  Serial.println(" bytes available");
  Serial.print(fs_info.usedBytes);
  Serial.println(" bytes used:");

  fd=SPIFFS.open("/diags.txt","a");
  fe=SPIFFS.open("/errmess.txt","a");

  resetReason.toCharArray(charBuf,resetReason.length()+1);
	diagMess(charBuf);       // restart message

  server.on ( "/", handleMetrics );
  server.on ( "/metrics", handleMetrics );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println( "HTTP server started" );
  server.handleClient();

  ads.setGain(GAIN_SIXTEEN);    // +/- 0.256V  1 bit = 0.008mV
  ads.begin();                  // ads is on an I2C bus at address 0x48

  t1 = micros();  //setup minimum period for first measurement
}

void loop(void) {
  //  reset watchdog timer
  watchDog = 0;
  //  check for change of the quarter hour
  if ( minute()/15 != oldQtr ) qtrProc();
  //  check for web requests
  server.handleClient();
  // handle background
  yield();
  // poll probes
  if (minute()!= oldMin) {
  //  scan1wire();
    oldMin = minute();
  }
  // check for OTA
  ArduinoOTA.handle(); 
  // read currents
  amps1 = ads.readADC_SingleEnded(0)*ampScale1;
  if (amps1 > 100.0) amps1 = 0.0;
  A1min = _min(A1min,amps1);
  A1max = _max(A1max,amps1);
  A1avg = 0.95*A1avg + 0.05*amps1;
  diag(amps1);
  amps2 = ads.readADC_SingleEnded(1)*ampScale2;
  if (amps2 > 100.0) amps2 = 0.0;
  A2min = _min(A2min,amps2);
  A2max = _max(A2max,amps2);
  A2avg = 0.95*A2avg + 0.05*amps2;
  diag(amps1);
  // calc Ah for each panel
  period = (float)(micros()-t1)/3.6E9;  // expressed in hours
  t1 = micros();
  A1hrs += amps1*period;
  A2hrs += amps2*period;
  // read volts
  volts1 = ads.readADC_SingleEnded(2)*voltScale1;
  if (volts1 > 20.0) volts1 = 0.0;
  V1min = _min(V1min,volts1);
  V1max = _max(V1max,volts1);
  V1avg = 0.95*V1avg + 0.05*volts1;
  diag(volts1);
  volts2 = ads.readADC_SingleEnded(3)*voltScale2;
  if (volts2 > 20.0) volts2 = 0.0;
  V2min = _min(V2min,volts2);
  V2max = _max(V2max,volts2);
  V2avg = 0.95*V2avg + 0.05*volts2;
  diag(volts2);
  Serial.println();
  delay(3);      // rounds up to 40ms total sampling interval
}

void diag(float val) {
  return;
  Serial.print(val);
  Serial.print(", ");
}
