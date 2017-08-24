/**
*	\file ExternalSensors.h
*	\brief ExternalSensors Header File
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

#ifndef ExternalSensors_H
#define ExternalSensors_H


#include"Arduino.h"  

#include"ExternalSensor.h"

/**
*	\class ExternalSensors
*	\brief This class handles the external sensors
*	run time defintion , configuartion and actions
*
*/
class ExternalSensors 
{
public:

	void begin(); 

	String read ();

	bool config();
	
	bool config(String reference[],String type[],uint8_t address[],int sensorsNumber);

	void printConf();

private:
	struct sensor
	{		
		String reference="";
		String type="";
		uint8_t address=0;	
		BaseExternalSensor *exSensor=NULL;
	}sensors[50];

int sensorsNumber=0;

};

#endif