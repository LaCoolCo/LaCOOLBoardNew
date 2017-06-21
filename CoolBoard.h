/*
*  CoolBoard.h
*  
*  This class is a wrapper.
*  It handles everything
*  
*  
*  
*  
*  
*/

#ifndef CoolBoard_H
#define CoolBoard_H


#include"CoolFileSystem.h" //CoolBoard File System Manager



#include"CoolBoardSensors.h" //CoolBoard Sensor Board Manager

#include"CoolBoardLed.h" //CoolBoard Led Manager

#include"CoolTime.h" //CoolBoard Real Time Clock Manager

#include"CoolMQTT.h"//CoolBoard MQTT Manager

#include"Jetpack.h"//CoolBoard Jetpack Manager

#include"Irene3000.h"//CoolBoard Irene3000 Manager

#include"ExternalSensors.h" //CoolBoard External Sensors Manager

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include"Arduino.h" //Arduino Defs

class CoolBoard
{

public:

	void begin(); 

	bool config();

	void update(const char*answer );

	void offLineMode();

	void onLineMode();

	bool connect();

	uint16_t getDelay();

	void printConf();

private:

	CoolFileSystem fileSystem; 

	CoolBoardSensors coolBoardSensors;

	CoolBoardLed coolBoardLed;

	CoolTime rtc;

	WiFiManager wifiManager;

	CoolMQTT mqtt;

	Jetpack jetPack;

	Irene3000 irene3000;

	ExternalSensors externalSensors;

	byte ireneActive;

	byte jetpackActive;

	byte externalSensorsActive;		



	uint16_t delay;

	int answerJsonSize;
	int sensorJsonSize;

	int serverTimeOut;

	String data;

	String answer;

	byte cnxStatus;

};

#endif