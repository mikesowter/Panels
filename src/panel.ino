#include <panel.h>

void setup(void) {
  secondTick.attach(1,ISRwatchDog);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Solar panel monitor  29 June 2018");
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

  diagMess(" restart");

  server.on ( "/", handleMetrics );
  server.on ( "/metrics", handleMetrics );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println( "HTTP server started" );
  server.handleClient();

  ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  ads.begin();                  // ads is on an I2C bus at address 48
}

void loop(void) {
  //  reset watchdog timer
  watchDog = 0;
  //  check for change of the quarter hour
  if ( minute()/15 != oldQtr ) qtrProc();
  //  check for web requests
  server.handleClient();
  // handle background
  delay(1000);
  // poll probes
  scan1wire();
  // query water level
//  readLevel();
  // check for OTA
  ArduinoOTA.handle();
  // read ads1115
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  Serial.print("AIN0: "); Serial.print(adc0);
  Serial.print("  AIN1: "); Serial.print(adc1);
  Serial.print("  AIN2: "); Serial.print(adc2);
  Serial.print("  AIN3: "); Serial.println(adc3);
}
