/*
 * File:   MuxTest.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef KEITHLEY2410_H
#define	KEITHLEY2410_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <string.h>
#include <vector>

#include "BufferedAsyncSerial.h"

using namespace std;
// using namespace BufferedAsyncSerial;

namespace KEITHLEY2410{
	typedef std::map< int, std::vector<double> >  KeithleyChannelMap;
	
	class Keithley2410
	{
	public:
		Keithley2410() {
		}
		
		// 		BufferedAsyncSerial serial("/dev/MuxTest",115200);
		~Keithley2410() {
			serial->close();
			delete serial;
		}
		
		// methods
		void Initialise();  // wants to be called after SystemController::ReadHW, ReadSettings
		void Configure();
		void ConfigureSingleRead();
		void ConfigureMultipleRead(int no_samples);
		void Reset();
		

		//Measure Methods
		void SenseVolt();
		void SenseVoltAC();
		void SenseCurr();
		void SenseCurrAC();
		void SenseRes();
		void SenseFres();
		void SenseFreq();
		void SensePer();
		void SenseTemp();
		
		void SourFuncVolt();
		void SourFuncCurr();
		void SourVoltModeFixed();
		void SourCurrModeFixed();
		void SourVoltRang(string rang);
		void SourCurrRang(string rang);
		void SourCurrLev(string lev);
		void SourVoltLev(string lev);
		
		void SenseFuncVolt();
		void SenseFuncCurr();
		void SenseCurrProt(string prot);
		void SenseVoltProt(string prot);
		void SenseCurrRang(string rang);
		void SenseVoltRang(string rang);
		
		void Outp(int i);

		void RangAuto(int onoff);
// 		void VoltAcRang(int rang);
		void TracCle();
		void Read(string &read_str);
		void InitCont(int onoff);
		void SampCoun(int no_samples);
		void TrigCoun(int no_triggers);
		void Init();
		void Fetch(string &read_str);
		void SystAzerStat(int onoff);
		void SystLSyn(int onoff);
		
// 		void MeasCurr(double &amp);
		void MeasAcVolt(double& volt);
		void TrigRetSingle(double&   value);
		void TrigRetMultiple(int no_samples, vector<double> &values);
// 		void MeasAll(HamegChannelMap &fGetHamegChannelMap);
// 		void GetHamegChannelMap(HamegChannelMap &fGetHamegChannelMap);
		
		
	private:
		
		BufferedAsyncSerial* serial;
		KeithleyChannelMap fHamegChannelMap;
		KeithleyChannelMap fHamegChannelMapCurr;
		std::string endline;
		std::string write_str,read_str;
		
	protected:
		void WriteSynchronized(string &command);
		void WriteNotSynchronized(string &command);
		void ReadSynchronized(string &command, string &readstring);
		void Timeout();
	};
};

#endif 
