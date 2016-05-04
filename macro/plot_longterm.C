#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <stdio.h>
#include <ctime>

#include "Riostream.h"
#include "TAttLine.h"
#include "TAttMarker.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TClass.h"
#include "TColor.h"
#include "TDatime.h"
#include "TDirectory.h"
#include "TExec.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TKey.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TList.h"
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
#include "TSystem.h"

#define SIZE 10
// bool isFloat( string myString ) {
//     std::istringstream iss(myString);
//     float f;
//     iss >> noskipws >> f; // noskipws considers leading whitespace invalid
//     // Check the entire string was consumed and if either failbit or badbit is set
//     return iss.eof() && !iss.fail(); 
// }
// bool is_not_digit(char c)
// {
// 	return !std::isdigit(c);
// }
// 
// bool numeric_string_compare(const std::string& s1, const std::string& s2)
// {
// 	//     std::string::const_iterator it1 = s1.begin(), it2 = s2.begin();
// 	
// 	int n1, n2;
// 	std::stringstream ss(s1);
// 	ss >> n1;
// 	ss.clear();
// 	ss.str(s2);
// 	ss >> n2;
// 	
// 	return n1 > n2;
// }
void convert_time(string& date, TDatime& d)
{
  struct std::tm tm;
  strptime(date.c_str(), "%FT%TZ", &tm);
  time_t secs_since_epoc=mktime(&tm);
  d.Set(secs_since_epoc,kFALSE);
//   d.Print();  
}
void plot(string datfile)
{
	TFile* file = new TFile("IV_longterm.root","RECREATE");
	
	TCanvas *canvas1  = new TCanvas("cv1","cv1",1);
	TCanvas *canvas2  = new TCanvas("cv2","cv2",1);
	TLegend *leg = new TLegend(.3,.5,.7,.8);
	TLegend *leg1= new TLegend(.6,.1,.975,.4);
	std::vector< std::string > FileAndDataString;
	std::vector< TString > Files;
	std::vector< TString > Name;
	std::vector< TString > Color;
	std::vector< int > color;

	//initialize graphs	
	std::vector< TGraphErrors* > TGraphs;
	std::vector< TGraphErrors* > TGraphs1;
	std::vector<TString> titlevector;
	std::vector< double > ubias;
	std::vector< TMultiGraph* > MGraphs;
	TMultiGraph *mg = new TMultiGraph("I-t"," ; t ; I [A]");
	TMultiGraph *mg1 = new TMultiGraph("V-t"," ; t ; V [V]");
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
	else std::cout << "could not open\n";

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
		TGraphs1.push_back(new TGraphErrors());
		std::cout<<Files.at(i)<<"\t"<<Name.at(i)<<"\t"<<color.at(i)<<std::endl;
		i++;
	}
	
	i=0;
	int j=0;
	for(TString filer : Files){
// 	{
// 		auto filer = Files.at(10);
		//readin file 
		string line1;
		ifstream myfile( filer );
		int k = 0;
		if (myfile)
		{
			int pointcounter=0;
			while (std::getline( myfile, line ))  
			{
				// std::cout<<k<<line<<std::endl;
				if(k>2)
				{
					double x, y, z;
					TDatime  d;
					std::string data_time;
					std::stringstream ss;
					ss<<line;
					ss>>data_time>>y>>z;
					if(data_time.find("Z")!=data_time.npos)
					{
						convert_time(data_time,d);
						
 						if(z>1 || z<-1 ||  d.Convert() <144e6)
 							Printf("Print %s %s %f %f",filer.Data(), data_time.c_str(),y,z);
						// 					if()
						// 					TGraphs.at(j)->TGraphErrors::SetPoint((k-3),(-1.)*x,1./(y*y));
						//   					if(y>=-5e-6)
// 						Printf("Print %s %f %f",data_time.c_str(),y,z);
						TGraphs.at(j)->TGraphErrors::SetPoint(pointcounter,d.Convert(),(-1.)*z);
						TGraphs.at(j)->SetTitle(Name.at(j));
						TGraphs1.at(j)->TGraphErrors::SetPoint(pointcounter,d.Convert(),(-1.)*y);
						TGraphs1.at(j)->SetTitle(Name.at(j));
						pointcounter++;
					}
				}
				k++;

				FileAndDataString.push_back(line);
			}
			if(filer.Contains("iv1.txt"))
				color.at(j)=1;
			else 
				color.at(j)=2;
			TGraphs.at(j)->SetMarkerColor(color.at(j));
			TGraphs.at(j)->SetLineColor(color.at(j));
			TGraphs.at(j)->SetLineWidth(2);
			TGraphs.at(j)->SetMarkerSize(1);
			TGraphs.at(j)->SetMarkerStyle(20);
			TGraphs.at(j)->GetXaxis()->SetTimeDisplay(true);
			TGraphs.at(j)->GetXaxis()->SetNdivisions(-503);
			TGraphs.at(j)->GetXaxis()->SetTimeOffset(0,"gmt");
			
			// TGraphs.at(j)->Print();
			TGraphs.at(j)->Write(Name.at(j));
			mg->Add(TGraphs.at(j),"p");
			
			TGraphs1.at(j)->SetMarkerColor(color.at(j));
			TGraphs1.at(j)->SetLineColor(color.at(j));
			TGraphs1.at(j)->SetLineWidth(2);
			TGraphs1.at(j)->SetMarkerSize(1);
			TGraphs1.at(j)->SetMarkerStyle(20);
			TGraphs1.at(j)->GetXaxis()->SetTimeDisplay(true);
			TGraphs1.at(j)->GetXaxis()->SetNdivisions(-503);
			TGraphs1.at(j)->GetXaxis()->SetTimeOffset(0,"gmt");
			
			// TGraphs.at(j)->Print();
			TGraphs1.at(j)->Write(Name.at(j));
			mg1->Add(TGraphs1.at(j),"p");
			myfile.close();
		}
		j++;
	}
// 	gPad->SetGrid();
	for(auto graphs:TGraphs)
	{
// 		leg->AddEntry(graphs, graphs->GetTitle(),"pl");
// 		graphs->Print();
	}
// 	canvas1->Divide(1,2);
	leg->AddEntry(TGraphs.at(0), "Sensor 01" ,"pl");
	leg->AddEntry(TGraphs.at(1), "Sensor 21" ,"pl");
	leg1->AddEntry(TGraphs1.at(0), "Sensor 01" ,"pl");
	leg1->AddEntry(TGraphs1.at(1), "Sensor 21" ,"pl");
// 	leg->AddEntry(TGraphs.at(0), "Sensor 06" ,"pl");
// 	leg->AddEntry(TGraphs.at(1), "Sensor 16" ,"pl");
// 	leg1->AddEntry(TGraphs1.at(0), "Sensor 06" ,"pl");
// 	leg1->AddEntry(TGraphs1.at(1), "Sensor 16" ,"pl");
	
	canvas1->cd();
	canvas1->SetGrid();
// Module 1	
// 	double low = 1446.7e6; double up =1447.2e6;
// Module 0
	double low = 1447.2e6; double up =1447.8e6;
	
	mg->Draw("ap");
	mg->GetXaxis()->SetTitleSize(0.05);
	mg->GetXaxis()->SetLabelSize(0.05);
	mg->GetXaxis()->SetLabelOffset(0.01);
	mg->GetXaxis()->SetTitleOffset(1);
	mg->GetXaxis()->SetTimeDisplay(1);
	mg->GetXaxis()->SetTimeOffset(0,"gmt");
	mg->GetXaxis()->SetLimits(low,up);
	mg->GetYaxis()->SetTitleSize(0.05);
	mg->GetYaxis()->SetLabelSize(0.05);
	mg->GetYaxis()->SetLabelOffset(0.01);
	mg->GetYaxis()->SetTitleOffset(1);
	leg->Draw();

	canvas1->SaveAs("It.eps");
	canvas1->SaveAs("It.pdf");
	canvas1->SaveAs("It.root");
	canvas2->cd();
	canvas2->SetGrid();
	mg1->Draw("ap");
	mg1->GetXaxis()->SetTitleSize(0.05);
	mg1->GetXaxis()->SetLabelSize(0.05);
	mg1->GetXaxis()->SetLabelOffset(0.01);
	mg1->GetXaxis()->SetTitleOffset(1);
	mg1->GetXaxis()->SetTimeDisplay(1);
	mg1->GetXaxis()->SetTimeOffset(0,"gmt");
	mg1->GetXaxis()->SetLimits(low,up);
	mg1->GetYaxis()->SetTitleSize(0.05);
	mg1->GetYaxis()->SetLabelSize(0.05);
	mg1->GetYaxis()->SetLabelOffset(0.01);
	mg1->GetYaxis()->SetTitleOffset(1);
	
	// 	mg->GetYaxis()->SetRangeUser(1e-8,1e-5);
// 	gPad->SetLogy();
	

// 	leg->AddEntry(TGraphs.at(12), "Wafer 20" ,"pl");
// 
 	leg1->Draw();

	canvas2->SaveAs("Ut.eps");
	canvas2->SaveAs("Ut.pdf");
	canvas2->SaveAs("Ut.root");
	
}
