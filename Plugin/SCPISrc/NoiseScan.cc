#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// #include "TApplication.h"
// #include "TROOT.h"

#include "../SCPIUtils/argvparser.h"
#include "../SCPIUtils/ConsoleColor.h"
#include "../SCPITools/Scan.h"

using namespace std;
using namespace boost;
using namespace CommandLineProcessing;

// using namespace HAMEG4040;

void signalHandler( int signum )
{
  cout << "Interrupt signal (" << signum << ") received.\n";
  
  // cleanup and close up stuff here  
  // terminate program  

  exit(signum);  

}

int main(int argc, char* argv[])
{
  signal(SIGINT, signalHandler);
	
	ArgvParser cmd;
	
	// init
	cmd.setIntroductoryDescription( "Take data at different voltages and sweep the Vcth threshold" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );
	
	cmd.defineOption( "file", "Config File . Default value: Plugin/SCPISettings/NoiseScan.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );
	
	cmd.defineOption( "output", "Output Directory . Default value: NoiseScanData", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "output", "o" );
	
	cmd.defineOption( "cbc0_configfile", "CBC0 ConfigFile Default Value: settings/Cbc_default_hole.txt", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "cbc0_configfile", "cbc0" );

	cmd.defineOption( "cbc1_configfile", "CBC1 ConfigFile Default Value: settings/Cbc_default_hole.txt", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "cbc1_configfile", "cbc1" );

	cmd.defineOption( "iv_curve", "If set the program will just draw an IV curve", ArgvParser::NoOptionAttribute /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "iv_curve", "iv" );

	cmd.defineOption( "angle", "Beam incident angle in degrees",  ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "angle", "a" );
		
	cmd.defineOption( "pos_x", "Perpendicular horizontal distance to the beam axis",  ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );

	cmd.defineOption( "pos_z", "Perpendicular vertical distance to the beam axis",  ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );

	cmd.defineOption( "batch", "Run the application in batch mode", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "batch", "b" );
	
	
	int result_parser = cmd.parse( argc, argv );
	if ( result_parser != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result_parser );
		exit( 1 );
	}
	
	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "Plugin/SCPISettings/NoiseScan.xml";
	std::string cDirectory = ( cmd.foundOption( "output" ) ) ? cmd.optionValue( "output" ) : "NoiseScanData/";
	std::string cCBC0Config= ( cmd.foundOption( "cbc0_configfile" ) ) ? cmd.optionValue( "cbc0_configfile" ) : "settings/Cbc_default_hole.txt";
	std::string cCBC1Config= ( cmd.foundOption( "cbc1_configfile" ) ) ? cmd.optionValue( "cbc1_configfile" ) : "settings/Cbc_default_hole.txt";

	std::string cAngle= ( cmd.foundOption( "angle" ) ) ? cmd.optionValue( "angle" ) : "0";
	std::string cPosX= ( cmd.foundOption( "pos_x" ) ) ? cmd.optionValue( "pos_x" ) : "0";
	std::string cPosZ= ( cmd.foundOption( "pos_z" ) ) ? cmd.optionValue( "pos_z" ) : "0";

// 	HAMEG4040::HamegChannelMap cHamegChannelMap;
// 	cHamegChannelMap.resize(4, vector<double>( 2 , 0. ) );
	
	bool batchMode = ( cmd.foundOption( "batch" ) ) ? true : false;
	bool cIVCurve= ( cmd.foundOption( "iv_curve" ) ) ? true : false;
	
	
//  	TApplication cApp( "Root Application", &argc, argv );
//  	if ( batchMode ) gROOT->SetBatch( true );
//  	else TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );
	
	printf("Read File %s\n",cHWFile.c_str());
	

//  	if ( !batchMode ) cApp.Run();
	
//  	HAMEG4040::Hameg4040 h(cHWFile.c_str());
	// 	h.Configure();
	SCAN::Scan scan(cHWFile, cDirectory);
 	scan.SetConfigfile(cCBC0Config,cCBC1Config);
 	scan.StartScan(cIVCurve,cAngle, cPosX, cPosZ);

	return 0;
}
