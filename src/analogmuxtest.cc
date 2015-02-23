#include <cstring>
#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"
#include "../tools/Calibration.h"
#include "../tools/MuxTest.h"
#include "../tools/Hameg4040.h"
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
		ofstream MeasFile;
		MeasFile.open("./ResultsScan/Measurement.txt");
		
		Hameg4040 *h = new Hameg4040();
		
		ChannelMap fChannelMap;
		for(int i=0; i<4; i++)
		{
			fChannelMap[i].resize(2);
		}
		
		fChannelMap[0].at(0)=1.2;
		fChannelMap[1].at(0)=3.3;
		fChannelMap[2].at(0)=5;
		fChannelMap[3].at(0)=5;
		fChannelMap[0].at(1)=0.2;
		fChannelMap[1].at(1)=0.2;
		fChannelMap[2].at(1)=0.02;
		fChannelMap[3].at(1)=0.02;
		
		
		h->Initialise();
		h->Configure(fChannelMap);
		
		double volt, amp;
		
		
		std::cout<<"V1\tI1\tV2\tI2\tV3\tI3\tV4\tI4"<<std::endl;
		// 	auto start = std::chrono::system_clock::now();
// 		for(int j=0; j<100; j++)
// 		{
// 			h->MeasAll(fChannelMap);
// 			for(int i=0;i<4;i++)
// 			{
// 				std::cout<<fChannelMap[i].at(0)<<"\t"<<fChannelMap[i].at(1)<<"\t";
// 			}
// 			std::cout<<std::endl;
// 		}
		MuxTest cAmuxTest( cOffsetTuneMode, cCalibrateTGrp);
		cAmuxTest.InitializeHw( cHWFile );
		cAmuxTest.InitializeSettings( cHWFile );
		cAmuxTest.CreateResultDirectory( cDirectory );
		cAmuxTest.InitResultFile( "AmuxTestResults" );
		cAmuxTest.ConfigureHw();
		cAmuxTest.Initialise(); // canvases etc. for fast calibration
		cAmuxTest.ScanVplusAMux();
		cAmuxTest.SaveResults();
		
			
		delete h;
	}

	
	if ( !batchMode ) cApp.Run();
	std::cout<<"finish amux scans"<<std::endl;
	return 0;
}
