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

  if(!SPIFFS.format()||!SPIFFS.begin())     //use to format SPIFFS drive
  //if(!SPIFFS.begin())
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

  ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
  ads.begin();                  // ads is on an I2C bus at address 0x48
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
    scan1wire();
    oldMin = minute();
  }
  // check for OTA
  ArduinoOTA.handle();
  // read currents
  ads.setGain(GAIN_SIXTEEN);    // FS 0.256V @ .025ohm 10A 1 bit = 0.3mA
  amps1 = ads.readADC_SingleEnded(0)*ampScale1;
  diag(amps1);
  amps2 = ads.readADC_SingleEnded(1)*ampScale2;
  diag(amps1);
  // read volts
  ads.setGain(GAIN_TWO);        // FS 2.048V x332k/1k8 360V 1 bit = 1.2mV
  volts1 = ads.readADC_SingleEnded(2)*voltScale1;
  diag(volts1);
  volts2 = ads.readADC_SingleEnded(3)*voltScale2;
  diag(volts2);
//  Serial.println();
  delayMicroseconds(3650);      // rounds up to 40ms total sampling interval
}

void diag(float val) {
  return;
  Serial.print(millis());
  Serial.print(",");
  Serial.print(val);
  Serial.print(", ");
}
