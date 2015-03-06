#include "MuxTest.h"
#include<cmath>
#include <boost/concept_check.hpp>
// TODO: Canvas divisions


void MuxTest::Initialise()
{
	//is to be called after system controller::ReadHW, ReadSettings
	// populates all the maps

	uint32_t cCbcCount = 0;

	for ( auto cShelve : fShelveVector )
	{
		uint32_t cShelveId = cShelve->getShelveId();

		for ( auto cBoard : cShelve->fBoardVector )
		{
			uint32_t cBoardId = cBoard->getBeId();

			for ( auto cFe : cBoard->fModuleVector )
			{
				uint32_t cFeId = cFe->getFeId();

				for ( auto cCbc : cFe->fCbcVector )
				{
					uint32_t cCbcId = cCbc->getCbcId();
					cCbcCount++;

					// populate the channel vector
					std::vector<Channel> cChanVec;

					for ( uint8_t cChan = 0; cChan < 254; cChan++ )
						cChanVec.push_back( Channel( cBoardId, cFeId, cCbcId, cChan ) );

					fCbcChannelMap[cCbc] = cChanVec;

					// now create the canvasses
					TString cCanvasname = Form( "Fe%d_Cbc%d_AnalogMux", cFeId, cCbcId );
					TCanvas* ctmpCanvas = dynamic_cast<TCanvas*>( gROOT->FindObject( cCanvasname ) );
					if ( ctmpCanvas ) delete ctmpCanvas;
					ctmpCanvas =  new TCanvas( cCanvasname, cCanvasname, 1000, 1000 );
					ctmpCanvas->Divide( 3, 3 );
					fCanvasMap[cCbc] = ctmpCanvas;

					std::vector<TGraphErrors*> GraphVector;
					std::vector<TGraphErrors*> GraphVector1;
					
					TGraphErrors* ctmpGraph;
					TGraphErrors* ctmpGraph1;
										
					// now the TGraphErrors
					for(auto& cNames : fTestRegisterVector)
					{
						TString cGraphname = Form( "Fe%d_Cbc%d_%s", cFeId, cCbcId, cNames.first.c_str() );
						
						ctmpGraph = dynamic_cast<TGraphErrors*>( gROOT->FindObject( cGraphname ) );
						if ( ctmpGraph ) delete ctmpGraph;
						ctmpGraph = new TGraphErrors();
						ctmpGraph->SetName( cGraphname );
						ctmpGraph->SetTitle( cGraphname );
						
						GraphVector.push_back(ctmpGraph);
						
						TString cGraphname1 = Form( "SingleCBC_Fe%d_Cbc%d_%s", cFeId, cCbcId, cNames.first.c_str() );
						
						ctmpGraph1 = dynamic_cast<TGraphErrors*>( gROOT->FindObject( cGraphname1 ) );
						if ( ctmpGraph1 ) delete ctmpGraph1;
						ctmpGraph1 = new TGraphErrors();
						ctmpGraph1->SetName( cGraphname1 );
						ctmpGraph1->SetTitle( cGraphname1 );
						
						GraphVector1.push_back(ctmpGraph1);
					}

					fGraphMap[0] = GraphVector;
					fGraphMap[1] = GraphVector1;
				}
			}
		}
	}
	auto cSetting = fSettingsMap.find( "HoleMode" );
	fHoleMode = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 1;
	cSetting = fSettingsMap.find( "TargetVcth" );
	fTargetVcth = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 120;
	cSetting = fSettingsMap.find( "Nevents" );
	fEventsPerPoint = ( cSetting != std::end( fSettingsMap ) ) ? cSetting->second : 10;
	fNCbc = cCbcCount;

	std::cout << "Created Object Maps and parsed settings:" << std::endl;
	std::cout << "	Hole Mode = " << fHoleMode << std::endl;
	std::cout << "	Nevents = " << fEventsPerPoint << std::endl;
	std::cout << "	TargetVcth = " << int( fTargetVcth ) << std::endl;
	
}

void MuxTest::writeGraphs()
{
	// just use auto iterators to write everything to disk
	for ( const auto& cGraph : fGraphMap )
		for(const auto& graphsvector : cGraph.second)
			graphsvector->Write( graphsvector->GetTitle(), TObject::kOverwrite );
	for ( const auto& cCanvas : fCanvasMap )
		cCanvas.second->Write( cCanvas.second->GetName(), TObject::kOverwrite );
}

void MuxTest::dumpConfigFiles()
{
	// visitor to call dumpRegFile on each Cbc
	struct RegMapDumper : public HwDescriptionVisitor
	{
		std::string fDirectoryName;
		RegMapDumper( std::string pDirectoryName ): fDirectoryName( pDirectoryName ) {};
		void visit( Cbc& pCbc ) {
			if ( !fDirectoryName.empty() ) {
				TString cFilename = fDirectoryName + Form( "/FE%dCBC%d.txt", pCbc.getFeId(), pCbc.getCbcId() );
				// cFilename += Form( "/FE%dCBC%d.txt", pCbc.getFeId(), pCbc.getCbcId() );
				pCbc.saveRegMap( cFilename.Data() );
			}
			else std::cout << "Error: no results Directory initialized! "  << std::endl;
		}
	};

	RegMapDumper cDumper( fDirectoryName );
	accept( cDumper );

	std::cout << BOLDBLUE << "Configfiles for all Cbcs written to " << fDirectoryName << RESET << std::endl;
}

void MuxTest::ScanVplusAMux()
{
	int i=0;
	for(auto& amuxregisterpair : fTestRegisterVector )
	{
// 		InitializeHw(pHWfile);
		InitializeSettings(pHWfile);
		ConfigureHw();
		
		for(auto it : fSettingsMap)
		{
			std::cout<<it.first<<"\t"<<it.second<<std::endl;
		}
		
		std::cout<<BOLDBLUE<<"Scanning "<<amuxregisterpair.first<<" with Amux Register setting "<<amuxregisterpair.second<<"\t"<<i<<RESET<<endl;
		CbcRegWriter cWriter1( fCbcInterface, "MiscTestPulseCtrl&AnalogMux", amuxregisterpair.second );
		accept( cWriter1 );
		
		int j=0;
		
		for ( auto cShelve : fShelveVector )
		{
			for ( auto cBoard : cShelve->fBoardVector )
			{
				for ( auto cFe : cBoard->fModuleVector )
				{
					j=0;
// 					for ( std::vector<int>::iterator cRegVal = fCBCRegVector.end() ; !fCBCRegVector.begin(); --cRegVal )
//  					for ( std::vector<u_int8_t>::iterator cRegVal = fCBCRegVector.begin() ; cRegVal != fCBCRegVector.end(); ++cRegVal )
 					for ( std::vector<u_int8_t>::iterator cRegVal = fCBCRegVector.end() ; cRegVal != fCBCRegVector.begin(); --cRegVal )
// 					for ( auto& cRegVal : fCBCRegVector )
					{
						// 		now loop over all register values
						for ( auto cCbc : cFe->fCbcVector )
						{
							// Set a register for scanning
// 							CbcRegWriter cWriter( fCbcInterface, amuxregisterpair.first, cRegVal );
							CbcRegWriter cWriter( fCbcInterface, amuxregisterpair.first, *cRegVal );
							accept( cWriter );
							std::cout << amuxregisterpair.first <<"\t" << int( *cRegVal )<<"\t";
// 							drawOnline(cCbc);
						}
// 						std::this_thread::sleep_for(std::chrono::milliseconds(10));
						this->SMUScan();
// 						for(int i=0; i<9; i++)
// 						{
// 							std::cout<<fSMUScanVector.at(i)<<"\t";
// 						}
// 						std::cout<<std::endl;
						
						if(amuxregisterpair.second==1 || amuxregisterpair.second==11 || amuxregisterpair.second==16 )
						{
							fGraphMap[0].at(i)->SetPoint(j, int(*cRegVal), fSMUScanVector.at(8));
							fGraphMap[1].at(i)->SetPoint(j, int(*cRegVal), fSMUScanVector.at(8));
							std::cout<<j<<"\t"<<int(*cRegVal)<<"\t"<<fSMUScanVector.at(8)<<endl;
						}else
						{
							fGraphMap[0].at(i)->SetPoint(j, int(*cRegVal), fSMUScanVector.at(1));
							fGraphMap[1].at(i)->SetPoint(j, int(*cRegVal), fSMUScanVector.at(1)/2.);
							std::cout<<j<<"\t"<<int(*cRegVal)<<"\t"<<fSMUScanVector.at(1)<<endl;
						}
						j++;
					}
				}
			}
		}
		for ( auto cShelve : fShelveVector )
		{
			for ( auto cBoard : cShelve->fBoardVector )
			{
				for ( auto cFe : cBoard->fModuleVector )
					CbcInterface::ReadAllCbc(cFe);
			}
		}
		
		drawOnline();
		std::cout<<BOLDBLUE<<"Finnished scanning "<<amuxregisterpair.first<<" with Amux Register setting "<<amuxregisterpair.second<<"\t"<<i<<"\t reconfigure "<<RESET<<endl;
		i++;
	}
}

void MuxTest::SMUInitialiseAndConfigure()
{
	//First configure keithley then hameg for safety reasons. 
	keithley = new KEITHLEY2700::Keithley2700();
	keithley->Initialise();
	keithley->Configure(); 
	keithley->SenseVolt();
	keithley->ConfigureSingleRead();

	hameg = new HAMEG4040::Hameg4040();
	

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

	hameg->Initialise();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	hameg->Configure(fHamegChannelMap);
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
void MuxTest::SMUScan()
{
	string readstring;
	double volt;
	{
		hameg->MeasAll(fHamegChannelMap);
// 		for(int i=0;i<4;i++)
// 		{
// 			std::cout<<fHamegChannelMap[i].at(0)<<"\t"<<fHamegChannelMap[i].at(1)<<"\t";
// 			
// 		}
		keithley->Read(readstring);
		volt = TString(((TString)readstring)(0,15)).Atof();
	}
	for(int i=0;i<8;i++)
	{
		fSMUScanVector.at(i)=fHamegChannelMap[i/2].at(i%2);
	}
	fSMUScanVector.at(8)=volt;
}
void MuxTest::SMUKill()
{
	delete hameg;
	delete keithley;
	std::cout<<RESET<<"Shutdown Hameg and Multimeter"<<std::endl;
}

void MuxTest::drawOnline()
{
	for ( auto cShelve : fShelveVector )
	{
		for ( auto cBoard : cShelve->fBoardVector )
		{
			for ( auto cFe : cBoard->fModuleVector )
			{
				for ( auto cbc : cFe->fCbcVector )
				{
					CanvasMap::iterator cCanvas = fCanvasMap.find(cbc);
					if ( cCanvas == std::end( fCanvasMap )) std::cout << "Error: could not find the CBC " << int( cbc->getCbcId() ) << std::endl;
					else
					{
						GraphMap::iterator cGraphs = fGraphMap.find(0);
						int padno = 1;
						for(auto&graph : cGraphs->second)
						{
							cCanvas->second->cd(padno);
							gPad->SetGridx();
							gPad->SetGridy();
							graph->SetMarkerStyle(1);
							graph->SetMarkerSize(4);
							graph->Draw("ap");
							padno++;
						}
						cCanvas->second->Update();
					}
				}
			}
		}
	}
}
