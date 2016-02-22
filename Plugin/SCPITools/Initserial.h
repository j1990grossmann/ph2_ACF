/*
 * File:   Hameg4040.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef INITSERIAL_H
#define	INITSERIAL_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <string.h>

#include "../../Utils/ConsoleColor.h"
#include "BufferedAsyncSerial.h"

using namespace std;
// using namespace BufferedAsyncSerial;

namespace INITSERIAL{
	typedef std::map<std::string, std::string>    SerialSettingsMap;
		
	class Serial
	{

	public:
		Serial(){
		}
		~Serial(){
		}
		// methods
		void Initialise(BufferedAsyncSerial &serial, SerialSettingsMap &fSerialSettingMap);  // wants to be called after SystemController::ReadHW, ReadSettings
		std::string endline;
	};
}

#endif 