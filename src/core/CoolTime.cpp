/**
 *  Copyright (c) 2018 La Cool Co SAS
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 */

#include "FS.h"

#include "Arduino.h"

#include "CoolTime.h"

#include "ArduinoJson.h"

#include "TimeLib.h"

#define DEBUG 0

/**
 *  CoolTime::begin():
 *  This method is provided to init
 *  the udp connection
 *
 */
void CoolTime::begin() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.begin()"));
  Serial.println();

#endif

  Udp.begin(localPort);

  this->update();
}

/**
 *  CoolTime::offGrid:
 *  This method is provided to init
 *  the udp connection
 *
 */
void CoolTime::offGrid() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.offGrid()"));
  Serial.println();

#endif
  if (compileTime == 1 && NTP == 0) {
    char posMarker = 0;
    for (int i = 0; i <= sizeof(__TIMESTAMP__); i++) {
      if (__TIMESTAMP__[i] == ':') {
#if DEBUG == 1
        Serial.print("position of ':' : ");
        Serial.println(i);
#endif
        posMarker = i;
        break;
      }
    }
    char monthAbbr[4] = {__TIMESTAMP__[4], __TIMESTAMP__[5], __TIMESTAMP__[6],
                         0};

    char tempDay[3] = {__TIMESTAMP__[8], __TIMESTAMP__[9], 0};
    int Day = atoi(&tempDay[0]);

    char tempHour[3] = {__TIMESTAMP__[posMarker - 2],
                        __TIMESTAMP__[posMarker - 1], '\0'};
    int Hour = atoi(&tempHour[0]);

    char tempMinute[3] = {__TIMESTAMP__[posMarker + 1],
                          __TIMESTAMP__[posMarker + 2], '\0'};
    int Minute = atoi(&tempMinute[0]);

    char tempSecond[3] = {__TIMESTAMP__[posMarker + 4],
                          __TIMESTAMP__[posMarker + 5], '\0'};
    int Second = atoi(&tempSecond[0]);

    char tempYear[3] = {
        __TIMESTAMP__[posMarker + 9], __TIMESTAMP__[posMarker + 10],
        '\0'}; //__TIMESTAMP__[posMarker + 7],__TIMESTAMP__[posMarker + 8],
    int Year = atoi(&tempYear[0]);
    int Month;

    if (strstr(monthAbbr, "Jan")) {
      // Serial.println(F("Month January"));
      Month = 1;
    }
    if (strstr(monthAbbr, "Feb")) {
      // Serial.println(F("Month February"));
      Month = 2;
    }
    if (strstr(monthAbbr, "Mar")) {
      // Serial.println(F("Month March"));
      Month = 3;
    }
    if (strstr(monthAbbr, "Apr")) {
      // Serial.println(F("Month April"));
      Month = 4;
    }
    if (strstr(monthAbbr, "May")) {
      // Serial.println(F("Month May"));
      Month = 5;
    }
    if (strstr(monthAbbr, "Jun")) {
      // Serial.println(F("Month June"));
      Month = 6;
    }
    if (strstr(monthAbbr, "Jul")) {
      // Serial.println(F("Month July"));
      Month = 7;
    }
    if (strstr(monthAbbr, "Aug")) {
      // Serial.println(F("Month August"));
      Month = 8;
    }
    if (strstr(monthAbbr, "Sep")) {
      // Serial.println(F("Month September"));
      Month = 9;
    }
    if (strstr(monthAbbr, "Oct")) {
      // Serial.println(F("Month October"));
      Month = 10;
    }
    if (strstr(monthAbbr, "Nov")) {
      // Serial.println(F("Month november"));
      Month = 11;
    }
    if (strstr(monthAbbr, "Dec")) {
      // Serial.println(F("Month december"));
      Month = 12;
    }
    // tmElements_t tm;
    setDateTime(y2kYearToTm(Year), Month, Day, Hour, Minute, Second);
    unsigned long instantTime = RTC.get(CLOCK_ADDRESS);
    this->timeSync = instantTime;
    this->compileTime = 0;
    saveTimeSync();

#if DEBUG == 1
    Serial.print(F("compileTime : "));
    Serial.println(__TIMESTAMP__);
    Serial.print(F("Month Abbrevation : "));
    Serial.println(monthAbbr);
    Serial.print(F("Day : "));
    Serial.println(Day);
    Serial.print(F("Month : "));
    Serial.println(Month);
    Serial.print(F("Year : "));
    Serial.println(Year);
    Serial.print(F("Hour : "));
    Serial.println(Hour);
    Serial.print(F("Minute : "));
    Serial.println(Minute);
    Serial.print(F("Seconds : "));
    Serial.println(Second);
#endif
    Serial.println("RTC set from __TIMESTAMP__");
    Serial.print("Seconds since 1970 : ");
    Serial.println(instantTime);
  }
}

/**
 *  CoolTime::update():
 *  This method is provided to correct the
 *  rtc Time when it drifts,once every week.
 */
void CoolTime::update() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.update()"));
  Serial.println();

#endif
  if (this->NTP == 1 && WiFi.status() == WL_CONNECTED) { // ensure that NTP is accessible!!!
    if (this->timePool == -1) {
      this->timePool = timePoolConfig();
    }
    if (!(this->isTimeSync())) {

#if DEBUG == 1

      Serial.println(F("waiting for sync"));
      Serial.println();

#endif
      // give 5 trys 
      int repeats = 0;
      this->timeSync = this->getNtpTime();
      // check if we got an answer, if not repeat
      while (this->timeSync == 0) {
        delay(1000);
        this->timeSync = this->getNtpTime();
        // if after 5 trys it doesn't worked, check the NTP pools and give it a last try..
        if (repeats >= 4) {
          timePoolConfig();
          delay(500);
          this->timeSync = this->getNtpTime();
          break;
        }
        repeats++;
      }
      breakTime(this->timeSync, this->tmSet);
      this->rtc.set(makeTime(this->tmSet), CLOCK_ADDRESS); // set the clock
      this->saveTimeSync();
    }
  }
}

/**
 *  CoolTime::setDateTime(year,month,dat,hour,minutes,seconds):
 *  This method is provided to manually set the RTc Time
 *
 */
void CoolTime::setDateTime(int year, int month, int day, int hour, int minutes,
                           int seconds) {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.setDateTime"));
  Serial.println();

#endif

  tmElements_t tm;
  tm.Second = seconds;
  tm.Minute = minutes;
  tm.Hour = hour;
  tm.Day = day;
  tm.Month = month;
  tm.Year = year;
  this->rtc.set(makeTime(tm), CLOCK_ADDRESS);

#if DEBUG == 1

  Serial.print(F("setting time to : ")); //"20yy-mm-ddT00:00:00Z

  Serial.print(tm.Year);
  Serial.print(F("-"));
  Serial.print(this->formatDigits(tm.Month));
  Serial.print(F("-"));
  Serial.print(this->formatDigits(tm.Day));
  Serial.print(F("T"));
  Serial.print(this->formatDigits(tm.Hour));
  Serial.print(F(":"));
  Serial.print(this->formatDigits(tm.Minute));
  Serial.print(F(":"));
  Serial.print(this->formatDigits(tm.Second));
  Serial.print(F("Z"));

  Serial.println();

  Serial.print(F("time set to : "));
  Serial.println(this->getESDate());
  Serial.println();

#endif
}

/**
 *  CoolTime::getTimeDate():
 *  This method is provided to get the RTC Time
 *
 *  \returns a tmElements_t structre that has
 *  the time in it
 */
tmElements_t CoolTime::getTimeDate() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.getTimeDate()"));
  Serial.println();

#endif

  tmElements_t tm;
  rtc.get(CLOCK_ADDRESS); // experimental to prevent slow rtc data
  delay(50);
  time_t timeDate = this->rtc.get(CLOCK_ADDRESS);
  breakTime(timeDate, tm);

#if DEBUG == 1

  Serial.print(F("time is : "));
  Serial.print(tm.Year + 1970);
  Serial.print(F("-"));
  Serial.print(this->formatDigits(tm.Month));
  Serial.print(F("-"));
  Serial.print(this->formatDigits(tm.Day));
  Serial.print(F("T"));
  Serial.print(this->formatDigits(tm.Hour));
  Serial.print(F(":"));
  Serial.print(this->formatDigits(tm.Minute));
  Serial.print(F(":"));
  Serial.print(this->formatDigits(tm.Second));
  Serial.print(F("Z"));

#endif

  return (tm);
}

/**
 *  CoolTime::getESD():
 *  This method is provided to return an
 *  Elastic Search compatible date Format
 *
 *  \return date String in Elastic Search
 *  format
 */
String CoolTime::getESDate() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.getESDate()"));
  Serial.println();

#endif

  tmElements_t tm = this->getTimeDate();

  //"20yy-mm-ddT00:00:00Z"
  String elasticSearchString =
      String(tm.Year + 1970) + "-" + this->formatDigits(tm.Month) + "-";

  elasticSearchString +=
      this->formatDigits(tm.Day) + "T" + this->formatDigits(tm.Hour) + ":";

  elasticSearchString +=
      this->formatDigits(tm.Minute) + ":" + this->formatDigits(tm.Second) + "Z";

#if DEBUG == 1

  Serial.print(F("elastic Search date : "));
  Serial.println(elasticSearchString);
  Serial.println();

#endif

  return (elasticSearchString);
}

/**
 *  CoolTime::getLastSyncTime():
 *  This method is provided to get the last time
 *  we syncronised the time
 *
 *  \return unsigned long representation of
 *  last syncronisation time in seconds
 */
unsigned long CoolTime::getLastSyncTime() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.getLastSyncTime()"));
  Serial.println();

  Serial.print(F("last sync time : "));
  Serial.println(this->timeSync);

#endif

  return (this->timeSync);
}

/**
 *  CoolTime::isTimeSync( time in seconds):
 *  This method is provided to test if the
 *  time is syncronised or not.
 *  By default we test once per week.
 *
 *  \return true if time is syncronised,false
 *  otherwise
 */
bool CoolTime::isTimeSync(unsigned long seconds) {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.isTimeSync() "));
  Serial.println();

#endif

  // expermental to prevent slow rtc data
  RTC.get(CLOCK_ADDRESS);
  unsigned long instantTime = RTC.get(CLOCK_ADDRESS);

#if DEBUG == 1
  Serial.print(F("Instant Time : "));
  Serial.println(instantTime);
  Serial.print(F("Last Sync    : "));
  Serial.println(this->getLastSyncTime());
  unsigned long testSync = instantTime - this->timeSync;
  Serial.print(F("Time since sy : "));
  Serial.println(testSync);
#endif

  // default is once per week we try to get a time update

  if ((instantTime - this->timeSync) > (seconds)) {

    Serial.println(F("time is not syncronised "));

    return (false);
  }

  Serial.println(F("time is syncronised : OK"));
  Serial.println();

  return (true);
}

/**
 *  CoolTime::getNtpTime():
 *  This method is provided to get the
 *  Time through an NTP request to
 *  a Time Server
 *
 *  \return a time_t (unsigned long ) timestamp in seconds
 */
time_t CoolTime::getNtpTime() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.getNtpTime()"));
  Serial.println();

#endif

  WiFi.hostByName(timeServer[timePool], timeServerIP);
  if (timeServerIP[0] == 0 && timeServerIP[1] == 0 && timeServerIP[2] == 0 && timeServerIP[3] == 0) {
    Serial.printf("No IP for Host %s, do benchmark and check back later... \n\n",timeServer[timePool]);
    //timePoolConfig();
    return 0;
  } else {

#if DEBUG == 1

    Serial.println(timeServer[timePool]);
    Serial.println(timeServerIP);

#endif

    Serial.println(F("Transmit NTP Request"));

    while (Udp.parsePacket() > 0)
    ; // discard any previously received packets
    sendNTPpacket(timeServerIP);

    uint32_t beginWait = millis();

    while (millis() - beginWait < TIMEOUT) {
      int size = Udp.parsePacket();
      if (size >= NTP_PACKET_SIZE) {
        Serial.println(F("Receive NTP Response"));
        Serial.printf("latency : %ld ms \n", millis() - beginWait);
        //break;
        Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
        unsigned long secsSince1900;
        // convert four bytes starting at location 40 to a long integer
        secsSince1900 = (unsigned long)packetBuffer[40] << 24;
        secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
        secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
        secsSince1900 |= (unsigned long)packetBuffer[43];
      
      #if DEBUG == 1
      
        Serial.print(F("received unix time : "));
        Serial.println(secsSince1900 - 2208988800UL);
        Serial.println();
      
      #endif
      
        return secsSince1900 - 2208988800UL;
      }
    }
  }

  Serial.println(F("No NTP Response :-("));

  return 0; // return 0 if unable to get the time
}

/**
 *  CoolTime::sendNTPpacket( Time Server IP address):
 *  This method is provided to send an NTP request to
 *  the time server at the given address
 */
void CoolTime::sendNTPpacket(IPAddress &address) {

#if DEBUG == 1

  Serial.println(F("Enter CoolTime.sendNTPpacket()"));

#endif

  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0;          // Stratum, or type of clock
  packetBuffer[2] = 6;          // Polling Interval
  packetBuffer[3] = 0xEC;       // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}


/**
 *  CoolTime::config():
 *  This method is provided to configure
 *  the CoolTime object through a configuration
 *  file.
 *
 *  \return true if successful,false otherwise
 */
bool CoolTime::config() {

#if DEBUG == 1

  Serial.println(F("Enter CoolTime.config()"));
  Serial.println();

#endif

  File rtcConfig = SPIFFS.open("/rtcConfig.json", "r");

  if (!rtcConfig) {

    Serial.println(F("failed to read /rtcConfig.json"));
    Serial.println();

    return (false);
  } else {
    size_t size = rtcConfig.size();
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    rtcConfig.readBytes(buf.get(), size);
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.parseObject(buf.get());
    if (!json.success()) {

      Serial.println(F("failed to parse rtcConfig json"));
      Serial.println();

      return (false);
    } else {

#if DEBUG == 1

      Serial.println(F("configuration json is :"));
      json.printTo(Serial);
      Serial.println();

      Serial.print(F("jsonBuffer size: "));
      Serial.println(jsonBuffer.size());
      Serial.println();

#endif
      if (json["timePool"].success()) {

        this->timePool = json["timePool"];
      } else {
        this->timePool = this->timePool;
      }
      json["timePool"] = this->timePool;

      if (json["timeSync"].success()) {

        this->timeSync = json["timeSync"];
      } else {
        this->timeSync = this->timeSync;
      }
      json["timeSync"] = this->timeSync;

      if (json["NTP"].success()) {

        this->NTP = json["NTP"].as<bool>();
      } else {
        this->NTP = this->NTP;
      }
      json["NTP"] = this->NTP;

      if (json["compileTime"].success()) // Get the compile Flag and immediatly
                                         // reset it to prevent re-reading on a
                                         // later startup
      {

        this->compileTime = json["compileTime"].as<bool>();
      } else {
        this->compileTime = this->compileTime;
      }
      json["compileTime"] = this->compileTime;

      rtcConfig.close();
      rtcConfig = SPIFFS.open("/rtcConfig.json", "w");

      if (!rtcConfig) {

#if DEBUG == 1

        Serial.println(F("failed to write to /rtcConfig.json"));
        Serial.println();

#endif

        return (false);
      }

      json.printTo(rtcConfig);
      rtcConfig.close();

#if DEBUG == 1

      Serial.println(F("configuration is :"));
      json.printTo(Serial);
      Serial.println();

#endif

      return (true);
    }
  }
}

/**
 *  CoolTime::saveTimeSync()
 *  This method is provided to save
 *  the last sync time in the
 *  SPIFFS.
 *
 *  \return true if successful,false
 *  otherwise
 */
bool CoolTime::saveTimeSync() {
  Serial.println(F("Enter CoolTime.saveTimeSync()"));
  Serial.println();

  File rtcConfig = SPIFFS.open("/rtcConfig.json", "r");

  if (!rtcConfig) {
    Serial.println(F("failed to read /rtcConfig.json"));
    Serial.println();

    return (false);
  } else {
    size_t size = rtcConfig.size();
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    rtcConfig.readBytes(buf.get(), size);
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.parseObject(buf.get());
    if (!json.success()) {

      Serial.println(F("failed to parse json"));
      Serial.println();

      return (false);
    } else {

#if DEBUG == 1

      Serial.println(F("configuration json is :"));
      json.printTo(Serial);
      Serial.println();

      Serial.print(F("jsonBuffer size: "));
      Serial.println(jsonBuffer.size());
      Serial.println();

#endif
      if (json["timePool"].success()) {
        json["timePool"] = this->timePool;
      } else {
        this->timePool = this->timePool;
      }
      json["timePool"] = this->timePool;

      if (json["timeSync"].success()) {
        json["timeSync"] = this->timeSync;
      } else {
        this->timeSync = this->timeSync;
      }
      json["timeSync"] = this->timeSync;

      if (json["NTP"].success()) {

        json["NTP"] = this->NTP;
      } else {
        this->NTP = this->NTP;
      }
      json["NTP"] = this->NTP;

      if (json["compileTime"].success()) {

        json["compileTime"] = this->compileTime;
      } else {
        this->compileTime = this->compileTime;
      }
      json["compileTime"] = this->compileTime;

      rtcConfig.close();
      rtcConfig = SPIFFS.open("/rtcConfig.json", "w");

      if (!rtcConfig) {
#if DEBUG == 1

        Serial.println(F("failed to write timeSync to /rtcConfig.json"));
        Serial.println();

#endif

        return (false);
      }

      json.printTo(rtcConfig);
      rtcConfig.close();

#if DEBUG == 1

      Serial.println(F("configuration is :"));
      json.printTo(Serial);
      Serial.println();

#endif
      return (true);
    }
  }
}

/**
 *  CoolTime::printConf():
 *  This method is provided to print
 *  the CoolTime configuration to the
 *  Serial Monitor
 */
void CoolTime::printConf() {

#if DEBUG == 1

  Serial.println(F("Entering CoolTime.printConf()"));
  Serial.println();

#endif

  Serial.println(F("RTC Configuration"));

  Serial.println(F("Time Server List "));
  for (int i = 1; i < SERVERCOUNT; i++ ){
    Serial.println(timeServer[i]);
  }

  Serial.print(F("NTP Flag :"));
  Serial.println(NTP);

  Serial.print(F("compileTime Flag :"));
  Serial.println(compileTime);
}

/**
 *  CoolTime::printDigits(digit)
 *
 *  utility method for digital clock display
 *  adds leading 0
 *
 *  \return formatted string of the input digit
 */
String CoolTime::formatDigits(int digits) {

#if DEBUG == 1

// Serial.println( F("Entering CoolTime.formatDigits()") );
// Serial.println();

#endif

  if (digits < 10) {

#if DEBUG == 1

  // Serial.println( F("output digit : ") );
  // Serial.println( String("0") + String(digits) );

#endif

    return (String("0") + String(digits));
  }

#if DEBUG == 1

    // Serial.println( F("output digit : ") );
    // Serial.println(digits);

#endif

  return (String(digits));
}

/**
 *  CoolTime::timePoolConfigl()
 *
 *  utility method for chosing the server with the best ping
 *  returns 0 if it fails or returns the number of the const char* timeServer[]
 *
 *  \return formatted string of the input digit
 */
int CoolTime::timePoolConfig() {

#if DEBUG == 1
  Serial.println();
  Serial.println(F("Enter timePoolConfig"));
  Serial.println();
#endif

  Serial.println(F("Please wait, performing NTP server test..."));
  unsigned long latency[SERVERCOUNT];
  bool timeout[SERVERCOUNT];
  for (int i = 0; i < SERVERCOUNT; i++) {
    latency[i] = 0;
    timeout[i] = false;
  }

  for (int j = 1; j <= NTP_OVERSAMPLE; j++) {
  //check all servers to get the fastest
    for (int i = 0; i < SERVERCOUNT; i++) {
      while (Udp.parsePacket() > 0) {
        ; // discard any previously received packets
      }
      WiFi.hostByName(timeServer[i], timeServerIP);
      // check if we got a valid IP
      if (timeServerIP[0] == 0 && timeServerIP[1] == 0 && timeServerIP[2] == 0 && timeServerIP[3] == 0) {
        Serial.printf("No IP for Host %s, setting Timeout and check next...\n\n",timeServer[i]);
        timeout[i] = true;
        //break;
      } else {

#if DEBUG == 1
        Serial.println(timeServer[i]);
        Serial.println(timeServerIP);
        Serial.println(F("Transmit NTP Request"));
#endif

        sendNTPpacket(timeServerIP);

        uint32_t beginWait = millis();

        while ((millis() - beginWait) < (TIMEOUT + 200)) {
          int size = Udp.parsePacket();
          if (size >= NTP_PACKET_SIZE) {
            latency[i] += (millis() - beginWait);

#if DEBUG == 1
            Serial.print(F("Receive NTP Response from "));
            Serial.println(timeServer[i]);
            Serial.printf("latency : %ld ms \n \n", latency[i] );
#endif

            break;
          }
          if ((millis() - beginWait) >= TIMEOUT) {
            timeout[i] = true;

  #if DEBUG == 1
            Serial.println(F("Hit Timeout !"));
            Serial.println();
  #endif
            break;
          }
        }
      }
    }
  }

  //compare values and if we run into timeout...
  unsigned long temp = 0;
  int result = -1;
  //get first value 
  if (latency[0] != 0 && !timeout[0]) {
    temp = latency[0];
    result = 0;
  }
  //compare if a other server was faster, check also if we hit timeout
  for (int i = 0; i < SERVERCOUNT; i++) {
    if ((latency[i] != 0) && !timeout[i] && (latency[i] < temp)){
      temp = latency[i];
      result = i;
    }
  }
  Serial.println();

#if DEBUG == 1  
  Serial.print(F("Timeout Flag : "));
    for (int i = 0; i < SERVERCOUNT; i++) {
    Serial.print(timeout[i]);
  }
  Serial.println();
#endif

  Serial.println();
  Serial.println(F("Latency test finished !"));
  Serial.printf("\nresult : %s \n",timeServer[result]);
  Serial.printf("latency : %ld ms \n\n", latency[result] / NTP_OVERSAMPLE);
  return result;
}