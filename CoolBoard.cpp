/**
*	\file	CoolBoard.cpp
*  	\brief	CoolBoard Source file
*	\author	Mehdi Zemzem
*	\version 1.0
*  	\date	27/06/2017
*
*
*/

#include "FS.h"
#include "CoolBoard.h"
#include "ArduinoJson.h"
#include "Arduino.h"
#include <memory>

#define DEBUG 1




/**
*	CoolBoard::CoolBoard():
*	This Constructor is provided to start
*	the I2C interface and Init the different
*	used pins
*/
CoolBoard::CoolBoard()
{

#if DEBUG == 1

	Serial.println( F("Entering CoolBoard Constructor") );
	Serial.println();

#endif
	
	Wire.begin(2, 14);                       //I2C init 

	pinMode(EnI2C, OUTPUT);		   //Declare I2C Enable pin 

}


/**
*	CoolBoard::begin():
*	This method is provided to configure and
*	start the used CoolKit Parts.
*	It also starts the first connection try
*	If Serial is enabled,it prints the configuration
*	of the used parts.
*/
void CoolBoard::begin()
{

#if DEBUG == 1

	Serial.println( F("Starting the CoolBoard  ")  );
	Serial.println( F("Entering CoolBoard.begin() ")  );
	Serial.println();
#endif	

#if DEBUG == 0
	Serial.println( F("Starting Coolboard..."));
#endif


	delay(100);
	
	coolBoardLed.write(255,128,0);//orange

	this->initReadI2C();
	delay(50);

	coolBoardSensors.config();
	coolBoardSensors.begin();
	delay(100);
	
	wifiManager.config();
	wifiManager.begin();
	delay(100);

	mqtt.config();
	mqtt.begin();
	delay(100);

#if DEBUG == 1

	coolBoardLed.printConf();
	coolBoardSensors.printConf();
	wifiManager.printConf();
	mqtt.printConf();

#endif


	if (jetpackActive)
	{
		jetPack.config();
		jetPack.begin();
		#if DEBUG == 1
			jetPack.printConf();
		#endif
		delay(100);
	}

	if (ireneActive)
	{
		irene3000.config();
		irene3000.begin();
		#if DEBUG == 1
			irene3000.printConf();
		#endif
		delay(100);
	}

	if (externalSensorsActive)
	{
		externalSensors.config();
		externalSensors.begin();
		#if DEBUG == 1
			externalSensors.printConf();
		#endif
		delay(100);
	}
	
	coolBoardLed.fadeOut(255,128,0,0.5);//orange

	this->connect();
	delay(100);

	rtc.config();
	rtc.begin();
	#if DEBUG == 1
		rtc.printConf();
	#endif
	delay(100);
	
	coolBoardLed.blink(0,255,0,0.5);//green

}

/**
*	CoolBoard::isConnected()
*	
*	This method is provided to check
*	if the card is connected to Wifi and MQTT
*
*	\return	 0 : connected
*		-1: Wifi Not Connected
*		-2: MQTT Not Connected
*			
*/
int CoolBoard::isConnected()
{

#if DEBUG == 1	

	Serial.println( F("Entering CoolBoard.isConnected ") );
	Serial.println();

#endif
	if (wifiManager.state() != WL_CONNECTED)
	{
	
		Serial.println(F("Wifi Not Connected"));

	#if DEBUG == 1

		Serial.println(F("Wifi State is "));
		Serial.println(wifiManager.state());
		
	#endif
		return(-1);
	}
	
	if(mqtt.state() != 0)
	{
		
		Serial.println( F("MQTT not Connected"));

	#if DEBUG==1
		Serial.println( F("mqtt state is :") );
		Serial.println(mqtt.state());	
	
	#endif

	}
	
	return(0);

}


/**
*	CoolBoard::connect():
*	This method is provided to manage the network
*	connection and the mqtt connection.
*
*	 \return mqtt client state
*/
int CoolBoard::connect()
{

#if DEBUG == 1	

	Serial.println( F("Entering CoolBoard.connect ") );
	Serial.println();
	Serial.println( F("Connecting the CoolBoard  ") );
	delay(100);

#endif
	coolBoardLed.write(0,0,255);//blue

	
			
	
#if DEBUG == 1		

	Serial.println( F("Launching CoolWifi") );
	Serial.println();

#endif
	wifiManager.connect();
	delay(100);


	//only attempt MQTT connection when Wifi is Connected
	if (wifiManager.state() == WL_CONNECTED)
	{

	#if DEBUG == 1	
	
		Serial.println( F("Launching mqtt.connect()") );
		Serial.println();
	
	#endif	
		//logInterval in seconds
		mqtt.connect(this -> getLogInterval());
		delay(100);
	}
	
		
	
	
#if DEBUG == 1

	Serial.println( F("mqtt state is :") );
	Serial.println(mqtt.state());
	Serial.println();
	delay(100);

#endif

	coolBoardLed.blink(0,0,255,0.5);//blue

	return(mqtt.state());
}

/**
*	CoolBoard::onLineMode():
*	This method is provided to manage the online
*	mode:	-update clock
*		-read sensor
*		-do actions
*		-publish data
*		-read answer
*		-update config
*/
void CoolBoard::onLineMode()
{

	coolBoardLed.fadeIn(128,255,50,0.5);//shade of green

#if DEBUG == 1

	Serial.println( F("Entering CoolBoard.onLineMode() ") );
	Serial.println();

#endif
#if DEBUG == 0

	Serial.println( F("CoolBoard is in Online Mode"));

#endif

	data="";
	answer="";

	//send saved data if any
	if(fileSystem.isDataSaved())
	{

		coolBoardLed.fadeIn(128,128,255,0.5);//shade of blue

		Serial.println( F("There is data saved on the File System") );
		Serial.println( F("Sending saved data over MQTT ") );
		Serial.println();
		coolBoardLed.strobe(128,128,255,0.5);//shade of blue 

		mqtt.publish("sending saved data");
		mqtt.mqttLoop();


		
		int size=0;
		std::unique_ptr<String[]> savedData(std::move(fileSystem.getSensorSavedData(size)));//{..,..,..}

		int i=0;
		//loop through the array
		while(i<size)
		{
			//formatting data:
		
			String jsonData = "{\"state\":{\"reported\":";
			jsonData += savedData[i]; // {"state":{"reported":{..,..,..,..,..,..,..,..}
			jsonData += " } }"; // {"state":{"reported":{..,..,..,..,..,..,..,..}  } }

		#if DEBUG == 1 
			Serial.println(F("Size is : "));
			Serial.println(size);
			Serial.print(F("sending line N°"));
			Serial.println(i);
			Serial.println(jsonData);
			Serial.println();

		#endif

			coolBoardLed.strobe(128,128,255,0.5);//shade of blue
		
			mqtt.publish( jsonData.c_str() );
			mqtt.mqttLoop();
		
			coolBoardLed.fadeOut(128,128,255,0.5);//shade of blue
			
			i++;
			yield();
		}		


	#if DEBUG == 1

		Serial.println( F("Saved data sent ") );
		Serial.println();
	
	#endif

	}

	coolBoardLed.blink(128,255,50,0.5);//shade of green

	//clock update
	Serial.println( F("Re-checking RTC..."));
	rtc.update();

	//read user data if user is active
	if(userActive)
	{
		coolBoardLed.fadeIn(245,237,27,0.5);//shade of yellow
	
	#if DEBUG == 1

		Serial.println( F("User is Active") );
		Serial.println( F("Collecting User's data ( mac,username,timeStamp )") );
		Serial.println();
	
	#endif	
		coolBoardLed.blink(245,237,27,0.5);//shade of yellow	

		//reading user data
		data=this->userData();//{"":"","":"","",""}

		//formatting json 
		data.setCharAt( data.lastIndexOf('}') , ',');//{"":"","":"","","",
				
		//read sensors data
	#if DEBUG == 1

		Serial.println( F("Collecting sensors data ") );
		Serial.println();
	
	#endif

		data+=this->readSensors();//{"":"","":"","","",{.......}		

		//formatting json correctly
		data.remove(data.lastIndexOf('{'), 1);//{"":"","":"","","",.......}
		
		coolBoardLed.fadeOut(245,237,27,0.5);//shade of yellow
				
	}	
	else
	{
		//read sensors data
	#if DEBUG == 1

		Serial.println( F("Collecting sensors data ") );
		Serial.println();
	
	#endif
		coolBoardLed.fade(190,100,150,0.5);//shade of violet		
		data=this->readSensors();//{..,..,..}
	}
	
	//do action
	if (jetpackActive)
	{


	#if DEBUG ==1

		Serial.println( F("jetpack is Active ") );
		Serial.println( F("jetpack doing action ") );
		Serial.println();

	#endif
		coolBoardLed.fade(100,100,150,0.5);//dark shade of blue		
		jetPack.doAction(data.c_str());
	}
	
	coolBoardLed.fadeIn(128,255,50,0.5);//shade of green

	//formatting data:
	String jsonData = "{\"state\":{\"reported\":";
	jsonData += data; // {"state":{"reported":{..,..,..,..,..,..,..,..}
	jsonData += " } }"; // {"state":{"reported":{..,..,..,..,..,..,..,..}  } }
	
	//mqtt client loop to allow data handling
	mqtt.mqttLoop();

	coolBoardLed.blink(128,255,50,0.5);//shade of green	

	//read mqtt answer
	answer = mqtt.read();

#if DEBUG == 1 

	Serial.println( F("checking if there's an MQTT message ")  );
	Serial.println( F("answer is : ") );	
	Serial.println(answer);	
	Serial.println();

#endif	

	coolBoardLed.fadeOut(128,255,50,0.5);//shade of green	

	//check if the configuration needs update 
	//and update it if needed 
	this -> update(answer.c_str());
	
	coolBoardLed.fadeIn(128,255,50,0.5);//shade of green	

	//publishing data	
	if( this->sleepActive==0 )	
	{	
		coolBoardLed.strobe(255,0,230,0.5);//shade of pink
		
		//logInterval in seconds
		mqtt.publish( jsonData.c_str(), this->getLogInterval() );
		mqtt.mqttLoop();
	
	}
	else
	{
		coolBoardLed.strobe(230,255,0,0.5);//shade of yellow	

		mqtt.publish(jsonData.c_str());		
		mqtt.mqttLoop();
		answer = mqtt.read();
		this ->update(answer.c_str());

		//logInterval in seconds
		this->sleep( this->getLogInterval() ) ;
	}

	coolBoardLed.fadeOut(128,255,50,0.5);//shade of green		

	mqtt.mqttLoop();

	//read mqtt answer
	answer = mqtt.read();
	this -> update(answer.c_str());	

	coolBoardLed.blink(128,255,50,0.5);//shade of green	


}


/**
*	CoolBoard::offlineMode():
*	This method is provided to manage the offLine
*	mode:	-read sensors
*		-do actions
*		-save data in the file system
*		-if there is WiFi but no Internet : make data available over AP
*		-if there is no connection : retry to connect
*/
void CoolBoard::offLineMode()
{
	coolBoardLed.fade(51,100,50,0.5);//dark shade of green	
#if DEBUG == 1	
	
	Serial.println( F("Entering off line mode ") );	
	
#endif

#if DEBUG == 0

	Serial.println( F("CoolBoard is in Offline Mode"));

#endif

	//read user data if user is active
	if(userActive)
	{

		coolBoardLed.fadeIn(245,237,27,0.5);//shade of yellow

	#if DEBUG == 1
		
		Serial.println( F("User is Active") );
		Serial.println( F("Collecting User's data ( mac,username,timeStamp )") );
		Serial.println();

	#endif

		coolBoardLed.blink(245,237,27,0.5);//shade of yellow	

		//reading user data
		data=this->userData();//{"":"","":"","",""}

		//formatting json 
		data.setCharAt( data.lastIndexOf('}') , ',');//{"":"","":"","","",
		
				
		//read sensors data

		Serial.println( F("Collecting sensors data ") );
		Serial.println();

		data+=this->readSensors();//{"":"","":"","","",{.......}

		

		//formatting json correctly
		data.remove(data.lastIndexOf('{'), 1);//{"":"","":"","","",.......}

		coolBoardLed.fadeOut(245,237,27,0.5);//shade of yellow
				
	}	
	else
	{
		//read sensors data
	#if DEBUG == 1

		Serial.println( F("Collecting sensors data ") );
		Serial.println();

	#endif

		coolBoardLed.fade(190,100,150,0.5);//shade of violet		

		data=this->readSensors();//{..,..,..}
	}

	coolBoardLed.fade(51,100,50,0.5);//dark shade of green	

	//do action
	if (jetpackActive)
	{
	


	#if DEBUG == 1

		Serial.println( F("jetpack is Active ") );
		Serial.println( F("jetpack doing action ") );
		Serial.println();
	
	#endif
		coolBoardLed.fade(100,100,150,0.5);//dark shade of blue	
	
		jetPack.doAction( data.c_str() );
	}
	
	coolBoardLed.fade(51,100,50,0.5);//dark shade of green	
	
	//saving data in the file system
	
	fileSystem.saveSensorData( data.c_str() );

	#if DEBUG == 0

		Serial.println( F("saving Data in Memory : OK"));

	#endif

	coolBoardLed.fadeOut(51,100,50,0.5);//dark shade of green

	//case we have wifi but no internet
	if( (wifiManager.state() == WL_CONNECTED) && ( mqtt.state()!=0 ) )
	{
		
		Serial.println(F("there is Wifi but no Internet"));
		Serial.println(F("lunching AP to check saved files"));
		Serial.println(F("and Add new WiFi if needed"));
		
		wifiManager.connectAP();
		
	}
	
	//case we have no connection at all
	if( wifiManager.state() != WL_CONNECTED )
	{
	
	#if DEBUG == 1
		
		Serial.println(F("there is No Wifi "));
		Serial.println(F("retrying to connect"));
	
	#endif

	#if DEBUG == 0
		Serial.println( F("there is no WiFi..."));
	#endif
		
		this->connect();//nomad case : just run wifiMulti
				//normal case : run wifiMulti+AP
		
	}	

}

/**
*	CoolBoard::config():
*	This method is provided to configure
*	the CoolBoard :	-log interval
*			-irene3000 activated/deactivated
*			-jetpack activated/deactivated
*			-external Sensors activated/deactivated
*			-mqtt server timeout
*
*	\return true if configuration is done,
*	false otherwise
*/
bool CoolBoard::config()
{

#if DEBUG == 1

	Serial.println( F("Entering CoolBoard.config() ") );
	Serial.println();

#endif
#if DEBUG == 0
	Serial.println();
	Serial.println( F("Loading configuration for this CoolBoard..."));
#endif 

	//open file system
	fileSystem.begin();
	
	//start the led
	coolBoardLed.config();
	coolBoardLed.begin();
	coolBoardLed.fadeIn(243,171,46,0.5);//shade of orange		

	
	//open configuration file
	File configFile = SPIFFS.open("/coolBoardConfig.json", "r");
	
	if (!configFile)

	{
	
		Serial.println( F("failed to read /coolBoardConfig.json  ") );

		coolBoardLed.blink(255,0,0,0.5);//shade of red		
		return(false);
	}

	else
	{
		size_t size = configFile.size();

		// Allocate a buffer to store contents of the file.
		std::unique_ptr < char[] > buf(new char[size]);

		configFile.readBytes(buf.get(), size);

		DynamicJsonBuffer jsonBuffer;

		JsonObject & json = jsonBuffer.parseObject(buf.get());

		if (!json.success())
		{
		
			Serial.println( F("failed to parse CoolBoard Config json object ") );
	
			coolBoardLed.blink(255,0,0,0.5);//shade of red		
			return(false);
		}

		else
		{	
		
		#if DEBUG == 1
			
			Serial.println( F("configuration json : ") );
			json.printTo(Serial);
			Serial.println();
			
			Serial.print(F("jsonBuffer size : "));
			Serial.print(jsonBuffer.size());
			Serial.println();

		#endif
			
			//parsing userActive Key
			if (json["userActive"].success())
			{
				this -> userActive = json["userActive"];
			}

			else
			{
				this -> userActive = this -> userActive;
			}
			json["userActive"] = this -> userActive;

			//parsing logInterval key
			if (json["logInterval"].success())
			{
				this -> logInterval = json["logInterval"];
			}
			else
			{
				this -> logInterval = this -> logInterval;
			}
			json["logInterval"] = this -> logInterval;
			
			//parsing ireneActive key			
			if (json["ireneActive"].success())
			{
				this -> ireneActive = json["ireneActive"];
			}
			else
			{
				this -> ireneActive = this -> ireneActive;
			}
			json["ireneActive"] = this -> ireneActive;
			
			//parsing jetpackActive key
			if (json["jetpackActive"].success())
			{
				this -> jetpackActive = json["jetpackActive"];
			}
			else
			{
				this -> jetpackActive = this -> jetpackActive;
			}
			json["jetpackActive"] = this -> jetpackActive;

			//parsing externalSensorsActive key
			if (json["externalSensorsActive"].success())
			{
				this -> externalSensorsActive = json["externalSensorsActive"];
			}
			else
			{
				this -> externalSensorsActive = this -> externalSensorsActive;
			}
			json["externalSensorsActive"] = this -> externalSensorsActive;

			
			//parsing sleepActive key
			if (json["sleepActive"].success())
			{
				this -> sleepActive = json["sleepActive"];
			}
			else
			{
				this -> sleepActive = this -> sleepActive;
			}
			json["sleepActive"] = this -> sleepActive;

			//saving the current/correct configuration
			configFile.close();
			configFile = SPIFFS.open("/coolBoardConfig.json", "w");
			if (!configFile)
			{
			
				Serial.println( F("failed to write to /coolBoardConfig.json") );
				Serial.println();

 				coolBoardLed.blink(255,0,0,0.5);//shade of red		
				return(false);
			}

			json.printTo(configFile);
			configFile.close();
			#if DEBUG == 0

				Serial.println( F("Configuration loaded : OK"));
				Serial.println();

			#endif

			return(true);
		}
	}

	coolBoardLed.strobe(243,171,46,0.5);//shade of orange
	
	coolBoardLed.fadeOut(243,171,46,0.5);//shade of orange				
}

/**
*
*	CoolBoard::printConf():
*	This method is provided to print
*	the configuration to the Serial
*	Monitor.
*/
void CoolBoard::printConf()
{

#if DEBUG == 1
	
	Serial.println( F("Entering CoolBoard.printConf() ") );
	Serial.println();

#endif

	Serial.println( F("Printing Cool Board Configuration "));
	Serial.print( F("log interval 		: "));
	Serial.println(this->logInterval);

	Serial.print( F("irene active 		: "));
	Serial.println(this->ireneActive);

	Serial.print( F("jetpack active		: "));
	Serial.println(this->jetpackActive);

	Serial.print( F("external sensors active 	: "));
	Serial.println(this->externalSensorsActive);

	Serial.print( F("sleept active 		: "));
	Serial.println(this->sleepActive);

	Serial.print( F("user active 		: "));
	Serial.println(this->userActive);

	Serial.println();




}

/**
*	CoolBoard::update(mqtt answer):
*	This method is provided to handle the
*	configuration update of the different parts
*/
void CoolBoard::update(const char * answer)
{
	coolBoardLed.fadeIn(153,76,0,0.5);//shade of brown		

#if DEBUG == 1

	Serial.println( F("Entering CoolBoard.update() ") );
	Serial.println();
	Serial.println( F("message is : ") );
	Serial.println(answer);
	Serial.println();

#endif

	DynamicJsonBuffer jsonBuffer;
	JsonObject & root = jsonBuffer.parseObject(answer);
	JsonObject & stateDesired = root["state"];

#if DEBUG == 1

	Serial.println( F("root json : ") );
	root.printTo(Serial);
	Serial.println();

	Serial.println( F("stateDesired json : "));
	stateDesired.printTo(Serial);
	Serial.println();
	
	Serial.print( F("jsonBuffer size : "));
	Serial.println(jsonBuffer.size());

#endif

	if (stateDesired.success())
	{
	
	#if DEBUG == 1

		Serial.println( F("update message parsing : success") );
		Serial.println();
	
	#endif

			String answerDesired;
		
			stateDesired.printTo(answerDesired);
			
		#if DEBUG == 1		
		
			Serial.println( F("update is ok ") );
			Serial.println( F("desired update is : ") );			
			Serial.println(answerDesired);
			Serial.println("json size is : ");
			Serial.println(jsonBuffer.size() ) ;				
			Serial.println();

		
		#endif
			//saving the new configuration
			fileSystem.updateConfigFiles(answerDesired);

		        //answering the update msg:
			//reported = received configuration
			//desired=null
		
			String updateAnswer;
			String tempString;
			
			stateDesired.printTo(tempString);
			updateAnswer="{\"state\":{\"reported\":";
			updateAnswer+=tempString;
			updateAnswer+=",\"desired\":null}}";

		#if DEBUG == 1

			Serial.println( F("preparing answer message ") );
			Serial.println();
			Serial.println( F("updateAnswer : ") );
			Serial.println(updateAnswer);
		
		#endif	

			mqtt.publish(updateAnswer.c_str());
			
			mqtt.mqttLoop();

			delay(10);
			
			//restart the esp to apply the config
			ESP.restart();
	}
	else
	{
	
	#if DEBUG == 1

		Serial.println( F("Failed to parse update message( OR no message received )") );
		Serial.println();
	
	#endif
	
	}

	coolBoardLed.strobe(153,76,0,0.5);//shade of brown
	coolBoardLed.fadeOut(153,76,0,0.5);//shade of brown								
}

/**
*	CoolBoard::getLogInterval():
*	This method is provided to get
*	the log interval
*
*	\return interval value in s
*/
unsigned long CoolBoard::getLogInterval()
{

#if DEBUG == 1

	Serial.println( F("Entering CoolBoard.getLogInterval() ") );
	Serial.println();
	Serial.println( F("log Interval is :") );
	Serial.println(logInterval);
	Serial.println();

#endif

	return(this -> logInterval);
}

/**
*	CoolBoard::readSensors():
*	This method is provided to read and
*	format all the sensors data in a
*	single json.
*	
*	\return	json string of all the sensors read.
*/
String CoolBoard::readSensors()
{

	coolBoardLed.fadeIn(128,255,0,0.5);//light shade of green
				
#if DEBUG == 1

	Serial.println( F("Entering CoolBoard.readSensors()") );
	Serial.println();

#endif
	coolBoardLed.strobe(128,255,0,0.5);//light shade of green

	String sensorsData;
	
	this->initReadI2C();

	sensorsData = coolBoardSensors.read(); // {..,..,..}
	
	if (externalSensorsActive)
	{
		sensorsData += externalSensors.read(); // {..,..,..}{..,..}

		sensorsData.setCharAt(sensorsData.lastIndexOf('}'), ','); // {..,..,..}{..,..,
		sensorsData.setCharAt(sensorsData.lastIndexOf('{'), ','); // {..,..,..},..,..,
		sensorsData.remove(sensorsData.lastIndexOf('}'), 1); // {..,..,..,..,..,
		sensorsData.setCharAt(sensorsData.lastIndexOf(','), '}'); // {..,..,..,..,..}

	}
	if (ireneActive)
	{
		sensorsData += irene3000.read(); // {..,..,..,..,..}{..,..,..}

		sensorsData.setCharAt(sensorsData.lastIndexOf('}'), ','); // {..,..,..,..,..}{..,..,..,
		sensorsData.setCharAt(sensorsData.lastIndexOf('{'), ','); // {..,..,..,..,..},..,..,..,
		sensorsData.remove(sensorsData.lastIndexOf('}'), 1); // {..,..,..,..,..,..,..,..,
		sensorsData.setCharAt(sensorsData.lastIndexOf(','), '}'); // {..,..,..,..,..,..,..,..}		
		
		
	}

	//getting Hour:
	tmElements_t tm;
	tm=rtc.getTimeDate();
	
	//adding Hour
	sensorsData.remove(sensorsData.lastIndexOf('}'), 1); // {..,..,..,..,..,..,..,..,	
	sensorsData+=",\"hour\":";	
	sensorsData+=tm.Hour;
	sensorsData+="}";
	
#if DEBUG == 1
	Serial.println();
	Serial.println( F("sensors data is ") );
	Serial.println(sensorsData);
	Serial.println();

#endif
	coolBoardLed.fadeOut(128,255,0,0.5);//light shade of green

	return(sensorsData);

}

/**
*	CoolBoard::initReadI2C():
*	This method is provided to enable the I2C
*	Interface.
*/
void CoolBoard::initReadI2C()
{

#if DEBUG == 1

 	Serial.println( F("Entering CoolBoard.initReadI2C()") );
	Serial.println();

#endif
 
	digitalWrite(EnI2C,HIGH);//HIGH= I2C Enable

}


/**
*	CoolBoard::userData():
*	This method is provided to
*	return the user's data.
*	
*	\return json string of the user's data
*/
String CoolBoard::userData()
{

#if DEBUG == 1

	Serial.println( F("Entering CoolBoard.userData() ") );
	Serial.println();

#endif

	String tempMAC = WiFi.macAddress();

	tempMAC.replace(":", "");

	String userJson = "{\"user\":\"";

	userJson += mqtt.getUser();

	userJson += "\",\"timestamp\":\"";

	userJson += rtc.getESDate(); // "timestamp":"20yy-mm-ddThh:mm:ssZ"

	userJson += "\",\"mac\":\"";

	userJson += tempMAC;

	userJson += "\"}";

#if DEBUG == 1

	Serial.println( F("userData is : ") );
	Serial.println(userJson);
	Serial.println();

#endif	
	
	return(userJson);
	
}


/**
*	CoolBoard::sleep(int interval):
*	This method is provided to allow the
*	board to enter deepSleep mode for
*	a period of time equal to interval in s 
*/
void CoolBoard::sleep(unsigned long interval)
{

	Serial.println( F("Entering CoolBoard.sleep() ") );
	Serial.print( F("going to sleep for ") );
	Serial.print(interval);
	Serial.println(F("s") );
	Serial.println();
	
	//interval is in seconds , interval*1000*1000 in µS
	ESP.deepSleep ( ( interval * 1000 * 1000 ), WAKE_RF_DEFAULT) ;

}



