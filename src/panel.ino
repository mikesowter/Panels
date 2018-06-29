#include <panel.h>

void setup(void) {
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Pool 2017-11-07");
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

  diagMess(" restart");

  server.on ( "/", handleMetrics );
  server.on ( "/metrics", handleMetrics );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println( "HTTP server started" );
  server.handleClient();

  i2c_init();   // ultrasonic transducer is on an I2C bus at address 8
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
  scan1wire();
  // query water level
  readLevel();
  // check for OTA
  ArduinoOTA.handle();
}
