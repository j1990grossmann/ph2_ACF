#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// #include "TApplication.h"
// #include "TROOT.h"

#include "../SCPIUtils/argvparser.h"
#include "../SCPIUtils/ConsoleColor.h"
#include "../SCPITools/Keithley2410.h"
#include "../SCPITools/RunCounter.h"
#include "../SCPITools/HV_control.h"

using namespace std;
using namespace boost;
using namespace CommandLineProcessing;

// using namespace HAMEG4040;
volatile sig_atomic_t stop(0);


void signalHandler( int signum )
{
  cout << "Interrupt signal (" << signum << ") received.\n";
  
  // cleanup and close up stuff here  
  // terminate program  
  stop=1;
}

int main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
	
	ArgvParser cmd;
	
	// init
	cmd.setIntroductoryDescription( "Switch HV on and of according to config file SMUSetting/HV_Control.xml and save timestamps and currents to file" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );
	
	cmd.defineOption( "file", "Config File . Default value: SMUSettings/HV_control.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );
	
	cmd.defineOption( "output", "Output Directory . Default value: NoiseScanData", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "output", "o" );
	
	cmd.defineOption( "rampup", "rampup the HV", ArgvParser::NoOptionAttribute /*| ArgvParser::OptionRequired*/ );
	cmd.defineOption( "rampdown", "rampdown the HV", ArgvParser::NoOptionAttribute /*| ArgvParser::OptionRequired*/ );

	cmd.defineOption( "batch", "Run the application in batch mode", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "batch", "b" );
	
	
	int result_parser = cmd.parse( argc, argv );
	if ( result_parser != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result_parser );
		exit( 1 );
	}
	
	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "SMUSettings/HV_Control.xml";
	std::string cDirectory = ( cmd.foundOption( "output" ) ) ? cmd.optionValue( "output" ) : "NoiseScanData/";

	bool batchMode = ( cmd.foundOption( "batch" ) ) ? true : false;
	
	bool cUp;
	if(cmd.foundOption( "rampup" ))
		cUp=true;
	if(cmd.foundOption( "rampdown" ))
		cUp=false;
	if((cmd.foundOption( "rampup" ) && cmd.foundOption( "rampdown" )) || (!cmd.foundOption( "rampup" ) && !cmd.foundOption( "rampdown" )))
	{
		std::cout << "Specify rampup or rampdown"<<std::endl;
		exit( 1 );
	}

//  	TApplication cApp( "Root Application", &argc, argv );
//  	if ( batchMode ) gROOT->SetBatch( true );
//  	else TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );
	
	printf("Read File %s\n",cHWFile.c_str());
//  	if ( !batchMode ) cApp.Run();

	HV_CONTROL::Controller(cHWFile, cDirectory, cUp, stop);
	

	return 0;
}
