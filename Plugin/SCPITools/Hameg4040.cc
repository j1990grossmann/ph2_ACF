#include "Hameg4040.h"

#include <iostream>
#include <regex>

using namespace HAMEG4040;

void Hameg4040::Configure()
{
	this->EmptyBuffer();
	this->IDNCheck();
	this->Reset();
	this->SystBeeperImmediate();
	this->SystRemote();
	// 	SystMix();
	std::cout<<"Initialized and Reset and Locked"<<std::endl;
	std::cout<<"Channel Voltage Current"<<std::endl;

	for(int i=0;i<4;i++)
	{
	  // printf("%d, %e, %e\n",i, fHamegChannelMap[i].at(0),fHamegChannelMap[i].at(1));
	  this->WriteChannelNo(i, fHamegChannelMap[i].at(0), fHamegChannelMap[i].at(1));
	  this->OutPutSel(i,1);
	}
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
bool Hameg4040::IDNCheck()
{ 
	write_str = "*IDN?";
	this->ReadSynchronized(write_str,read_str);
	printf("*IDN?\t%s\n",read_str.c_str());
	std::regex e ("(HAMEG,HMP4040,)(.*)");
	if (std::regex_match (read_str,e))
	{
		std::cout << "Identified Hameg HMP4040\n";
		return true;
	}
	else
	{
		std::cout<<"Instrument not identified aborting"<<std::endl;
		exit(1);	
	}
}
void Hameg4040::GetHamegChannelMap(HamegChannelMap& fGetHamegChannelMap)
{
	fGetHamegChannelMap=fHamegChannelMapCurr;
}
void Hameg4040::EmptyBuffer()
{
	read_str="a";int counter=0;
	while(!read_str.empty())
	{		
		if(counter == 10000){break;}
		read_str=serial.readStringUntil(endline);
// 		std:cout<<"counter"<<counter<<"\t"<<read_str<<std::endl;
		counter++;
	}
}


void Hameg4040::ReadSynchronized(string& command, string& read_str)
{
	int counter = 0;
	this->serial.writeString(command+endline);
	auto start = std::chrono::system_clock::now();
	read_str="";
	while(read_str.empty())
	{		
// 		if(counter == 10000){break;}
		Timeout();
		read_str=serial.readStringUntil(endline);
		// 		std:cout<<readstring<<"\ttest\t"<<counter<<std::endl;
		counter++;
	}
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std:cout<<read_str<<"\ttest\t"<<counter<<"\telapsed\t"<<elapsed.count()<<std::endl;
}

void Hameg4040::WriteSynchronized(string& command)
{
	int counter = 0;
	this->serial.writeString("*CLS"+endline);
	this->serial.writeString(command+endline);
	this->serial.writeString("*OPC?"+endline);
	//  	auto start = std::chrono::system_clock::now();
	read_str ="";
	while(read_str.empty())
	{		
		//  		if(counter == 10000){break;}
		this->Timeout();
		read_str=serial.readStringUntil(endline);
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
	this->serial.writeString(command+endline);
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
	std::this_thread::sleep_for(std::chrono::nanoseconds(10));
}

void Hameg4040::ParseSettingsXML(const string& pFilename, ostream& os)
{
  endline='\n';
	INITSERIAL::SerialSettingsMap tmpHamegSettingsMap;
	HAMEG4040::HamegChannelMap  tmpHamegChannelMap;tmpHamegChannelMap.resize(4, vector<double>( 2 , 0. ) );
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
				tmpHamegSettingsMap[nSetting.attribute( "rs232" ).value()]=nSetting.first_child().value();
				cout << RED << "Setting" << RESET << " --" << BOLDCYAN << nSetting.attribute( "rs232" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
			}
		}
		for ( pugi::xml_node nSetting = nSettings.child( "Voltages" ).child("Voltage"); nSetting; nSetting = nSetting.next_sibling() )
		{				
			if(attr=nSetting.attribute( "Id" )){
				cout << RED << "Voltage    Channel" << RESET << " --" << BOLDCYAN << nSetting.attribute( "Id" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				tmpHamegChannelMap.at(nSetting.attribute( "Id" ).as_int(0))[0]=nSetting.first_child().text().as_double();
			}
		}
		for ( pugi::xml_node nSetting = nSettings.child( "CurrentsMax" ).child("CurrentMax"); nSetting; nSetting = nSetting.next_sibling() )
		{				
			if(attr=nSetting.attribute( "Id" )){
				cout << RED << "CurrentMax Channel" << RESET << " --" << BOLDCYAN << nSetting.attribute( "Id" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				tmpHamegChannelMap.at(nSetting.attribute( "Id" ).as_int())[1]=nSetting.first_child().text().as_double();
			}
		}
	}
	fHamegSettingsMap=tmpHamegSettingsMap;
	fHamegChannelMap=tmpHamegChannelMap;
}
