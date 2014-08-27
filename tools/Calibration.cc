/*!
*
* \file Calibration.cc
* \brief Calibration class, calibration of the hardware
* \author Lorenzo BIDEGAIN
* \date 13/08/14
*
* Support : mail to : lorenzo.bidegain@cern.ch
*
*/

#include "Calibration.h"

Calibration::Calibration(std::string pResultfilepath){
	// if(fResultFile != NULL) delete fResultFile;
	// else{
		fResultFile = TFile::Open(pResultfilepath.c_str(),"RECREATE");
	// }
		// InitialiseTestGroup();
}

Calibration::~Calibration(){

			// fResultFile->Write();
			fResultFile->Close();
}

void Calibration::InitialiseTestGroup()
{
    // Iterating over the Shelves
    for (auto cShelve:fShelveVec)
    {
        for(auto cBoard:(cShelve)->fBoardVector)
        {
            for(auto cFe : cBoard.fModuleVector)
            {
                for(auto cCbc : cFe.fCbcVector)
                {
                  // std::cout<<"Cbc: "<<cCbc<<"created!"<<std::endl;
                	// std::cout << uint32_t((cShelve)->getShelveId()) << " " << uint32_t(cBoard.getBeId()) << " " << uint32_t(cFe.getFeId()) << " " << uint32_t(cCbc.getCbcId()) << std::endl;
                   ConstructTestGroup((cShelve)->getShelveId(),cBoard.getBeId(), cFe.getFeId(), cCbc.getCbcId());
                }
            }
        }
    }
}


void Calibration::ConstructTestGroup(uint8_t pShelveId, uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId)
{
	for(uint8_t lgroup=0; lgroup<8; lgroup++)
	{
		TestGroup fTestgroup(pShelveId, pBeId, pFeId, pCbcId, lgroup);

		std::vector<Channel> cChannelVect;

        // In the CBC Registers, Channels start with 1
		for(uint8_t lchannel=0; lchannel<16; lchannel++)
		{

			if (lchannel*16+lgroup*2 < 254)
			{
				Channel cChannel(pBeId,pFeId,pCbcId, lchannel*16+lgroup*2 +1 );
				// std::cout<<"		Channel: "<<int(lchannel*16+lgroup*2)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

			if ((lchannel*16+lgroup*2) +1 < 254)
			{
				Channel cChannel(pBeId,pFeId,pCbcId, (lchannel*16+lgroup*2) +2 );
				// std::cout<<"		Channel: "<<int( (lchannel*16+lgroup*2) +1)<<"created!"<<std::endl;
				cChannelVect.push_back(cChannel);
			}

		}

		// std::cout << "Shelve " << uint32_t(fTestgroup.fShelveId) << " Be " << uint32_t(fTestgroup.fBeId) << " FE " << uint32_t(fTestgroup.fFeId) << " Cbc " << uint32_t(fTestgroup.fCbcId) << " Group " << uint32_t(fTestgroup.fGroupId) << std::endl;

		TestGroupGraph cTestGroupGraph(pBeId, pFeId, pCbcId, lgroup);


		fTestGroupMap[fTestgroup]=cChannelVect;
		fTestGroupGraphMap[fTestgroup]=cTestGroupGraph;
		// std::cout<<"	Group: "<<int(lgroup)<<" created @ " << &fTestGroupMap[fTestgroup] <<std::endl;

	}
}


void Calibration::VplusScan(){

	// for 4 different values of Vplus, loop over all Test Groups on all Cbc's simultaniously:
	// 1. enable test groups on all Cbc's on 1 FE
	// 2. Loop over Vcth Range and set Vcth on all Cbc's
	// 3. Run and Take Data for all Cbc's on 1 FE
	// 4. Get Event and again loop over all Cbc's on 1 FE and analyze the data

	// fVplusValues.push_back(0x40);
	fVplusValues.push_back(0x50);
	// fVplusValues.push_back(0x60);
	// fVplusValues.push_back(0x70);

	// uint32_t cEventsperVcth = fSettingsMap.find("Nevents")->second;
	uint32_t cEventsperVcth = 10;

	// bool cHoleMode = fSettingsMap.find("HoleMode")->second;
	bool cHoleMode = true;

	uint8_t cTargetVcth = 0x78;

	// Loop over 4 values of Vplus for the Vplus vs. Pedestal (Vcth) graph
	// for(std::vector<uint8_t>::iterator cVplus = fVplusValues.begin(); cVplus != fVplusValues.end(); cVplus++)
	for(uint cVplusindex = 0; cVplusindex < fVplusValues.size(); cVplusindex++)
	{
		uint8_t cVplus = fVplusValues.at(cVplusindex);

		for(std::vector<Shelve*>::iterator cShelve = fShelveVec.begin(); cShelve != fShelveVec.end(); cShelve++)
		{
			// Iterating over the Boards

			for(uint8_t cBeId = 0; cBeId < (*cShelve)->getNBoard(); cBeId++)
			{
				BeBoard* board = (*cShelve)->getBoard(cBeId);

				// Everything starting from here is done on a per-Board basis

				setGlobalReg((*cShelve)->getShelveId(), board->getBeId(), "Vplus", cVplus);

				// Now Vplus is set to the correct value for all Cbc's connected to the current board
				// Now loop over test groups, enable them, loop over Vcth, read & analyze data, fill histograms & disable test groups again
				for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
				{
					EnableTestGroup((*cShelve)->getShelveId(), board->getBeId(), cGroupId, cVplus);

					// Sanity check for Vcth Range
					uint8_t cVcthMin, cVcthMax;
					if(cTargetVcth <= 40) cVcthMin = 0x00;
					else cVcthMin = cTargetVcth - 40;

					if(cTargetVcth >= 215) cVcthMax = 0xFF;
					else cVcthMax = cTargetVcth + 40;

					std::cout << int(cTargetVcth) << " " << (int)cVcthMin << " " << (int)cVcthMax << std::endl;
					// for(uint8_t cVcth = cVcthMin; cVcth < cVcthMax; cVcth+=0x02)
					for(uint8_t cVcth = 100; cVcth < 120; cVcth+=0x01)
					{

						// Set current Vcth value on all Cbc's of the current board
				    	setGlobalReg((*cShelve)->getShelveId(), board->getBeId(), "VCth", cVcth);

						uint32_t cN = 0;
						uint32_t cNthAcq = 0;

						while( cN < cEventsperVcth )
						{
							Run(board, cNthAcq);

							const Event *cEvent = fBeBoardInterface->GetNextEvent(board);
							// std::cout << "processing events from " << cNthAcq << " th Acquisition" << std::endl;

							// Loop over Events from this Acquisition
						    while( cEvent )
						    {

						        if( cN == cEventsperVcth )
						        {
						            break;
						        }

						        FillScurveHists((*cShelve)->getShelveId(), board->getBeId(), cGroupId, cVcth, cEvent);

						        cN++;

						        //
						        if(cN < cEventsperVcth ) cEvent = fBeBoardInterface->GetNextEvent(board);
						        else break;
							}
							cNthAcq++;
						} // End of Analyze Events of last Acquistion loop
					}  // done looping over Vcth, Scurves created

					// Now Fit the hists & fill the VplusVcth Graph & set the TestGroup Offset back to 0x00 or 0xFF

					processSCurves((*cShelve)->getShelveId(), board->getBeId(), cGroupId, cEventsperVcth, cVplus, cHoleMode);
				}
			} // End of Be Loop
		} // End of the Shelve Loop
	} // End of Vplus Loop
}


void Calibration::FitVplusVcth(bool pDoDraw, uint8_t pTargetVcth){

	for(auto& cShelve : fShelveVec)
	{
		// Iterating over the Shelves
		for(BeBoard& cBoard : cShelve->fBoardVector)
		{
			// Loop over all BeBoards, Fes, Cbcs
			// BeBoard* board = (*cShelve)->getBoard(cBeId);

		    for(Module& cModule : cBoard.fModuleVector)
			{
				for(Cbc& cCbc : cModule.fCbcVector)
				{
					TString multigraphname = Form("VplusVcth_Be%d_Fe%d_Cbc%d", cBoard.getBeId(), cModule.getFeId(), cCbc.getCbcId());
					TMultiGraph* cVplusVcthMultiGraph = new TMultiGraph(multigraphname,Form("Vplus vs. Vcth; Vcth; Vplus"));
					TCanvas* cVplusVcthCanvas;

					if (pDoDraw){
						TString canvasname = Form("Be%d_Fe%d_Cbc%d_VplusVcth", cBoard.getBeId(), cModule.getFeId(), cCbc.getCbcId());
						cVplusVcthCanvas = (TCanvas*) gROOT->FindObject(canvasname);
						if (cVplusVcthCanvas != NULL) delete cVplusVcthCanvas;
						cVplusVcthCanvas = new TCanvas(canvasname, canvasname);
						cVplusVcthCanvas->cd();
					}

					for(uint8_t cGroupId = 0; cGroupId < 8; cGroupId++)
					{

						// Create a test Group to find the corresponding in the map
						TestGroup cTempTestGroup(cShelve->getShelveId(), cBoard.getBeId(), cModule.getFeId(), cCbc.getCbcId(), cGroupId);

						// Find it!
						TestGroupGraphMap::iterator cGroup = fTestGroupGraphMap.find(cTempTestGroup);
						if (cGroup != fTestGroupGraphMap.end())
						{
							cGroup->second.fVplusVcthGraph->SetMarkerStyle(8);
							cGroup->second.fVplusVcthGraph->SetMarkerColor(cGroupId+1);
							cVplusVcthMultiGraph->Add(cGroup->second.fVplusVcthGraph);
						}
						else std::cout << "This test group (BE" << cBoard.getBeId() << " FE" << cModule.getFeId() << " CBC" << cCbc.getCbcId() << " Group" << cGroupId << ") was not found!" << std::endl;
					}

					// All Points in the Multigraph; Fit it per Cbc, draw it Per Cbc
					TF1* cVplusVcthFit = new TF1("VplusVcthFit","pol1",1,0xFF);
					TFitResultPtr cVplusVcthFitResult = cVplusVcthMultiGraph->Fit(cVplusVcthFit, "Q");

					if (pDoDraw){
						cVplusVcthMultiGraph->Draw("AP");
						cVplusVcthFit->Draw("same");
						cVplusVcthMultiGraph->GetXaxis()->SetRangeUser(0,255);
						cVplusVcthMultiGraph->SetMinimum(0);
						cVplusVcthMultiGraph->SetMaximum(255);
						cVplusVcthCanvas->Update();
					}
					// Get the right Vplus setting & write to the Cbc
					uint8_t cVplusResult = (uint8_t) ((int) cVplusVcthFit->Eval(pTargetVcth));

					fCbcInterface->WriteCbcReg(cModule.getCbc(cCbc.getCbcId()),"Vplus",cVplusResult);
					std::cout << "Vplus Setting for Be " << (int)cBoard.getBeId() << " Fe " << (int)cModule.getFeId() << " Cbc " << (int)cCbc.getCbcId() << " : " << std::hex << cVplusResult << std::endl;

				} // End of Cbc Loop
			} // End of Fe Loop
		} // End of Be Loop
	} // End of Shelve Loop

	// temporarily close the Result File here
	// fResultFile->Write();
	// fResultFile->Close();
}


void Calibration::setGlobalReg(uint8_t pShelveId, uint8_t pBeId, std::string pRegName, uint8_t pRegValue){
	// Set 1 Register on all Cbcs connected to 1 BeBoard
	BeBoard* cBoard = fShelveVec.at(pShelveId)->getBoard(pBeId);

	for(auto cFe : cBoard->fModuleVector)
	{
		for(Cbc& cCbc : cFe.fCbcVector)
		{
			fCbcInterface->WriteCbcReg(&cCbc, pRegName,pRegValue);
		}
	}
	std::cout << "Setting " << RED << pRegName << RESET << " to Value " << GREEN << (int)pRegValue << RESET << " on all CBCs connected to Be " << int(pBeId) << std::endl;
}

void Calibration::EnableTestGroup(uint8_t pShelveId, uint8_t pBeId, uint8_t pGroupId, uint8_t pVplus){

	for(auto& cGroupIt : fTestGroupMap)
	{
		// check if the Group belongs to the right Shelve, BE and if it is the right group
		if(cGroupIt.first.fShelveId == pShelveId && cGroupIt.first.fBeId == pBeId && cGroupIt.first.fGroupId == pGroupId)
		{
			
			// Iterate through the channels and set Offset to 0x50
			// Vector of pairs for the write MultiReg
			std::vector< std::pair< std::string, uint8_t > > cRegVec;

			for(Channel& cChannel : cGroupIt.second)
			{
				// Initialize the Histogram & fit for the current Vplus Settings
				cChannel.initializeHist(pVplus, true);

				// Write the offset to the Cbcs
				TString cRegName = Form("Channel%03d",cChannel.fChannelId);
				uint8_t cRegValue = 0x50;
				std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
				cRegVec.push_back(cRegPair);

				// Update the Channel Object
				cChannel.setOffset(0x50);
			}

			fCbcInterface->WriteCbcMultReg(fShelveVec.at(pShelveId)->getBoard(pBeId)->getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId),cRegVec);

		}
	}
	std::cout << "Enabled Test group " << GREEN <<  uint32_t(pGroupId) << RESET << " on all Cbc's connected to BeBoard " << RED <<  uint32_t(pBeId) << RESET << std::endl;
}

void Calibration::FillScurveHists(uint8_t pShelveId, uint8_t pBeId, uint8_t pGroupId, uint8_t pVcth, const Event* pEvent){
	// Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms

	for(auto& cGroupIt : fTestGroupMap)
	{
		// check if the Group belongs to the right Shelve, BE and if it is the right group
		if(cGroupIt.first.fShelveId == pShelveId && cGroupIt.first.fBeId == pBeId && cGroupIt.first.fGroupId == pGroupId)
		{
	        uint32_t cNHits = 0;
	        std::vector<bool> cDataBitVector = pEvent->DataBitVector(cGroupIt.first.fBeId, cGroupIt.first.fCbcId);

	        // Now loop over all channels in the TestGroup
			for(Channel& cChannel : cGroupIt.second)
			{
				// In the CBC, Channels start with 1, so decrement by 1 to have the right element from the data bit vector
	            if(cDataBitVector[cChannel.fChannelId-1])
	            {
	            	cChannel.fillHist(pVcth);
	                cNHits++;
	            }
	        } // Channel Loop
		} // If Statement
	}
}

void Calibration::processSCurves(uint8_t pShelveId, uint8_t pBeId, uint8_t pGroupId, uint32_t pEventsperVcth, uint8_t pVplus, bool pHoleMode){
	// Here Loop over all FE's Cbc's, Test Groups & Channels to fill Histograms

    for(auto& cGroupIt : fTestGroupMap)
    {
    	// check if the Group belongs to the right Shelve, BE and if it is the right group
    	if(cGroupIt.first.fShelveId == pShelveId && cGroupIt.first.fBeId == pBeId && cGroupIt.first.fGroupId == pGroupId)
    	{
    		// Iterator for the Channels in the TestGroup
    		// std::vector<Channel>::iterator cChannel = cGroup->second.begin();

    		// Iterate through the channels and set Offset to 0x50
    		// Vector of pairs for the write MultiReg
    		std::vector< std::pair< std::string, uint8_t > > cRegVec;

    		for(Channel& cChannel : cGroupIt.second)
    		{
				cChannel.fitHist(pEventsperVcth, pHoleMode, pVplus, fResultFile);

				// fill test Group TGraphErrors here
				TestGroupGraphMap::iterator cGraphIt = fTestGroupGraphMap.find(cGroupIt.first);
				if(cGraphIt != fTestGroupGraphMap.end()){
					cGraphIt->second.FillVplusVcthGraph(pVplus, cChannel.getPedestal(), cChannel.getNoise());
					if(cChannel.getPedestal() == 0) std::cout << RED << "ERROR " << RESET << "The fit for Channel " << int(cChannel.fChannelId) << " CBC " << int(cChannel.fCbcId) << " FE " << int(cChannel.fFeId) << " did not work correctly!" << std::endl;
				}
				else std::cout << "The Graph for this test Group could not be found! There is a problem!" << std::endl;


				// Reset the Histogram for the next value of Vplus
				// cChannel.resetHist();

				// set offsets back to 0x00 or 0xFF
				TString cRegName = Form("Channel%03d",cChannel.fChannelId);
				uint8_t cRegValue;
				if (pHoleMode) cRegValue = 0x00;
				else cRegValue = 0xFF;
				std::pair<std::string, uint8_t> cRegPair = std::make_pair(cRegName.Data(), cRegValue);
				cRegVec.push_back(cRegPair);

				// Update the Channel Object
				if (pHoleMode) cChannel.setOffset(0x00);
				else cChannel.setOffset(0xFF);
			}

			fCbcInterface->WriteCbcMultReg(fShelveVec.at(pShelveId)->getBoard(pBeId)->getModule(cGroupIt.first.fFeId)->getCbc(cGroupIt.first.fCbcId),cRegVec);
		}
	}
	std::cout << "Disabled Test group " << GREEN <<  uint32_t(pGroupId) << RESET << " on all Cbc's connected to BeBoard " << RED <<  uint32_t(pBeId) << RESET << std::endl;}
