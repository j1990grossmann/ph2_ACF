/*
 * File:   Hameg4040.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef HAMEG4040_H
#define	HAMEG4040_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <string.h>

#include "BufferedAsyncSerial.h"

using namespace std;
// using namespace BufferedAsyncSerial;

namespace HAMEG4040{
	typedef std::map< int, std::vector<double> >  HamegChannelMap;
	
	class Hameg4040
	{
	public:
		Hameg4040() {
			for(int i=0; i<4; i++){
				fHamegChannelMapCurr[i].resize(2);
				fHamegChannelMapCurr[i].at(0)=0;
				fHamegChannelMapCurr[i].at(1)=0;
				
				fHamegChannelMap[i].resize(2);
				fHamegChannelMap[i].at(0)=0;
				fHamegChannelMap[i].at(1)=0;
			}
		}
		
		// 		BufferedAsyncSerial serial("/dev/Hameg4040",115200);
		~Hameg4040() {
			OutPutGen(0);
			SystLocal();
			serial->close();
			delete serial;
		}
		
		// methods
		void Initialise();  // wants to be called after SystemController::ReadHW, ReadSettings
		void Configure(HamegChannelMap &fSetHamegChannelMap);
		void Reset();
		
		void WriteChannelNo(int channelno, double voltage, double current);
		void SourVolt(int channelno, double voltage);
		void SourCurr(int channelno, double current);
		
		
		void InstSelect(int channelno);
		int  InstSelectQuery();
		void OutPutSel(int channelno, int outp_state);
		int  OutPutSelQuery();
		void OutPutGen(int outp_state);
		
		void SystBeeperImmediate();	
		void SystError();
		void SystLocal();
		void SystRemote();
		void SystRWLock();
		void SystMix();
		void SystVersion();
		
		
		//Measure Methods
		void MeasCurr(double &amp);
		void MeasVolt(double &volt);
		void MeasAll(HamegChannelMap &fGetHamegChannelMap);
		void GetHamegChannelMap(HamegChannelMap &fGetHamegChannelMap);
		
		
	private:
		
		BufferedAsyncSerial* serial;
		HamegChannelMap fHamegChannelMap;
		HamegChannelMap fHamegChannelMapCurr;
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