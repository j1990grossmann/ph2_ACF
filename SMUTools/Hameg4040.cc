#include "Hameg4040.h"

#include <iostream>

using namespace HAMEG4040;

void Hameg4040::Initialise()
{	
	serial = new BufferedAsyncSerial();
	
	string devname("/dev/Hameg4040");
	unsigned int baudrate = 115200;
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
	
	
	
// 	set the Initial voltages and currents to 0
	for(int i=0; i<4; i++){
		fHamegChannelMap[i].resize(2);
		fHamegChannelMap[i].at(0)=0;
		fHamegChannelMap[i].at(1)=0;
		
		fHamegChannelMapCurr[i].resize(2);  
		fHamegChannelMapCurr[i].at(0)=0;
		fHamegChannelMapCurr[i].at(1)=0;
	}
}

void Hameg4040::Configure(HamegChannelMap &fSetHamegChannelMap)
{
	this->Reset();
	write_str = "*IDN?";
	this->ReadSynchronized(write_str,read_str);
	this->SystBeeperImmediate();
	this->SystRemote();
// 	SystMix();
	std::cout<<"Initialized and Reset and Locked"<<read_str<<std::endl;
	fHamegChannelMap=fSetHamegChannelMap;
	for(int i=0;i<4;i++)
	{
		this->WriteChannelNo(i, fHamegChannelMap[i].at(0), fHamegChannelMap[i].at(1));
		this->OutPutSel(i,1);
	}
	cout<<"Configured ? Ready to go? y/n"<<endl;
	read_str="";
	while(cin>>read_str)
	{
		if(read_str=="y")
			break;
		else if(read_str=="n")
			exit(1);
		else
			cout<<"this is not y/n"<<endl;
	}
	this->OutPutGen(1);
}

void Hameg4040::Reset()
{
	write_str = "*RST";
	this->WriteSynchronized(write_str);
}

void Hameg4040::WriteChannelNo(int channelno, double voltage, double current)
{
	string voltagestring = std::to_string(voltage);
	string currentstring = std::to_string(current);
	string outstring = "APPLY "+voltagestring+","+currentstring;
 	this->InstSelect(channelno);
	this->WriteSynchronized(outstring);
}

void Hameg4040::SourVolt(int channelno, double voltage)
{
	write_str="VOLT "+std::to_string(channelno+1);
	this->WriteSynchronized(write_str);
}

void Hameg4040::SourCurr(int channelno, double current)
{
	write_str="CURR "+std::to_string(channelno+1);
	this->WriteSynchronized(write_str);
}

void Hameg4040::InstSelect(int channelno)
{	
	write_str="INST:NSEL "+std::to_string(channelno+1);
	this->WriteSynchronized(write_str);
}

int Hameg4040::InstSelectQuery()
{
	write_str="INST:NSEL?";
	this->ReadSynchronized(write_str,read_str);
	return atoi(read_str.c_str());
}

void Hameg4040::OutPutSel(int channelno, int outp_state)
{
	this->InstSelect(channelno);
	write_str="OUTP:SEL "+std::to_string(outp_state);
	this->WriteSynchronized(write_str);
}

int Hameg4040::OutPutSelQuery()
{
	write_str="OUTP?";
	this->ReadSynchronized(write_str,read_str);
	return atoi(read_str.c_str());
}

void Hameg4040::OutPutGen(int outp_state)
{
	write_str="OUTP:GEN "+std::to_string(outp_state);
	this->WriteSynchronized(write_str);
}

void Hameg4040::SystBeeperImmediate()
{
	write_str= "SYST:BEEP:IMM";
	this->WriteSynchronized(write_str);
}

void Hameg4040::SystError()
{
	write_str = "SYST:ERR?";
	this->ReadSynchronized(write_str,read_str);
}

void Hameg4040::SystLocal()
{
	write_str = "SYST:LOC";
	this->WriteNotSynchronized(write_str);
}

void Hameg4040::SystMix()
{
	write_str = "SYST:MIX";
	this->WriteSynchronized(write_str);
}

void Hameg4040::SystRemote()
{
	write_str = "SYST:REM";
	this->WriteSynchronized(write_str);
}

void Hameg4040::SystRWLock()
{
	write_str = "SYST:RWL";
	this->WriteSynchronized(write_str);
}

void Hameg4040::SystVersion()
{
	write_str= 	"SYST:VERS?";
	this->ReadSynchronized(write_str,read_str);
}


void Hameg4040::MeasCurr(double &amp)
{
	write_str="MEAS:CURR?";
	this->ReadSynchronized(write_str, read_str);
	amp=atof(read_str.c_str());
}

void Hameg4040::MeasVolt(double &volt)
{
	write_str="MEAS:VOLT?";
	this->ReadSynchronized(write_str, read_str);
	volt=atof(read_str.c_str());
}

void Hameg4040::MeasAll(HamegChannelMap& fGetHamegChannelMap)
{
	for(int i=0; i<4; i++)
	{
		this->InstSelect(i);
		this->MeasVolt(fHamegChannelMapCurr[i].at(0));
		this->MeasCurr(fHamegChannelMapCurr[i].at(1));
	}
	fGetHamegChannelMap=fHamegChannelMapCurr;
}

void Hameg4040::GetHamegChannelMap(HamegChannelMap& fGetHamegChannelMap)
{
	fGetHamegChannelMap=fHamegChannelMapCurr;
}



void Hameg4040::ReadSynchronized(string& command, string& read_str)
{
	int counter = 0;
	this->serial->writeString(command+endline);
// 	auto start = std::chrono::system_clock::now();
	read_str="";
	while(read_str.empty())
	{		
 		if(counter == 10000){break;}
 		Timeout();
 		read_str=serial->readStringUntil(endline);
// 		std:cout<<readstring<<"\ttest\t"<<counter<<std::endl;
		counter++;
	}
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std:cout<<readstring<<"\ttest\t"<<elapsed.count()<<std::endl;
}

void Hameg4040::WriteSynchronized(string& command)
{
	int counter = 0;
	this->serial->writeString("*CLS"+endline);
	this->serial->writeString(command+endline);
	this->serial->writeString("*OPC?"+endline);
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

void Hameg4040::WriteNotSynchronized(string& command)
{
	int counter = 0;
	this->serial->writeString(command+endline);
//  	auto start = std::chrono::system_clock::now();
	for(int i=0; i<100; i++){
		this->Timeout();	
	}
// 	auto end = std::chrono::system_clock::now();
// 	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
// 	std::cout<<"time for completion of command\t"<<elapsed.count()<<std::endl;
}


void Hameg4040::Timeout()
{
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

