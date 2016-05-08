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
#include <memory>

#include "AsyncSerial.h"
#include "BufferedAsyncSerial.h"
#include "Initserial.h"
#include "../SCPIUtils/pugixml.hpp"
#include "../SCPIUtils/ConsoleColor.h"

using namespace std;
// using namespace BufferedAsyncSerial;

namespace KEITHLEY2700{
 typedef std::map< int, std::vector<double> >  KeithleyChannelMap;
 
 class Keithley2700
 {
 public:
  Keithley2700(const std::string& pFilename) {
   fFilename=pFilename;
   endline = "\n";
   ParseSettingsXML(fFilename, std::cout);
   serial=make_shared<BufferedAsyncSerial>();
   INITSERIAL::Serial().Initialise(serial, fSerialSettingsmap);
  }
  
  // 		BufferedAsyncSerial serial("/dev/MuxTest",115200);
  ~Keithley2700() {
  }
  
  // methods
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
  
  void MeasAcVolt(double& volt);
  void TrigRetSingle(double&   value);
  void TrigRetMultiple(int no_samples, vector<double> &values);

  
  
 private:
  shared_ptr<BufferedAsyncSerial>serial;
  INITSERIAL::SerialSettingsMap fSerialSettingsmap;
  std::string endline;
  std::string write_str,read_str;
  std::string fFilename;
 
 protected:
  void WriteSynchronized(string &command);
  void WriteNotSynchronized(string &command);
  void ReadSynchronized(string &command, string &readstring);
  void Timeout();
  void ParseSettingsXML(const string& pFilename, ostream& os);        
 };
};

#endif 
