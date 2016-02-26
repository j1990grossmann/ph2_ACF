#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// #include "TApplication.h"
// #include "TROOT.h"

#include "../SCPITools/Hameg4040.h"
#include "../SCPIUtils/argvparser.h"
#include "../SCPIUtils/ConsoleColor.h"
#include "../SCPIUtils/Timer.h"



using namespace std;
using namespace boost;
using namespace CommandLineProcessing;
using namespace HAMEG4040;

volatile sig_atomic_t stop(0);


void signalHandler( int signum )
{
  cout << "Interrupt signal (" << signum << ") received.\n";
  // terminate program  

  stop=1;

}

int main(int argc, char* argv[])
{
	signal(SIGINT, signalHandler);  

	ArgvParser cmd;
	
	// init
	cmd.setIntroductoryDescription( "Hameg 4040 test program" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );
	
	cmd.defineOption( "file", "Hw Description File . Default value: SMUSettings/Hameg.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );
	
	cmd.defineOption( "output", "Output Directory . Default value: NoiseScanData", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "output", "o" );

	cmd.defineOption( "on_off", "on/off Default value: on", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	
	cmd.defineOption( "batch", "Run the application in batch mode", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "batch", "b" );
	
	
	int result_parser = cmd.parse( argc, argv );
	if ( result_parser != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result_parser );
		exit( 1 );
	}
	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "SCPISettings/Hameg.xml";
	std::string cDirectory = ( cmd.foundOption( "output" ) ) ? cmd.optionValue( "output" ) : "NoiseScanData/Hameg";
 	std::string on_off_str = ( cmd.foundOption( "on_off" ) ) ? cmd.optionValue( "on_off" ) : "on";
	
	HAMEG4040::HamegChannelMap cHamegChannelMap;
	cHamegChannelMap.resize(4, vector<double>( 2 , 0. ) );
	
	bool batchMode = ( cmd.foundOption( "batch" ) ) ? true : false;
	
	bool on_off=(on_off_str=="on")? true :false;
//  	TApplication cApp( "Root Application", &argc, argv );
//  	if ( batchMode ) gROOT->SetBatch( true );
//  	else TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );
		printf("Read File %s\n",cHWFile.c_str());
//  	if ( !batchMode ) cApp.Run();
	Timer timer;
 	HAMEG4040::Hameg4040 h(cHWFile.c_str());
	timer.start();
	h.Configure();
	timer.stop();
	timer.show("Configured");
	if(on_off)
	{
		h.OutPutGen(1);
		std::cout<<"V1\tI1\tV2\tI2\tV3\tI3\tV4\tI4"<<std::endl;
		timer.start();
// 		for(int i=0;i<100;i++)
		h.MeasAllA();
		timer.stop();
		timer.show("Version A");
		timer.start();
// 		for(int i=0;i<100;i++)
			h.MeasAllB();
		timer.stop();
		timer.show("Version B");
		timer.start();
// 		for(int i=0;i<100;i++)
		h.MeasAllC();
		timer.stop();
		timer.show("Version C");
// 		while(!stop)
// 		{
// 			sleep(2);
// 			timer.reset();timer.start();
// 			h.MeasAll(cHamegChannelMap);
// 			for(int i=0;i<4;i++)
// 			{
// 				std::cout<<cHamegChannelMap[i].at(0)<<"\t"<<cHamegChannelMap[i].at(1)<<"\t";
// 			}
// 			std::cout<<std::endl;
// 		}
	}
	else
		h.OutPutGen(0);
	return 0;
}
