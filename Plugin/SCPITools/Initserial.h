  /*
  * File:   Hameg4040.h
  * Author: Johannes Grossmann
  * Distributed under the Boost Software License, Version 1.0.
  * Created on February 19th, 2015, 10:46 AM
  */

  #ifndef INITSERIAL_H
  #define	INITSERIAL_H

  #include <chrono>
  #include <cstdlib>
  #include <iostream>
  #include <memory>
  #include <string>
  #include <thread>
  #include <vector>
  #include "../SCPIUtils/pugixml.hpp"
  #include "../SCPIUtils/ConsoleColor.h"
  #include "AsyncSerial.h"
  #include "BufferedAsyncSerial.h"

  using namespace std;
  // using namespace BufferedAsyncSerial;

  namespace INITSERIAL{
  typedef std::map<std::string, std::string>    SerialSettingsMap;
  
  class Serial
  {
    
  public:
    Serial(){	
    ParseSettingsXML(fFilename, std::cout);
    serial=make_shared<BufferedAsyncSerial>();
    this->Initialise(gSerialSettingsmap);
    this->EmptyBuffer();
    }
    ~Serial(){
    }
    // methods
    // 		void Initialise(AsyncSerial &serial, SerialSettingsMap &fSerialSettingMap);  // wants to be called after SystemController::ReadHW, ReadSettings
    std::string endline;
  protected:
    void EmptyBuffer();  // wants to be called after SystemController::ReadHW, ReadSettings
    void Initialise(SerialSettingsMap &fSerialSettingMap);  // wants to be called after SystemController::ReadHW, ReadSettings
    void ParseSettingsXML(const string& pFilename, ostream& os);
    string fFilename;
  private:
    shared_ptr<BufferedAsyncSerial>serial;
    SerialSettingsMap gSerialSettingsmap;
  };
  }

  #endif 