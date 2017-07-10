/**
*	\file CoolBoardLed.cpp	
*	\brief CoolBoardLed Source File
*	\author Mehdi Zemzem
*	\version 1.0
*
*/

#include "FS.h"
#include "Arduino.h"

#include <NeoPixelBus.h>
#include "CoolBoardLed.h"
#include "ArduinoJson.h"



#define DEBUG 1

#ifndef DEBUG

#define DEBUG 0

#endif



/**
*	CoolBoardLed::colorFade ( Red , Green , Blue, Time in seconds ):
*	colorFade animation:	Fade In over T(seconds)
*				Fade Out over T(seconds)
*/
void CoolBoardLed::colorFade(int R, int G, int B, int T) 
{

#if DEBUG == 1

	Serial.println("Entering CoolBoardLed.colorFade()");
	Serial.println();
	Serial.print("R : ");
	Serial.println(R);
	Serial.print("G	: ");
	Serial.println(G);
	Serial.print("B	: ");
	Serial.println(B);
	Serial.print("Time :");
	Serial.println(T);
	Serial.println();

#endif	

	for (int k = 0; k < 1000; k++) 
	{
		neoPixelLed->SetPixelColor(0, RgbColor(k * R / 1000, k * G / 1000, k * B / 1000));
		neoPixelLed->Show();
		delay(T);
	}
	for (int k = 1000; k >= 0; k--) 
	{
		neoPixelLed->SetPixelColor(0, RgbColor(k * R / 1000, k * G / 1000, k * B / 1000));
		neoPixelLed->Show();
		delay(T);
	}
}

/**
*	CoolBoardLed::blink( Red , Green , Blue , Time in seconds ):
*	Blink animation:	Led On for T seconds
				Led off
*/
void CoolBoardLed::blink(int R, int G, int B, int T) 
{

#if DEBUG == 1

	Serial.println("Entering CoolBoardLed.blink()");
	Serial.println();
	Serial.print("R : ");
	Serial.println(R);
	Serial.print("G	: ");
	Serial.println(G);
	Serial.print("B	: ");
	Serial.println(B);
	Serial.print("Time :");
	Serial.println(T);
	Serial.println();

#endif	

	neoPixelLed->SetPixelColor(0, RgbColor(R, G, B));
	neoPixelLed->Show();
	delay(T);
	neoPixelLed->SetPixelColor(0, RgbColor(0, 0, 0));
	neoPixelLed->Show();
}

/**
*	CoolBoardLed::fadeIn(Red , Green , Blue , Time in seconds)
*	Fade In animation:	gradual increase over T(seconds)
*/
void CoolBoardLed::fadeIn(int R, int G, int B, int T) 
{

#if DEBUG == 1

	Serial.println("Entering CoolBoardLed.fadeIn()");
	Serial.println();
	Serial.print("R : ");
	Serial.println(R);
	Serial.print("G	: ");
	Serial.println(G);
	Serial.print("B	: ");
	Serial.println(B);
	Serial.print("Time :");
	Serial.println(T);
	Serial.println();

#endif	

	for (int k = 0; k < 1000; k++) 
	{
		neoPixelLed->SetPixelColor(0, RgbColor(k * R / 1000, k * G / 1000, k * B / 1000));
		neoPixelLed->Show();
		delay(T);
	}
}

/**
*	CoolBoardLed::fadeOut( Red , Green , Blue , Time in seconds)
*	Fade Out animation:	gradual decrease over T(seconds)
*/
void CoolBoardLed::fadeOut(int R, int G, int B, int T) 
{

#if DEBUG == 1 

	Serial.println("Entering CoolBoardLed.fadeOut()");
	Serial.println();
	Serial.print("R : ");
	Serial.println(R);
	Serial.print("G	: ");
	Serial.println(G);
	Serial.print("B	: ");
	Serial.println(B);
	Serial.print("Time :");
	Serial.println(T);
	Serial.println();

#endif	


	for (int k = 1000; k >= 0; k--) 
	{
		neoPixelLed->SetPixelColor(0, RgbColor(k * R / 1000, k * G / 1000, k * B / 1000));
		neoPixelLed->Show();
		delay(T);
	}
}

/**
*	CoolBoardLed::strobe(Red , Green , Blue , Time in seconds)
*	Strobe animation:	blinks over T(seconds)	
*/
void CoolBoardLed::strobe(int R, int G, int B, int T) 
{

#if DEBUG == 1

	Serial.println("Entering CoolBoardLed.strobe()");
	Serial.println();
	Serial.print("R : ");
	Serial.println(R);
	Serial.print("G	: ");
	Serial.println(G);
	Serial.print("B	: ");
	Serial.println(B);
	Serial.print("Time :");
	Serial.println(T);
	Serial.println();

#endif	

	
	for (int k = 1000; k >= 0; k--) 
	{
		neoPixelLed->SetPixelColor(0, RgbColor(R, G, B));
		neoPixelLed->Show();
		delay(T);
		neoPixelLed->SetPixelColor(0, RgbColor(0, 0, 0));
		neoPixelLed->Show();
		delay(T);
	}
}

/**
* 	CoolBoardLed::end() :
*	this method is provided to delete the dynamically created neoPixelLed
*/
void CoolBoardLed::end()
{

#if DEBUG == 1 
	
	Serial.println("Entering CoolBoardLed.end()");

#endif

	delete neoPixelLed;
}


/**
*	CoolBoardLed::begin():
*	This method is provided to start the Led Object 
*	by setting the correct pin and creating a dynamic
*	neoPixelBus  
*/
void CoolBoardLed::begin( )
{

#if DEBUG == 1

	Serial.println("Entering CoolBoardLed.begin() ");

#endif

	pinMode(5,OUTPUT);
	digitalWrite(5,HIGH);
	neoPixelLed = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(1,2); 
	neoPixelLed->Begin();
	neoPixelLed->Show();
} 

/**
*	CoolBoardLed::write(Red,Green,Blue):
*	This method is provided to set the 
*	Color of the Led
*/
void CoolBoardLed::write(int R, int G, int B)
{

#if DEBUG == 1

	Serial.println("Entering CoolBoardLed.write()");
	Serial.println();
	Serial.print("R : ");
	Serial.println(R);
	Serial.print("G	: ");
	Serial.println(G);
	Serial.print("B	: ");
	Serial.println(B);
	Serial.println();	

#endif

	neoPixelLed->SetPixelColor(0, RgbColor(R, G, B));
	neoPixelLed->Show();
}

/**
*	CoolBoardLed::config():
*	This method is provided to configure
*	the Led Object :	-ledActive=0 : deactivated
*				-ledActive=1 : activated
*	\return true if the configuration done,
*	false otherwise			
*/
bool CoolBoardLed::config()
{

#if DEBUG == 1 
		
	Serial.println("Entering CoolBoardLed.config()");
	Serial.println();

#endif
	
	File coolBoardLedConfig = SPIFFS.open("/coolBoardLedConfig.json", "r");

	if (!coolBoardLedConfig) 
	{
	
	#if DEBUG == 1

		Serial.println("failed to read /coolBoardLedConfig.json");
		Serial.println();

	#endif

		return(false);
	}
	else
	{
		size_t size = coolBoardLedConfig.size();
		// Allocate a buffer to store contents of the file.
		std::unique_ptr<char[]> buf(new char[size]);

		coolBoardLedConfig.readBytes(buf.get(), size);
		DynamicJsonBuffer jsonBuffer;
		JsonObject& json = jsonBuffer.parseObject(buf.get());
		if (!json.success()) 
		{
		
		#if DEBUG == 1

			Serial.println("failed to parse json");
			Serial.println();
		
		#endif

			return(false);
		} 
		else
		{
		
		#if DEBUG == 1
  	
			Serial.println("read configuration file : ");
			json.printTo(Serial);
			Serial.println();
		
		#endif
  
			if(json["ledActive"].success() )
			{
				this->ledActive = json["ledActive"]; 
			}
			else
			{
				this->ledActive=this->ledActive;			
			}
			
			json["ledActive"]=this->ledActive;
			coolBoardLedConfig.close();
			
			coolBoardLedConfig = SPIFFS.open("/coolBoardLedConfig.json", "w");
			if(!coolBoardLedConfig)
			{
			
			#if DEBUG == 1 

				Serial.println("failed to write to /coolBoardLedConfig.json");
				Serial.println();

			#endif

				return(false);			
			}

			json.printTo(coolBoardLedConfig);
			coolBoardLedConfig.close();

		#if DEBUG == 1
	
			Serial.println("saved Led Config is : ");
			json.printTo(Serial);
			Serial.println();

		#endif

			return(true); 
		}
	}	

}				

/**
*	CoolBoardLed::printConf():
*	This method is provided to print the
*	Led Object Configuration to the Serial
*	Monitor
*/
void CoolBoardLed::printConf()
{

#if DEBUG == 1 

	Serial.println("Entering CoolBoardLed.printConf()");
	Serial.println();

#endif

	Serial.println("Led Configuration");

	Serial.print("ledActive : ");
	Serial.println(ledActive);

	Serial.println();	
}
