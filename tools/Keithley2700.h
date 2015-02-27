/*
 * File:   MuxTest.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef KEITHLEY2700_H
#define	KEITHLEY2700_H

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

namespace KEITHLEY2700{
	typedef std::map< int, std::vector<double> >  ChannelMap;
	
	class Keithley2700
	{
	public:
		Keithley2700() {
		}
		
		// 		BufferedAsyncSerial serial("/dev/MuxTest",115200);
		~Keithley2700() {
			serial->close();
			delete serial;
		}
		
		// methods
		void Initialise();  // wants to be called after SystemController::ReadHW, ReadSettings
		void Configure();
		void ConfigureSingleRead();
		void ConfigureMultipleRead(int no_samples);
		void Reset();
		
// 		void InstSelect(int channelno);
// 		int  InstSelectQuery();
// 		void OutPutSel(int channelno, int outp_state);
// 		int  OutPutSelQuery();
// 		void OutPutGen(int outp_state);
// 		
// 		void SystBeeperImmediate();	
// 		void SystError();
// 		void SystLocal();
// 		void SystRemote();
// 		void SystRWLock();
// 		void SystMix();
// 		void SystVersion();
		
		
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
// 		void MeasAll(ChannelMap &fGetChannelMap);
// 		void GetChannelMap(ChannelMap &fGetChannelMap);
		
		
	private:
		
		BufferedAsyncSerial* serial;
		ChannelMap fChannelMap;
		ChannelMap fChannelMapCurr;
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