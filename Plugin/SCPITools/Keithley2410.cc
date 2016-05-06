#include "Keithley2410.h"
#include "Initserial.h"

#include <iostream>

using namespace KEITHLEY2410;

void Keithley2410::Configure()
{
	this->Reset();
// 	this->SystAzerStat(1);
// 	this->SystLSyn(1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->SourFuncVolt();
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->SourVoltModeFixed();
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->SourVoltRang("1000");
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->SourVoltLev("0");
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->SenseCurrProt("5E-6");
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	this->SenseFuncCurr();
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
// 	this->SenseCurrRang("10E-6");
	this->RangAuto(1);
// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout<<"Initialized and Reset"<<std::endl;
}

void Keithley2410::ConfigureSingleRead()
{
	this->TracCle();
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(1);
}

void Keithley2410::ConfigureMultipleRead(int no_samples)
{
	this->TracCle();
	this->InitCont(0);
	this->SampCoun(no_samples);
}

void Keithley2410::Reset()
{
	write_str = "*RST";
	this->WriteNotSynchronized(write_str);
}

void Keithley2410::SenseCurr()
{
	write_str = ":FUNC 'CURR'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseCurrAC()
{
	write_str = ":FUNC 'CURR:AC'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseFreq()
{
	write_str = ":FUNC 'FREQ'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseFres()
{
	write_str = ":FUNC 'FRES'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SensePer()
{
	write_str = ":FUNC 'PER'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseRes()
{
	write_str = ":FUNC 'RES'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseTemp()
{
	write_str = ":FUNC 'TEMP'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseVolt()
{
	write_str = ":FUNC VOLT";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseVoltAC()
{
	write_str = ":FUNC VOLT:AC";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourFuncVolt()
{
	write_str = ":SOUR:FUNC VOLT";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourFuncCurr()
{
	write_str = ":SOUR:FUNC CURR";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourCurrModeFixed()
{
	write_str = ":SOUR:CURR:MODE FIX";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourVoltModeFixed()
{
	write_str = ":SOUR:VOLT:MODE FIX";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourCurrRang(string rang)
{
	write_str = ":SOUR:CURR:RANG ";
	write_str+=rang;
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourVoltRang(string rang)
{
	write_str = ":SOUR:VOLT:RANG ";
	write_str+=rang;
	this->WriteSynchronized(write_str);
}


void Keithley2410::SourCurrLev(string lev)
{
	write_str = ":SOUR:CURR:LEV ";
	write_str+=lev;
	this->WriteSynchronized(write_str);
}

void Keithley2410::SourVoltLev(string lev)
{
	write_str = ":SOUR:VOLT:LEV ";
	write_str+=lev;
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseFuncCurr()
{
	write_str = ":SENS:FUNC 'CURR'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseFuncVolt()
{
	write_str = ":SENS:FUNC 'VOLT'";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseVoltProt(string prot)
{
	write_str = ":SENS:VOLT:PROT ";
	write_str+=prot;
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseCurrProt(string prot)
{
	write_str = ":SENS:CURR:PROT ";
	write_str+=prot;
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseVoltRang(string rang)
{
	write_str = ":SENS:VOLT:RANG ";
	write_str+=rang;
	this->WriteSynchronized(write_str);
}

void Keithley2410::SenseCurrRang(string rang)
{
	write_str = ":SENS:CURR:RANG ";
	write_str+=rang;
	this->WriteSynchronized(write_str);
}

void Keithley2410::Outp(int i)
{
	write_str = ":OUTP ";
		if(i==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}

void Keithley2410::RangAuto(int onoff)
{
	write_str = ":RANG:AUTO ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}


void Keithley2410::TracCle()
{
	write_str = "TRAC:CLE";
	this->WriteSynchronized(write_str);
}

void Keithley2410::Read(string &read_str)
{
	write_str = "Read?";
	this->ReadSynchronized(write_str,read_str);
}

void Keithley2410::InitCont(int onoff)
{
		write_str = ":INIT:CONT ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SampCoun(int no_samples)
{
	write_str = "SAMP:COUN ";
	write_str+=std::to_string(no_samples);
	this->WriteSynchronized(write_str);
}

void Keithley2410::MeasAcVolt(double& volt)
{
	this->TracCle();
	this->Reset();
	this->SenseVolt();
}


void Keithley2410::TrigCoun(int no_triggers)
{
	write_str = "TRIG:COUN ";
	write_str+=std::to_string(no_triggers);
	this->WriteSynchronized(write_str);
}


void Keithley2410::Init()
{
	write_str = "INIT";
	this->WriteSynchronized(write_str);
}

void Keithley2410::TrigRetSingle(double& value)
{
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(1);
	this->Read(read_str);
	std::cout<<read_str<<std::endl;
	value=0;
}

void Keithley2410::TrigRetMultiple(int no_samples, vector< double >& values)
{
	this->TracCle();
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(no_samples);
	this->Read(read_str);
	std::cout<<read_str<<std::endl;
}

void Keithley2410::Fetch(string &read_str)
{
	write_str = "FETC?";
	this->ReadSynchronized(write_str,read_str);
}

void Keithley2410::SystAzerStat(int onoff)
{
	write_str = "SYST:AZER:STAT ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}

void Keithley2410::SystLSyn(int onoff)
{
	write_str = "SYST:LSYN ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}


void Keithley2410::ReadSynchronized(string& command, string& read_str)
{
	int counter = 0;
	this->serial->writeString(command+endline);
// 	auto start = std::chrono::system_clock::now();
	read_str="";
	while(read_str.empty())
	{		
//  		if(counter == 10000){break;}
 		Timeout();
 		read_str=serial->readStringUntil(endline);
// 		std:cout<<readstring<<"\ttest\t"<<counter<<std::endl;
		counter++;
	}
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std:cout<<readstring<<"\ttest\t"<<elapsed.count()<<std::endl;
}

void Keithley2410::WriteSynchronized(string& command)
{
	int counter = 0;
	this->serial->writeString("*CLS"+endline);
	this->serial->writeString(command+endline);
	this->serial->writeString("*OPC?"+endline);
	std::cout<<"written "<<command<<std::endl;
  	auto start = std::chrono::system_clock::now();
	read_str ="";
	while(read_str.empty())
	{		
 		if(counter==100000)
		{
			std::cout<<"no OPC received"<<std::endl;
			break;
		}
 		this->Timeout();
 		read_str=serial->readStringUntil(endline);
		if(!read_str.empty()){
// 			std:cout<<read_str<<"\tMicroseconds until OPC received\t"<<counter<<std::endl;
		}
		if(counter%10000==0 && counter!=0)
		{
			std::cout<<counter<<endl;
		}
		counter++;
	}
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std::cout<<"time for completion of command\t"<<elapsed.count()<<std::endl;
}

void Keithley2410::WriteNotSynchronized(string& command)
{
	int counter = 0;
	this->serial->writeString(command+endline);
	std::cout<<"written "<<command<<std::endl;
//  	auto start = std::chrono::system_clock::now();
	std::this_thread::sleep_for(std::chrono::seconds(2));
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std::cout<<"time for completion of command\t"<<elapsed.count()<<std::endl;
}


void Keithley2410::Timeout()
{
	std::this_thread::sleep_for(std::chrono::microseconds(1));
}
void Keithley2410::ParseSettingsXML(const string& pFilename, ostream& os)
{
	endline='\n';
	INITSERIAL::SerialSettingsMap tmpSerialSettingsMap;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( pFilename.c_str() );
	if ( !result )
	{
		cout << "ERROR :\n Unable to open the file : " << pFilename<< std::endl;
		cout << "Error description : " << result.description() << std::endl;
		return ;
	}
	for ( pugi::xml_node nSettings = doc.child( "Settings" ); nSettings; nSettings = nSettings.next_sibling() )
	{
		pugi::xml_attribute attr;
		for ( pugi::xml_node nSetting = nSettings.child( "Setting" ); nSetting; nSetting = nSetting.next_sibling() )
		{
			if(attr=nSetting.attribute( "rs232" )){
				tmpSerialSettingsMap[nSetting.attribute( "rs232" ).value()]=nSetting.first_child().value();
				cout << RED << "Setting" << RESET << " --" << BOLDCYAN << nSetting.attribute( "rs232" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
			}
		}
	}
	fSerialSettingsmap=tmpSerialSettingsMap;
}
