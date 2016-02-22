/*
 * File:   HV_control.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef HV_CONTROL_H
#define	HV_CONTROL_H

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <string.h>
#include <math.h>
#include <vector>
#include <ctime>

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "Keithley2410.h"
#include "RunCounter.h"

using namespace std;

namespace HV_CONTROL{
	struct HV_control
	{
		bool updown;
		int Steps;
		double V_1;
		double V_2;
		double I_compliance1;
		double I_compliance2;
		int Wait_ms;
		int Wait_ms_ramp;
		string SerialFileKeithley;
		string SerialFileKeithley1;
		
		HV_control():
		updown(true),
		Steps(10),
		V_1(0),
		V_2(0),
		I_compliance1(2E-5),
		I_compliance2(2E-5),
		Wait_ms(500),
		Wait_ms_ramp(500),
		SerialFileKeithley("SMUSettings/Keithley.xml"),
		SerialFileKeithley1("SMUSettings/Keithley1.xml")
		{}
	};
	class Controller
	{
	public:
		Controller(const std::string& pFilename, const std::string& pOutFilename, bool updown, volatile sig_atomic_t& stop):
		Data_name("")
		{
			fOutFilename=pOutFilename;
			ParseSettingsXML(pFilename);
			ramp(updown,stop);
		}
		~Controller() {
			ivFile.close();
			ivFile1.close();
		}
		// methods
	private:
		std::string fOutFilename, Data_name;
		std::fstream ivFile, ivFile1;
		pugi::xml_document cFile;
		vector<string> datavec;
		vector<string> datavec1;
		HV_control fHV_control;
	protected:
		void ParseSettingsXML( const std::string& pFilename, std::ostream& os = std::cout);
		void FileGenerator();
		void Tokenizer(vector< string >& datavec, string& datastring, boost::char_separator<char> sep);
		string IntToHexStr(int x);
		void ramp(bool up, volatile sig_atomic_t& stop);
	};
};

#endif


