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

#ifndef CoolWifi_H
#define CoolWifi_H

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

/**
 *  \class CoolWifi
 *
 *  \brief This class manages the WiFi connection .
 *
 */

class CoolWifi {

public:
  void begin();

  void config(bool printConf);

  //bool config(String ssid[], String pass[], int wifiNumber, int APTimeOut);

  wl_status_t connect();

  wl_status_t connectWifiMulti();

  wl_status_t connectAP(String ssid_a[], String pass_a[]);

  wl_status_t state();

  wl_status_t disconnect();

  String getExternalIP();

  void printConf(String ssid[], uint8_t wifiNumber, uint8_t timeOut_v);

  bool addWifi(String ssid, String pass, String ssid_a[], String pass_a[]);

  bool start_to_connect = false;
  /**
   *  number of saved WiFi's
   */
  uint8_t wifiCount = 0;

private:
  /**
   *  ESP8266WiFiMulti instance
   */
  ESP8266WiFiMulti wifiMulti;

  /**
   *  WiFi SSID Array
   */
 // String ssid[50] = {"0"};

  /**
   *  WiFi PASS Array
   */
 // String pass[50] = {"0"};

  /**
   *  WiFi Access Point TimeOut Value in Seconds
   */
  uint8_t timeOut = 180;
};

#endif
