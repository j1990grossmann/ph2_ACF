#include "Keithley2700.h"

#include <iostream>

using namespace KEITHLEY2700;

void Keithley2700::Initialise()
{	
	serial = new BufferedAsyncSerial();
	
	string devname("/dev/Moxa");
	unsigned int baudrate = 19200;
	boost::asio::serial_port_base::parity            parity(boost::asio::serial_port_base::parity::none);
	boost::asio::serial_port_base::character_size    character_size(8);
	boost::asio::serial_port_base::flow_control      flow_control(boost::asio::serial_port_base::flow_control::none);
	boost::asio::serial_port_base::stop_bits         stop_bits(boost::asio::serial_port_base::stop_bits::one);
	endline = "\n";
	try{
		serial->open(devname,baudrate, parity, character_size, flow_control, stop_bits);
	}catch(boost::system::system_error& e){
		std::cout<<"boost error catched serial not open  "<<e.what()<<std::endl;
		exit(1);
	}
	
}

void Keithley2700::Configure()
{
	this->Reset();
	this->SystAzerStat(1);
	this->SystLSyn(1);
	this->SenseVolt();
	this->RangAuto(1);
	//for single read
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(1);
	std::cout<<"Initialized and Reset"<<std::endl;
}

void Keithley2700::ConfigureSingleRead()
{
	this->TracCle();
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(1);
}

void Keithley2700::ConfigureMultipleRead(int no_samples)
{
	this->TracCle();
	this->InitCont(0);
	this->SampCoun(no_samples);
}

void Keithley2700::Reset()
{
	write_str = "*RST";
	this->WriteNotSynchronized(write_str);
}

void Keithley2700::SenseCurr()
{
	write_str = ":FUNC 'CURR'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseCurrAC()
{
	write_str = ":FUNC 'CURR:AC'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseFreq()
{
	write_str = ":FUNC 'FREQ'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseFres()
{
	write_str = ":FUNC 'FRES'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SensePer()
{
	write_str = ":FUNC 'PER'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseRes()
{
	write_str = ":FUNC 'RES'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseTemp()
{
	write_str = ":FUNC 'TEMP'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseVolt()
{
	write_str = ":FUNC 'VOLT'";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SenseVoltAC()
{
	write_str = ":FUNC 'VOLT:AC'";
	this->WriteSynchronized(write_str);
}


void Keithley2700::RangAuto(int onoff)
{
	write_str = ":RANG:AUTO ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}
// void Keithley2700::VoltAcRang(int rang)
// {
// 	write_str = "VOLT:AC:RANG ";
// 	if(onoff==0)
// 	{
// 		write_str+="OFF";
// 	}else
// 		write_str+="ON";
// 	this->WriteSynchronized(write_str);
// }

void Keithley2700::TracCle()
{
	write_str = "TRAC:CLE";
	this->WriteSynchronized(write_str);
}

void Keithley2700::Read(string &read_str)
{
	write_str = "Read?";
	this->ReadSynchronized(write_str,read_str);
}

void Keithley2700::InitCont(int onoff)
{
		write_str = ":INIT:CONT ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}

void Keithley2700::SampCoun(int no_samples)
{
	write_str = "SAMP:COUN ";
	write_str+=std::to_string(no_samples);
	this->WriteSynchronized(write_str);
}

void Keithley2700::MeasAcVolt(double& volt)
{
	this->TracCle();
	this->Reset();
	this->SenseVolt();
}


void Keithley2700::TrigCoun(int no_triggers)
{
	write_str = "TRIG:COUN ";
	write_str+=std::to_string(no_triggers);
	this->WriteSynchronized(write_str);
}


void Keithley2700::Init()
{
	write_str = "INIT";
	this->WriteSynchronized(write_str);
}

void Keithley2700::TrigRetSingle(double& value)
{
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(1);
	this->Read(read_str);
	std::cout<<read_str<<std::endl;
	value=0;
}

void Keithley2700::TrigRetMultiple(int no_samples, vector< double >& values)
{
	this->TracCle();
	this->InitCont(0);
	this->TrigCoun(1);
	this->SampCoun(no_samples);
	this->Read(read_str);
	std::cout<<read_str<<std::endl;
}

void Keithley2700::Fetch(string &read_str)
{
	write_str = "FETC?";
	this->ReadSynchronized(write_str,read_str);
}

void Keithley2700::SystAzerStat(int onoff)
{
	write_str = "SYST:AZER:STAT ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteNotSynchronized(write_str);
}

void Keithley2700::SystLSyn(int onoff)
{
	write_str = "SYST:LSYN ";
	if(onoff==0)
	{
		write_str+="OFF";
	}else
		write_str+="ON";
	this->WriteSynchronized(write_str);
}


void Keithley2700::ReadSynchronized(string& command, string& read_str)
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

void Keithley2700::WriteSynchronized(string& command)
{
	int counter = 0;
	this->serial->writeString("*CLS"+endline);
	this->serial->writeString(command+endline);
	this->serial->writeString("*OPC?"+endline);
	std::cout<<"written "<<command<<std::endl;
//  	auto start = std::chrono::system_clock::now();
	read_str ="";
	while(read_str.empty())
	{		
//  		if(counter == 10000){break;}
 		this->Timeout();
 		read_str=serial->readStringUntil(endline);
		if(!read_str.empty()){
// 			std:cout<<read_str<<"\ttest\t"<<counter<<std::endl;
		}
		counter++;
	}
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std::cout<<"time for completion of command\t"<<elapsed.count()<<std::endl;
}

void Keithley2700::WriteNotSynchronized(string& command)
{
	int counter = 0;
	this->serial->writeString(command+endline);
	std::cout<<"written "<<command<<std::endl;
//  	auto start = std::chrono::system_clock::now();
	for(int i=0; i<10000; i++){
		this->Timeout();	
	}
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std::cout<<"time for completion of command\t"<<elapsed.count()<<std::endl;
}


void Keithley2700::Timeout()
{
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

