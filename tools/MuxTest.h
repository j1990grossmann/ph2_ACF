/*!
*
* \file MuxTest.h
* \brief Analoge MUX Test class for irradiation testing, temperature scans and general testing 
* \author Johannes Grossmann
* \date 18 / 02 / 15
*
* \Support : johannes.grossmann@cern.ch
*
*/

#ifndef MuxTest_h__
#define MuxTest_h__

#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../System/SystemController.h"
#include "../Utils/ConsoleColor.h"
#include "Channel.h"
// #include "Hameg4040.h"
#include "../Utils/Visitor.h"
#include "../Utils/CommonVisitors.h"


#include <map>

#include "TCanvas.h"
#include "TProfile.h"
#include "TString.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TText.h"
#include "TSystem.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;
using namespace Ph2_System;


typedef std::map<Cbc*, std::vector<Channel> > CbcChannelMap;
typedef std::map<Cbc*, TCanvas*> CanvasMap;
typedef std::map<Cbc*, TGraphErrors*> GraphMap;
typedef std::map<Cbc*, TF1*> FitMap;
typedef std::map<Cbc*, TH1F*> HistMap;
typedef std::vector<std::pair< std::string, uint8_t> > RegisterVector;
typedef std::map< int, std::vector<uint8_t> >  TestGroupChannelMap;
/*
Key=-1 to do calibration on all channels
Key=0-7 for the 8 Test Groups
*/

class MuxTest : public SystemController
{
  public:
	MuxTest( bool pbitwisetune , bool pAllChan ) {
		for(int i=0; i<25; i++)
		{
			fVplusVec1.push_back(i*10);
		}
		fVplusVec.push_back( 0x14 );
		fVplusVec.push_back( 0x64 );
		fVplusVec.push_back( 0xA4 );
		fdoTGrpCalib = !pAllChan;
		fdoBitWisetuning = pbitwisetune;
		for ( int gid = -1; gid < 8; gid++ ) {
			std::vector<uint8_t> tempchannelVec;
			if ( gid > -1 ) {
				for ( int idx = 0; idx < 16; idx++ ) {
					int ctemp1 = idx * 16 + gid * 2;
					int ctemp2 = ctemp1 + 1;
					if ( ctemp1 < 254 ) tempchannelVec.push_back( ctemp1 );
					if ( ctemp2 < 254 )  tempchannelVec.push_back( ctemp2 );

				}
			}
			else {
				for ( int idx = 0; idx < 254; idx++ )
					tempchannelVec.push_back( idx );
			}
			fTestGroupChannelMap[gid] = tempchannelVec;
		}

	}
	~MuxTest() {
		if ( fResultFile ) {
			fResultFile->Write();
			fResultFile->Close();
		}
	}
	// methods
	void Initialise();  // wants to be called after SystemController::ReadHW, ReadSettings
	void ScanVplus();
	void ScanOffset();
// 	void HamegTest();

	void SaveResults() {
		writeGraphs();
		dumpConfigFiles();
	}

	void Validate();
	
	void ScanVplusAMux();

  private:
	CbcChannelMap fCbcChannelMap;
	CanvasMap fCanvasMap;
	GraphMap fGraphMap;
	GraphMap fGraphMap1;
	FitMap fFitMap;
	HistMap fHistMap;
	TestGroupChannelMap fTestGroupChannelMap;
	bool fdoTGrpCalib;
	bool fdoBitWisetuning;
	bool fHoleMode;
	uint32_t fEventsPerPoint;
	uint32_t fNCbc;
	uint8_t fTargetVcth;

	std::vector<uint8_t> fVplusVec;
	std::vector<uint8_t> fVplusVec1;


  protected:
	void measureSCurves( bool pOffset, int  pTGrpId );
	void measureSCurvesBinary( bool pOffset, int  pTGrpId );
	//void measureSCurves( bool pOffset );
	void setOffset( uint8_t pOffset, int  pTGrpId );
	//void setOffset( uint8_t pOffset );
	void toggleOffsetBit( uint8_t pBit, int  pTGrpId );
	//void toggleOffsetBit( uint8_t pBit );
	uint32_t fillSCurves( BeBoard* pBoard,  const Event* pEvent, uint8_t pValue, int  pTGrpId, bool pDraw = false );
	//uint32_t fillSCurves( BeBoard* pBoard,  const Event* pEvent, uint8_t pValue, bool pDraw = false );
	void initializeSCurves( TString pParameter, uint8_t pValue, int  pTGrpId );
	//void initializeSCurves( TString pParameter, uint8_t pValue );
	void processSCurves( TString pParameter, uint8_t pValue, bool pDraw, int  pTGrpId );
	void processSCurvesOffset( TString pParameter, uint8_t pTargetBit, bool pDraw, int pTGrpId );
	// void fitVplusVcthGraph();
	void findVplus( bool pDraw );
	void writeGraphs();
	void dumpConfigFiles();

};


#endif
