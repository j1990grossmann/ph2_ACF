#include <cstring>
#include <vector>
#include <TApplication.h>
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include <string>
#include <map>
#include <iostream>
typedef std::map<uint8_t, std::vector<TGraphErrors*> > GraphMap;

int main( int argc, char* argv[] )
{
	
	TApplication cApp( "Root Application", &argc, argv );
	gROOT->SetBatch( true );
	TFile *file = new TFile("../testfile.root","RECREATE");
	GraphMap lala;
	std::vector<TGraphErrors*> peb;
	for(int i=0; i<10;i++)
	{
		std::string depper;
		depper+=i;
		TGraphErrors *d = new TGraphErrors();
		d->SetTitle(depper.c_str());
		d->SetPoint(0,1,1);
		
		peb.push_back(d);
	}
	
	lala[0]=peb;
	for ( const auto& cGraph : lala)
		for(const auto& graphsvector : cGraph.second)
			graphsvector->Write(graphsvector->GetTitle());
	
	file->Write();
	file->Close();

	std::cout<<"finish scans"<<std::endl;
	return 0;
}
