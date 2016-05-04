/*
 * File:   Simple_IV_Curves.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef SCAN_H
#define	SCAN_H

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <memory>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "Hameg4040.h"
#include "Keithley2410.h"
#include "RunCounter.h"

using namespace std;

namespace SCAN{
	struct Scanconfig
	{
		int Vcth_min;
		int Vcth_max;
		int Vcth_step;
		int No_events;
		double V_min;
		double V_max;
		double V_step;
		double dV_dt;
		double I_compliance;
		string SerialFileHameg;
		string SerialFileKeithley;
		string SerialFileKeithley1;
		std::vector <pair<int,string>> SerialConfigVec;
		
		Scanconfig():
		V_min(0),
		V_max(0),
		V_step(0),
		Vcth_min(100),
		Vcth_max(130),
		Vcth_step(5),
		dV_dt(5),
		I_compliance(1E-9),
		No_events(1000),
		SerialFileHameg    ("Plugins/SCPISettings/Hameg.xml"),
		SerialFileKeithley ("Plugins/SCPISettings/Keithley.xml"),
		SerialFileKeithley1("Plugins/SCPISettings/Keithley1.xml")
		{}
	};
	class Scan
	{
	public:
		Scan(const std::string& pFilename, const std::string& pOutFilename):
		cbc1configfilename("settings/Cbc_default_hole.txt"),
		cbc2configfilename("settings/Cbc_default_hole.txt"),
		Data_name("")
		{
			fOutFilename=pOutFilename;
			ParseSettingsXML(pFilename);
		}
		~Scan() {
			ivFile.close();
			ivFile1.close();
		}
		void StartScan(bool cIV, string cAngle, string cPosX, string cPosZ);
		void SetConfigfile(const std::string& cbc1configfilename_in, const std::string& cbc2configfilename_in)
		{
			cbc1configfilename=cbc1configfilename_in;
			cbc2configfilename=cbc2configfilename_in;
		};
		void ReadSetConfigFile(int Vcth, string configfilename);
		// methods
	private:
		std::string fOutFilename, cbc1configfilename, cbc2configfilename, Data_name;
		std::fstream ivFile, ivFile1, lvFile;
		std::vector<std::string> Outfilenames;
		pugi::xml_document cFile;
		std::fstream cbc1config;
		vector<string> datavec;
		vector<string> datavec1;
		Scanconfig fScanconfig;
	protected:
		void ParseSettingsXML( const std::string& pFilename, std::ostream& os = std::cout);
// 		void Rampdown( KEITHLEY2410::Keithley2410& k, std::ostream& os = std::cout);
		std::string exec(const char* cmd);
		void FileGenerator();
		void Tokenizer(vector< string >& datavec, string& datastring, boost::char_separator<char> sep);
		string IntToHexStr(int x);
		void preramp(bool up, KEITHLEY2410::Keithley2410&  k,KEITHLEY2410::Keithley2410&  k1);
	};
};
#endif