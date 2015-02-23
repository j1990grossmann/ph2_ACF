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

// namespace Hameg4040{
	typedef std::map< int, std::vector<double> >  ChannelMap;
	
	class Hameg4040
	{
	public:
		Hameg4040() {
			for(int i=0; i<4; i++){
				fChannelMapCurr[i].resize(2);
				fChannelMapCurr[i].at(0)=0;
				fChannelMapCurr[i].at(1)=0;
				
				fChannelMap[i].resize(2);
				fChannelMap[i].at(0)=0;
				fChannelMap[i].at(1)=0;
			}
		}
		
		// 		BufferedAsyncSerial serial("/dev/Hameg4040",115200);
		~Hameg4040() {
			OutPutGen(0);
			SystLocal();
			
			delete serial;
		}
		
		// methods
		void Initialise();  // wants to be called after SystemController::ReadHW, ReadSettings
		void Configure(ChannelMap &fSetChannelMap);
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
		void MeasAll(ChannelMap &fGetChannelMap);
		void GetChannelMap(ChannelMap &fGetChannelMap);
		
		
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
// };

#endif 