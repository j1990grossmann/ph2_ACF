#include "RunCounter.h"

string RUNCOUNTER::Runcounter::GetFileName()
{
	std::string line;
	std::fstream cFile;
	int cRunNumber;
	if ( boost::filesystem::exists( "NoiseScan/.run_number.txt" ) )
	{
		cFile.open( "NoiseScan/.run_number.txt", std::fstream::out | std::fstream::in );
		if ( cFile.is_open() )
		{
			cFile >> cRunNumber ;

			cRunNumber ++;
			cFile.clear();
			cFile.seekp( 0 );
			cFile << cRunNumber;
			cFile.close();
		}
		else
			std::cout << "File not opened!" << std::endl;
	}
	else
	{
		cRunNumber = 1;
		cFile.open( "NoiseScan/.run_number.txt", std::fstream::out );
		cFile << cRunNumber;
		cFile.close();
	}
	char buffer[1000];
	sprintf(buffer,"run_%03d", cRunNumber );
	return std::string(buffer);
}
void RUNCOUNTER::Runcounter::ResetCount()
{
	if ( boost::filesystem::exists( "NoiseScan/.run_number.txt" ) )
	{
		boost::filesystem::remove("NoiseScan/.run_number.txt");
	}
}
