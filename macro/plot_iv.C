#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <sstream>
#include <bitset>

#include "Riostream.h"
#include "TAttMarker.h"
#include "TAxis.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TCanvas.h"
#include "TClass.h"
#include "TColor.h"
#include "TDirectory.h"
#include "TExec.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TH2.h"
#include "TKey.h"
#include "TKey.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TPad.h"
#include "TPostScript.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TSpectrum.h"
#include "TString.h"
#include "TStyle.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TList.h"
#include "TAttMarker.h"
#include "TAttLine.h"
#include "TH1.h"

#define SIZE 10
bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}
bool is_not_digit(char c)
{
	return !std::isdigit(c);
}

bool numeric_string_compare(const std::string& s1, const std::string& s2)
{
	//     std::string::const_iterator it1 = s1.begin(), it2 = s2.begin();
	
	int n1, n2;
	std::stringstream ss(s1);
	ss >> n1;
	ss.clear();
	ss.str(s2);
	ss >> n2;
	
	return n1 > n2;
}

void plot(string datfile)
{
	TFile* file = new TFile("IV.root","RECREATE");
	
	TCanvas *canvas1  = new TCanvas("cv1","cv1",1);
	TLegend *leg = new TLegend(.2,.65,.7,.95);
	std::vector< std::string > FileAndDataString;
	std::vector< TString > Files;
	std::vector< TString > Name;
	std::vector< TString > Color;
	std::vector< int > color;

	//initialize graphs	
	std::vector< TGraphErrors* > TGraphs;
	std::vector<TString> titlevector;
	std::vector< double > ubias;
	std::vector< TMultiGraph* > MGraphs;
	TMultiGraph *mg = new TMultiGraph("IV"," ; U [V]; I [A]");
	//readin file 
	string line;
	ifstream myfile( datfile );
	int i = 0;

	if (myfile)
	{
		while (std::getline( myfile, line ))  
		{
// 			if(i>0)
				FileAndDataString.push_back(line);
			// 			std::cout<<line<<std::endl;
			i++;
		}
		myfile.close();
	}
	else{
	  std::cout << "could not open\n";
	  exit(1);
	}

	i=0;
	for(auto atline : FileAndDataString)
	{
		std::cout<<atline<<std::endl;
		std::string dummy, dummy1;
		std::string::size_type dummysize;
		std::stringstream ss;
		ss<<atline;
		ss>>dummy;
// 		dummy1+=dummy;ss>>dummy;dummy1+=" "+dummy;ss>>dummy;dummy1+=" "+dummy;
		Files.push_back(dummy);
		ss>>dummy;
		Name.push_back(dummy);
		ss>>dummy;
		Color.push_back(dummy);
		color.push_back(std::stoi((string)Color.at(i),&dummysize));
		TGraphs.push_back(new TGraphErrors());
		std::cout<<Files.at(i)<<"\t"<<Name.at(i)<<"\t"<<color.at(i)<<std::endl;
		i++;
	}
	
	i=0;
	int j=0;
	for(auto filer : Files){
// 	{
// 		auto filer = Files.at(10);
		//readin file 
		string line1;
		ifstream myfile( filer );
		int k = 0;
		if (myfile)
		{
			while (std::getline( myfile, line ))  
			{
				// std::cout<<k<<line<<std::endl;
				if(k>2)
				{
					double x, y, z;
					std::stringstream ss;
					ss<<line;
					ss>>x>>y;
// 					Printf("Print %f %f %f",x,y,z);
					// 					if()
// 					TGraphs.at(j)->TGraphErrors::SetPoint((k-3),(-1.)*x,1./(y*y));
//   					if(y>=-5e-6)
					TGraphs.at(j)->TGraphErrors::SetPoint((k-3),(-1.)*x,(-1.)*y);
					TGraphs.at(j)->SetTitle(Name.at(j));
				}
				k++;

				FileAndDataString.push_back(line);
			}
			TGraphs.at(j)->SetMarkerColor(color.at(j));
			TGraphs.at(j)->SetLineColor(color.at(j));
			TGraphs.at(j)->SetLineWidth(2);
			TGraphs.at(j)->SetMarkerSize(1);
			TGraphs.at(j)->SetMarkerStyle(20);
			// TGraphs.at(j)->Print();
			TGraphs.at(j)->Write(Name.at(j));
			mg->Add(TGraphs.at(j),"p");
			myfile.close();
		}
		j++;
	}
	gPad->SetGrid();
	for(auto graphs:TGraphs)
	{
		leg->AddEntry(graphs, graphs->GetTitle(),"pl");
	}
	mg->Draw("ap");

	mg->GetXaxis()->SetTitleSize(0.05);
	mg->GetXaxis()->SetLabelSize(0.05);
	mg->GetXaxis()->SetLabelOffset(0.01);
	mg->GetXaxis()->SetTitleOffset(1);
	
	mg->GetYaxis()->SetTitleSize(0.05);
	mg->GetYaxis()->SetLabelSize(0.05);
	mg->GetYaxis()->SetLabelOffset(0.01);
	mg->GetYaxis()->SetTitleOffset(1);
	mg->GetYaxis()->SetRangeUser(-1e-6,1e-5);
	mg->GetXaxis()->SetRangeUser(-10,300);
	// gPad->SetLogy();
	
// 	leg->AddEntry(TGraphs.at(0), "Wafer 11" ,"pl");
// 	leg->AddEntry(TGraphs.at(7), "Wafer 17" ,"pl");
// 	leg->AddEntry(TGraphs.at(12), "Wafer 20" ,"pl");

	leg->Draw();
	canvas1->SaveAs("IV.eps");
	canvas1->SaveAs("IV.pdf");
	canvas1->SaveAs("IV.C");
	canvas1->SaveAs("IV.root");

}


