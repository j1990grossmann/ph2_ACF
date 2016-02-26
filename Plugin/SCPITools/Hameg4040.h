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

#include "../SCPIUtils/pugixml.hpp"
#include "../SCPIUtils/ConsoleColor.h"
#include "BufferedAsyncSerial.h"
#include "Initserial.h"


using namespace std;
// using namespace BufferedAsyncSerial;

namespace HAMEG4040{
	typedef std::vector< std::vector< double > >  HamegChannelMap;
	
	class Hameg4040: public INITSERIAL::Serial
	{
	public:
		Hameg4040(const std::string& pFilename) {
			fHamegChannelMap.resize(4, vector<double>( 2 , 0. ) );
			fHamegChannelMapCurr.resize(4, vector<double>( 2 , 0. ) );
			endline = "\n";
			ParseSettingsXML(pFilename);
// 			serial = new BufferedAsyncSerial();
// 			Initialise(serial);			
			INITSERIAL::Serial().Initialise(serial, fHamegSettingsMap);
		}
		
		~Hameg4040() {
			if(serial.isOpen())
			{
 				cout<<RED<<"Set output off Hameg4040"<<RESET<<endl;
				OutPutGen(0);
				SystLocal();
 				serial.close();			
			}else
 				cout<<GREEN<<"Nothing to do for Hameg4040"<<RESET<<endl;				
		}
		
		// methods
// 		void Initialise(BufferedAsyncSerial &serial);  // wants to be called after SystemController::ReadHW, ReadSettings
		void Configure();
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
		void MeasAllA();
		void MeasAllB();
		void MeasAllC();
		
		void GetHamegChannelMap(HamegChannelMap &fGetHamegChannelMap);
		
		
	private:
		
		BufferedAsyncSerial serial;
		HamegChannelMap  fHamegChannelMap;
		HamegChannelMap  fHamegChannelMapCurr;
		INITSERIAL::SerialSettingsMap fHamegSettingsMap;
		std::string endline = "\n";
		std::string write_str,read_str;
		
	protected:
		void WriteSynchronized(string &command);
		void WriteNotSynchronized(string &command);
		void ReadSynchronized(string &command, string &readstring);
		void ReadSynchronizedLines(string &command, string &readstring, int lines);		
		void Timeout();
		void ParseSettingsXML( const std::string& pFilename, std::ostream& os = std::cout);
		void EmptyBuffer();
		bool IDNCheck();
	};
};

#endif 
