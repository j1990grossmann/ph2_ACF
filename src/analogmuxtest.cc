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
		
		
 		HAMEG4040::Hameg4040 *h = new HAMEG4040::Hameg4040();
 		
 		HAMEG4040::HamegChannelMap fHamegChannelMap;
 		for(int i=0; i<4; i++)
 		{
 			fHamegChannelMap[i].resize(2);
 		}
 		
		fHamegChannelMap[0].at(0)=1.2;
		fHamegChannelMap[1].at(0)=3.3;
		fHamegChannelMap[2].at(0)=5;
		fHamegChannelMap[3].at(0)=5;
		fHamegChannelMap[0].at(1)=0.2;
		fHamegChannelMap[1].at(1)=0.2;
		fHamegChannelMap[2].at(1)=0.02;
		fHamegChannelMap[3].at(1)=0.02;
		
		
		h->Initialise();
		h->Configure(fHamegChannelMap);
		
		std::cout<<"now keithley"<<std::endl;
		KEITHLEY2700::Keithley2700 *d = new KEITHLEY2700::Keithley2700();
		d->Initialise();
		std::cout<<"initialized"<<endl;
		d->Configure(); 
		std::cout<<"configured"<<endl;
		d->SenseVolt();
		std::cout<<"sense_volt"<<endl;
		d->ConfigureSingleRead();
		
		double volt, amp;
		vector<double> values;
		string readstring;
		
		
		std::cout<<"V1\tI1\tV2\tI2\tV3\tI3\tV4\tI4"<<std::endl;

		for(int j=0; j<10; j++)
		{
			h->MeasAll(fHamegChannelMap);
			for(int i=0;i<4;i++)
			{
				std::cout<<fHamegChannelMap[i].at(0)<<"\t"<<fHamegChannelMap[i].at(1)<<"\t";
				
			}
			d->Read(readstring);
			std::cout<<readstring<<std::endl;
			std::cout<<std::endl;
		}

		
		
		
		delete h;
		delete d;
		
		
		
		MuxTest cAnalogmuxTest( cOffsetTuneMode, cCalibrateTGrp );
		cAnalogmuxTest.InitializeHw( cHWFile );
		cAnalogmuxTest.InitializeSettings( cHWFile );
		cAnalogmuxTest.CreateResultDirectory( cDirectory );
		cAnalogmuxTest.InitResultFile( "CalibrationResults" );
		cAnalogmuxTest.ConfigureHw();
		cAnalogmuxTest.Initialise(); // canvases etc. for fast calibration
		if ( !cVplus ) cAnalogmuxTest.ScanVplus();
		cAnalogmuxTest.ScanOffset();
		cAnalogmuxTest.Validate();
		cAnalogmuxTest.SaveResults();
		
		
	}
	
	
	if ( !batchMode ) cApp.Run();
	std::cout<<"finish amux scans"<<std::endl;
	return 0;
}
