#include <iostream>
#include <fstream>
#include <csignal>

#include "../SMUTools/Hameg4040.h"
#include "../SMUTools/Keithley2700.h"
#include <vector>

using namespace std;
using namespace KEITHLEY2700;
using namespace HAMEG4040;
// using namespace HAMEG4040;
using namespace boost;
void signalHandler( int signum )
{
//     cout << "Interrupt signal (" << signum << ") received.\n";
	
    // cleanup and close up stuff here  
    // terminate program  

   exit(signum);  

}

int main(int argc, char* argv[])
{
	signal(SIGINT, signalHandler);  
	
	//File for data:
// 	ofstream MeasFile;
// 	MeasFile.open("./ResultsScan/Measurement.txt");
	
	
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
// 	auto start = std::chrono::system_clock::now();
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
// 	auto stop = std::chrono::system_clock::now();
	
// 	auto diff = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	
// 	std::cout<<"readout rate 4 channels "<<diff.count()<<std::endl;
	

	
	delete h;
 	delete d;
	

	return 0;
}
