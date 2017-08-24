/**
*	\file CoolFileSystem.h
*	\brief CoolFileSystem Header File
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

#ifndef CoolFileSystem_H
#define CoolFileSystem_H


#include "Arduino.h"


/**
*	\class CoolFileSystem
*  
*	\brief This class handles the file system 
*  
*/
class CoolFileSystem
{

public:
	bool begin(); 

	bool updateConfigFiles(String answer );
	
	bool fileUpdate(String update,const char* path);	
	
	bool saveSensorData(const char* data );
	
	bool saveSensorDataCSV(const char* data );
	
	int isDataSaved();
	
	String* getSensorSavedData(int& size);
	
	bool incrementsavedData();

	void getsavedData();
		
private:
	
	int savedData=0;
	
	int linesToSkip=0;	

};

#endif