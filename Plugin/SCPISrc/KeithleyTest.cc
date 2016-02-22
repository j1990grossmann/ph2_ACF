#include <iostream>
#include <fstream>
#include <csignal>

#include "../SMUTools/Keithley2410.h"
#include "../SMUTools/RunCounter.h"

#include <vector>

using namespace std;
using namespace KEITHLEY2410;

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
	RUNCOUNTER::Runcounter a;
	a.GetFileName();
// 	string filename(a.GetFileName());
// 	printf("NoiseScanNo_%s\n",filename.c_str());
	KEITHLEY2410::Keithley2410  k("SMUSettings/Keithley.xml");
	KEITHLEY2410::Keithley2410  k1("SMUSettings/Keithley.xml");
	
	string readstr;
// 	k.Initialise();
	k.Configure();
	k.Outp(1);
	k.Read(readstr);
	cout<<readstr<<endl;
	
	k.SourVoltLev("10");
	k.Read(readstr);
	cout<<readstr<<endl;
	k.SourVoltLev("20");
	k.Read(readstr);
	cout<<readstr<<endl;
	sleep(2);
	k.Outp(0);

	return 0;
}
