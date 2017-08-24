/**
*	\file CoolTime.h
*	\brief CoolTime Header File
*	\author Mehdi Zemzem
*	\version 1.0
*	\date 27/06/2017
*	\copyright La Cool Co SAS 
*	\copyright MIT license
*	Copyright (c) 2017 La Cool Co SAS
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*
*/

#ifndef CoolTime_H
#define CoolTime_H


#include "Arduino.h"

#include "TimeLib.h"

#include <WiFiUdp.h>

#include <DS1337RTC.h>

#define NTP_PACKET_SIZE  48 // NTP time is in the first 48 bytes of message

/**
*	\class CoolTime
*  
*	\brief This class manages the DS1337 RTC .
*  
*/

class CoolTime
{

public:
	void begin();
	
	void update();
	
	bool config();

	void config(IPAddress timeServer,unsigned int localPort); 
	
	void printConf();

	void setDateTime(int year, int month, int day, int hour, int minutes, int seconds);
	
	tmElements_t getTimeDate();

	String getESDate();
	
	unsigned long getLastSyncTime();
	
	bool isTimeSync(unsigned long seconds=604800);

	time_t getNtpTime();

	void sendNTPpacket(IPAddress &address);
	
	String formatDigits(int digits);
	
	bool saveTimeSync();

private:
	
	unsigned long timeSync=0;
	
	IPAddress timeServer; // time-a.timefreq.bldrdoc.gov
	
	WiFiUDP Udp;
	
	unsigned int localPort=0;  // local port to listen for UDP packets

	byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
	
	tmElements_t tmSet;
	
	DS1337RTC rtc;

};

#endif