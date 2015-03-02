#include <cstring>
#include <vector>
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../tools/Calibration.h"
#include "../tools/MuxTest.h"
#include "../SMUTools/Hameg4040.h"
#include "../SMUTools/Keithley2700.h"

#include <TApplication.h>
#include "../Utils/argvparser.h"
#include "TROOT.h"



using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;

using namespace CommandLineProcessing;


int main( int argc, char* argv[] )
{
	ArgvParser cmd;
	
	// init
	cmd.setIntroductoryDescription( "CMS Ph2_ACF Test procedure using Analog Mux" );
	// error codes
	cmd.addErrorCode( 0, "Success" );
	cmd.addErrorCode( 1, "Error" );
	// options
	cmd.setHelpOption( "h", "help", "Print this help page" );
	
	cmd.defineOption( "file", "Hw Description File . Default value: settings/Calibration2CBC.xml", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "file", "f" );
	
	cmd.defineOption( "output", "Output Directory . Default value: Results", ArgvParser::OptionRequiresValue /*| ArgvParser::OptionRequired*/ );
	cmd.defineOptionAlternative( "output", "o" );
	
	cmd.defineOption( "skip", "skip scaning VCth vs Vplus", ArgvParser::NoOptionAttribute );
	
	cmd.defineOption( "old", "Use old calibration algorithm", ArgvParser::NoOptionAttribute );
	
	cmd.defineOption( "bitmode", "Turn on bitwise offset tuning. Default: false", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "bitmode" , "bm" );
	
	cmd.defineOption( "allChan", "Do calibration using all channels? Default: false", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "allChan", "a" );
	
	cmd.defineOption( "batch", "Run the application in batch mode", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "batch", "b" );
	
	cmd.defineOption( "AmuxTest", "Set amux register to dec 2, VCth ", ArgvParser::NoOptionAttribute );
	cmd.defineOptionAlternative( "AmuxTest", "am" );
	
	int result = cmd.parse( argc, argv );
	if ( result != ArgvParser::NoParserError )
	{
		std::cout << cmd.parseErrorDescription( result );
		exit( 1 );
	}
	
	// now query the parsing results
	std::string cHWFile = ( cmd.foundOption( "file" ) ) ? cmd.optionValue( "file" ) : "settings/Calibration2CBC.xml";
	std::string cDirectory = ( cmd.foundOption( "output" ) ) ? cmd.optionValue( "output" ) : "Results/";
	cDirectory += "AMuxTest";
	bool cVplus = ( cmd.foundOption( "skip" ) ) ? true : false;
	bool cOld = ( cmd.foundOption( "old" ) ) ? true : false;
	
	bool cOffsetTuneMode = ( cmd.foundOption( "bitmode" ) ) ? true : false;
	bool cCalibrateTGrp = ( cmd.foundOption( "allChan" ) ) ? true : false;
	bool batchMode = ( cmd.foundOption( "batch" ) ) ? true : false;
	bool cAmuxTest = ( cmd.foundOption( "AmuxTest" ) ) ? true : false;
	
	
	TApplication cApp( "Root Application", &argc, argv );
	if ( batchMode ) gROOT->SetBatch( true );
	else TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );
	
	
	if( cAmuxTest)
	{
		//File for data:
		
		//		ofstream MeasFile;
		//	MeasFile.open("ResultsScan/Measurement.txt");
		
		

		
		MuxTest cAnalogmuxTest( cOffsetTuneMode, cCalibrateTGrp, cHWFile );
		cAnalogmuxTest.SMUInitialiseAndConfigure();
		cAnalogmuxTest.InitializeHw( cHWFile );
		cAnalogmuxTest.InitializeSettings( cHWFile );
		cAnalogmuxTest.CreateResultDirectory( cDirectory );
		cAnalogmuxTest.InitResultFile( "AmuxResults" );
		cAnalogmuxTest.ConfigureHw();
		cAnalogmuxTest.Initialise(); // canvases etc. for fast calibration
		cAnalogmuxTest.ScanVplusAMux();		
 		cAnalogmuxTest.SMUKill();
		
	}
	
	
	if ( !batchMode ) cApp.Run();
	std::cout<<RESET<<"finish amux scans"<<std::endl;
	return 0;
}
