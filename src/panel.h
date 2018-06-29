#ifndef ARDUINO_ARCH_ESP8266
#define ARDUINO_ARCH_ESP8266 1
#endif

#include <One-Wire.h>
#include "i2c.h"
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <fs.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
extern "C" {
#include "user_interface.h"
}

const uint8_t NTP_PACKET_SIZE = 48;
const uint8_t BUFFER_SIZE = 128;
const uint8_t TIME_ZONE = 10;
const uint8_t NUM_COLS = 181;
const uint8_t NUM_DAYS = 9;
const uint16_t HTML_SIZE = 13000;
const float THE_TEMP = -1.0;

ESP8266WebServer server( 80 );
WiFiUDP udp;
WiFiClient client,dclient;
time_t getNtpTime();
FSInfo fs_info;
File fh,fd,fe;
Ticker secondTick;
volatile uint8_t watchDog = 0;

char fileName[] = "/XXyymmdd.csv";
char todayName[] = "/XXyymmdd.csv";
char userText[20];
char saveName[20];
char dateStr[] = "yymmdd";
char timeStr[] = "hh:mm:ss";
unsigned long getTime();
unsigned long sendNTPrequest(IPAddress& address);
unsigned long getNTPreply();

//char ssid[] = "ZombiesAteMyBrains";   //  your network SSID (name)
char ssid[] = "TelstraCF6EC7";
char pass[] = "meauff8qqwn9";               //  your network password
char d2Str[] = "12";
char d8Str[] = "12345.78";
uint8_t Xmit[4] = {1,2,4,8};
uint8_t reply[6];
uint8_t medPtr=0;
float medBuf[64]={0.0};

unsigned long t0, t1, minMillis, startMillis, startSeconds, midNight;
unsigned int localPort = 2391;   //  a random local port to listen for UDP packets

IPAddress localIP,timeServerIP,fileServerIP;
IPAddress ip(192, 168, 1, 51);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);
const char* ntpServerName = "au.pool.ntp.org";
const char* ftpServerName = "ftp.sowter.com";
char titleData[60];
char htmlStr[HTML_SIZE];            // use C strings for storage efficiency
char outBuf[128];                   // for ftpRcv and errMessage

byte buffer[BUFFER_SIZE];
uint8_t oldMin,oldQtr,oldHour,oldDay,oldMonth;
uint16_t i,oldYear,qtrPtr,htmlLen,inputPtr,outPtr;

struct minStruct {       // all data are stored as int(100.0*float)
  uint16_t pool;
  uint16_t pump;
  uint16_t air;
  uint16_t level;
} Data[NUM_DAYS*96];     // 9 days are necessary to cover a 7.5 day chart

OneWire  ds(12);          // on GPIO12 - pin D6 on d1-mini

uint8_t probe,swimming=1;
// a little maintenance problem, so don't add probes!
const uint8_t numProbes = 3;
char sensors[numProbes][5] = {"pool","pump","air "};
uint16_t knownAddr[numProbes] = {0xFFAF,0xFF83,0xFFDB};
float lastTemp[numProbes];
float avgTemp[numProbes];
float sumTemp[numProbes];
uint16_t numSamp[numProbes];
uint8_t data[12],addr[8];
float celsius,maxTemp = 0.0;
float waterLevel,lastReading = 0.0;
bool FTP_busy = false;
