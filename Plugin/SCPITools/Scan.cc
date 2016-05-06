#include "Scan.h"
#include <boost/concept_check.hpp>

void SCAN::Scan::ParseSettingsXML(const std::string& pFilename, std::ostream& os)
{
	std::cout<<"parsed file"<<std::endl;
// 	printf("%e %e %e %e %d %d %d %d\n", fScanconfig.V_max, fScanconfig.V_min, fScanconfig.V_step, fScanconfig.I_compliance, fScanconfig.Vcth_max, fScanconfig.Vcth_min, fScanconfig.Vcth_step, fScanconfig.No_events);
// 	printf("%s %s\n", fScanconfig.SerialFileKeithley.c_str(), fScanconfig.SerialFileHameg.c_str());
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( pFilename.c_str() );
	if ( !result )
	{
		cout << "ERROR :\n Unable to open the file : " << pFilename<< std::endl;
		cout << "Error description : " << result.description() << std::endl;
		return ;
	}
	for ( pugi::xml_node nSettings = doc.child( "Settings" ); nSettings; nSettings = nSettings.next_sibling() )
	{
		int i=0;
		pugi::xml_attribute attr;
		for ( pugi::xml_node nSetting = nSettings.child( "Setting" ); nSetting; nSetting = nSetting.next_sibling() )
		{
			if(attr=nSetting.attribute( "rs232_file" )){
// 				if((string)nSetting.attribute( "rs232_file" ).value()=="Hameg4040")
// 				{
// 					fScanconfig.SerialFileHameg=nSetting.first_child().value();
// 				}
				if((string)nSetting.attribute( "rs232_file" ).value()=="Keithley")
				{
					fScanconfig.SerialFileKeithley=nSetting.first_child().value();
					fScanconfig.SerialConfigVec.emplace_back(i,nSetting.first_child().value());
					i++;
				}
// 				if((string)nSetting.attribute( "rs232_file" ).value()=="Keithley1")
// 				{
// 					fScanconfig.SerialFileKeithley1=nSetting.first_child().value();
// 				}
				cout << RED << "Setting" << RESET << " --" << BOLDCYAN << nSetting.attribute( "rs232_file" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
			}
		}
		for ( pugi::xml_node nSetting = nSettings.child( "Voltages" ).child("Voltage"); nSetting; nSetting = nSetting.next_sibling() )
		{				
			if((string)nSetting.attribute("Id").as_string()=="min"){
				cout << RED << "Voltage_min" << RESET << " --" << BOLDCYAN << nSetting.attribute( "min").value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fScanconfig.V_min=atof(nSetting.first_child().value());
			}
			if((string)nSetting.attribute("Id").as_string()=="max"){
				cout << RED << "Voltage_max" << RESET << " --" << BOLDCYAN << nSetting.attribute( "max" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fScanconfig.V_max=atof(nSetting.first_child().value());
			}
			if((string)nSetting.attribute("Id").as_string()=="step"){
				cout << RED << "Voltage_step" << RESET << " --" << BOLDCYAN << nSetting.attribute( "step" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fScanconfig.V_step=atof(nSetting.first_child().value());
			}
			if((string)nSetting.attribute("Id").as_string()=="dV_dt_V_per_s"){
				cout << RED << "dV_dt_V_per_s" << RESET << " --" << BOLDCYAN << nSetting.attribute( "dV_dt_V_per_s" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fScanconfig.dV_dt=atof(nSetting.first_child().value());
			}
// 			cout << RED << "Voltage_min" << RESET << " --" << BOLDCYAN << nSetting.attribute("Id").as_string() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
		}
		for ( pugi::xml_node nSetting = nSettings.child( "CurrentsMax" ).child("CurrentMax"); nSetting; nSetting = nSetting.next_sibling() )
		{				
			if(attr=nSetting.attribute( "Id" )){
				cout << RED << "CurrentMax Channel" << RESET << " --" << BOLDCYAN << nSetting.attribute( "Id" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fScanconfig.I_compliance=atof(nSetting.first_child().value());
			}
		}
		for ( pugi::xml_node nSetting = nSettings.child( "Vcth" ).child("Setting"); nSetting; nSetting = nSetting.next_sibling() )
		{				
			if(attr=nSetting.attribute( "Vcth" )){
				if((string)nSetting.attribute( "Vcth" ).value()=="min" )
					fScanconfig.Vcth_min=atoi(nSetting.first_child().value());
				if((string)nSetting.attribute( "Vcth" ).value()=="max" )
					fScanconfig.Vcth_max=atoi(nSetting.first_child().value());
				if((string)nSetting.attribute( "Vcth" ).value()=="step" )
					fScanconfig.Vcth_step=atoi(nSetting.first_child().value());
				if((string)nSetting.attribute( "Vcth" ).value()=="events")
					fScanconfig.No_events=atoi(nSetting.first_child().value());
				cout << RED << "Vcth" << RESET << " --" << BOLDCYAN << (string)nSetting.attribute( "Vcth" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
			}
		}
		std::cout<<"parsed file"<<std::endl;
// 		printf("%e %e %e %e %d %d %d %d\n", fScanconfig.V_max, fScanconfig.V_min, fScanconfig.V_step, fScanconfig.I_compliance, fScanconfig.Vcth_max, fScanconfig.Vcth_min, fScanconfig.Vcth_step, fScanconfig.No_events);
// 		printf("%s %s\n", fScanconfig.SerialFileKeithley.c_str(), fScanconfig.SerialFileHameg.c_str());
	}
}
string SCAN::Scan::exec(const char* cmd)
{
	FILE* pipe = popen(cmd, "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	std::string result;
	int counter=0;
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
		{
			if(counter%10==0)
				printf(buffer);
			if(std::string(buffer).find("Saving binary raw data to:") != std::string::npos)
			{
				vector<string> tokenized_strings;string dummy(buffer); 
				this->Tokenizer(tokenized_strings,dummy,boost::char_separator<char>(" "));
// 				cout<<tokenized_strings.at(tokenized_strings.size()-1)<<endl;
				result=tokenized_strings.at(tokenized_strings.size()-1);
				result= result.substr(0,result.size()-5);
// 				for(auto it=tokenized_strings.begin(); it!=(tokenized_strings.end()); ++it)
// 				{
// 					cout<<*it<<endl;
// 				}
			}
			counter++;
		}
	}
	pclose(pipe);
	return result;
}
void SCAN::Scan::StartScan(bool cIV, string cAngle, string cPosX, string cPosZ)
{
	char linestring[10000];
	int V_steps,Vcth_steps,	V_dir, Vcth_dir;
	if(fScanconfig.V_step<0.001 && -0.001<fScanconfig.V_step)
		V_steps=1;
	else
		V_steps=round(fabs(fScanconfig.V_max-fScanconfig.V_min)/fabs(fScanconfig.V_step)+1);
	
	Vcth_steps=round(fabs(fScanconfig.Vcth_max-fScanconfig.Vcth_min)/fabs(fScanconfig.Vcth_step)+1);
	V_dir = ((fScanconfig.V_max-fScanconfig.V_min)>0)?1:-1;
	Vcth_dir = ((fScanconfig.Vcth_max-fScanconfig.Vcth_min)>0)?1:-1;
	
	if(abs(fScanconfig.dV_dt)>0.000001)
	{
		fScanconfig.Dt=fabs(fScanconfig.V_step/fScanconfig.dV_dt);
		cout<<"Wait Time between two set points in s"<<fScanconfig.Dt<<endl;
	}else{
		cout<<"A zero value for ramprate was specified"<<endl;
		exit(1);
	}
	this->FileGenerator();
	
 	std::vector <KEITHLEY2410::Keithley2410> keithleyvec;
	for(auto i:fScanconfig.SerialConfigVec)
	{
 		keithleyvec.push_back(KEITHLEY2410::Keithley2410(i.second));
		cout<<i.second<<endl;
	}
	
//  	KEITHLEY2410::Keithley2410  k(fScanconfig.SerialFileKeithley);
	// KEITHLEY2410::Keithley2410  k1(fScanconfig.SerialFileKeithley1);
	string readstr;
	string readstr1("0,0");
	for(auto i: keithleyvec)
	{
		i.Configure();
		i.SenseCurrProt(to_string(fScanconfig.I_compliance));
		i.Outp(1);
	}
// 	k.Configure();
// 	k.SenseCurrProt(to_string(fScanconfig.I_compliance));
// 	k.Outp(1);

 	// k1.Configure();
 	// k1.SenseCurrProt(to_string(fScanconfig.I_compliance));
 	// k1.Outp(1);

	pugi::xml_node node = cFile.append_child();
	node.set_name("Noise_VS_Bias");

// 	cout<<readstr<<endl;

// Now preramp up 
	// preramp(true,k, k1);
// 	preramp(true,k, k);
	for(int i=0; i<V_steps; i++)
	{
		double V=fScanconfig.V_min+fScanconfig.V_step*i*V_dir;
		for(auto i: keithleyvec)
		{
			i.SourVoltLev(to_string(V));
		}

 		// k1.SourVoltLev(to_string(V));
		if(cIV)
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(fScanconfig.Dt*1000)));
		else
			std::this_thread::sleep_for(std::chrono::seconds(5));
		pugi::xml_node descr = node.append_child();
		descr.set_name("Measurement");
		descr.append_attribute("V_set") = V;
		if(cIV)
			Vcth_steps=1;
		for(int j=0; j<Vcth_steps;j++)
		{
			int Vcth=fScanconfig.Vcth_min+fScanconfig.Vcth_step*j*Vcth_dir;
			for(auto i: keithleyvec)
			{
				i.Read(readstr);
				Tokenizer(datavec, readstr,boost::char_separator<char>(","));

			}
//  			k.Read(readstr);
 			// k1.Read(readstr1);
			Tokenizer(datavec1, readstr1,boost::char_separator<char>(","));
			if(!cIV){
				this->ReadSetConfigFile(Vcth, cbc1configfilename);
				this->ReadSetConfigFile(Vcth, cbc2configfilename);
			}
			string execstring="miniDAQ -f settings/Beamtest_Nov15.xml -e ";
			std::chrono::time_point<std::chrono::system_clock> start, end;
			// Start the run here
			start = std::chrono::system_clock::now();
			execstring+=to_string(fScanconfig.No_events);
			
			string raw_file_name;
			if(!cIV)
			{
				raw_file_name=this->exec(execstring.c_str());
				// raw_file_name="test";
			}
			else
				raw_file_name="IV_Curve";
				
			end = std::chrono::system_clock::now();

			std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			std::time_t start_time = std::chrono::system_clock::to_time_t(start);

			struct tm *end_time_tm;
			struct tm *start_time_tm;

			int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();
			double Rate;
			if(elapsed_milliseconds!=0)
				Rate=(fScanconfig.No_events*1000)/(elapsed_milliseconds);
			else
				Rate=66666666666;
			end_time_tm = localtime(&end_time);
			start_time_tm = localtime(&start_time);char buffer [80];char buffer1 [80];
			strftime (buffer,80,"%FT%TZ",start_time_tm);
			strftime (buffer1,80,"%FT%TZ",end_time_tm);
			
			printf("Voltage %.2e Vcth %d V_read %s I_read %s V1_read %s I1_read %s Filename %s\n",V, Vcth, datavec.at(0).c_str(), datavec.at(1).c_str(), datavec1.at(0).c_str(),datavec1.at(1).c_str(),raw_file_name.c_str());
			ivFile<<datavec.at(0)<<"\t"<<datavec.at(1)<<endl;
			ivFile1<<datavec1.at(0)<<"\t"<<datavec1.at(1)<<endl;
			pugi::xml_node param = descr.append_child();
			param.set_name("Vcth");
			param.append_attribute("Filename");
			param.attribute("Filename").xml_attribute::set_value(raw_file_name.c_str());
			param.append_attribute("Vcth");
			param.attribute("Vcth").xml_attribute::set_value(Vcth);
 			param.append_attribute("V");
			param.attribute("V").set_value(datavec.at(0).c_str());
 			param.append_attribute("I");
			param.attribute("I").set_value(datavec.at(1).c_str());
 			param.append_attribute("V1");
			param.attribute("V1").set_value(datavec1.at(0).c_str());
 			param.append_attribute("I1");
			param.attribute("I1").set_value(datavec1.at(1).c_str());
 			param.append_attribute("Nevents");
			param.attribute("Nevents").set_value(fScanconfig.No_events);
 			param.append_attribute("Start");
			param.attribute("Start").set_value(buffer);
 			param.append_attribute("Stop");
			param.attribute("Stop").set_value(buffer1);
 			param.append_attribute("Rate_Hz");
			param.attribute("Rate_Hz").set_value(Rate);
 			param.append_attribute("Position_x");
			param.attribute("Position_x").set_value(cPosX.c_str());
 			param.append_attribute("Position_z");
			param.attribute("Position_z").set_value(cPosZ.c_str());
 			param.append_attribute("Angle");
			param.attribute("Angle").set_value(cAngle.c_str());
			cFile.save_file(Data_name.c_str());	
		}
	}
// 	Rampdown
	V_dir=V_dir*-1;
	for(int i=0; i<V_steps; i++)
	{
		double V=fScanconfig.V_max+fScanconfig.V_step*i*V_dir;
		for(auto j: keithleyvec)
		{
			j.SourVoltLev(to_string(V));

			// 		k.SourVoltLev(to_string(V));
			// k1.SourVoltLev(to_string(V));
			
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(fScanconfig.Dt*1000)));
			j.Read(readstr);
		}
// 			k.Read(readstr);
 		// k1.Read(readstr1);
		Tokenizer(datavec, readstr,boost::char_separator<char>(","));
		printf("Voltage %.2e V_read %s I_read %s\n",V, datavec.at(0).c_str(), datavec.at(1).c_str());
	}
	
	// preramp(false,k, k1);
// 	preramp(false,k, k);

for(auto j: keithleyvec)	
{
	j.SourVoltLev("0");
}
	std::this_thread::sleep_for(std::chrono::seconds(5));

	for(auto j: keithleyvec)	
{
	j.Read(readstr);
	j.Outp(0);
}


// 	k.SourVoltLev("0");
 	// k1.SourVoltLev("0");

// 	k.Read(readstr);
// 	k.Outp(0);
	
 	// k1.Read(readstr);
 	// k1.Outp(0);
	
}

void SCAN::Scan::FileGenerator()
{
	RUNCOUNTER::Runcounter runcounter;string rundstring(runcounter.GetFileName());
	fOutFilename.append(rundstring.c_str());
	string dirname=	fOutFilename;
	vector<string> OutfilenamesTmp;
	for( auto files : fScanconfig.SerialConfigVec)
		Outfilenames.push_back(fOutFilename+"/"+rundstring+"_iv_"+to_string(files.first)+".txt");
	string IV_name =fOutFilename+"/"+rundstring+"_iv.txt";
	string IV_name1 =fOutFilename+"/"+rundstring+"_iv1.txt";
	Data_name=fOutFilename+"/"+rundstring+".xml";

	boost::filesystem::create_directory(dirname);
// 	 	printf("%s\n",fOutFilename.c_str());
	 	printf("%s\n",dirname.c_str());
// 	 	printf("%s\n",Data_name.c_str());
// 	 	printf("%s\n",IV_name.c_str());
		time_t ltime;struct tm *newtime;
		time(&ltime);  newtime = localtime(&ltime);	

	for(auto files:Outfilenames)
	{
		cout<<files<<endl;
		if ( !boost::filesystem::exists( files ) )
		{
			std::fstream tmp;
			tmp.open( files, std::fstream::out);
			if ( !tmp.is_open() )
			{
				std::cout << "File not opened!" << std::endl;
				return;
			}
			tmp<<"Run started at:"<<endl<<asctime(newtime);
			tmp<<"Voltage [V]\tCurrent [A]\tStepsize[V]:\t"<<setprecision(3)<<scientific<<fScanconfig.V_step<<"\tCompliance[A]:\t"<<fScanconfig.I_compliance<<"\tRamprate [V/s]:\t"<<fScanconfig.dV_dt<<endl;
			tmp.close();
		}
	}
		
	if ( !boost::filesystem::exists( IV_name ) )
	{
		ivFile.open( IV_name, std::fstream::out);
		if ( !ivFile.is_open() )
		{
			std::cout << "File not opened!" << std::endl;
			return;
		}
		ivFile1.open( IV_name1, std::fstream::out);
		if ( !ivFile1.is_open() )
		{
			std::cout << "File not opened!" << std::endl;
			return;
		}
	}
	ivFile<<"Run started at:"<<endl<<asctime(newtime);
	ivFile<<"Voltage [V]   Current [A]    Stepsize[V]:\t"<<setprecision(3)<<scientific<<fScanconfig.V_step<<"\tCompliance[A]: "<<fScanconfig.I_compliance<<endl;
	
	ivFile1<<"Run started at:"<<endl<<asctime(newtime);
	ivFile1<<"Voltage [V]   Current [A]    Stepsize[V]:\t"<<setprecision(3)<<scientific<<fScanconfig.V_step<<"\tCompliance[A]: "<<fScanconfig.I_compliance<<endl;
}
void SCAN::Scan::Tokenizer(vector< string >& datavec, string& datastring, boost::char_separator<char> sep)
{
	datavec.clear();
	boost::tokenizer< boost::char_separator<char> > tok(datastring, sep);
	for(boost::tokenizer< boost::char_separator<char> >::iterator beg = tok.begin(); beg != tok.end(); ++beg)
	{
		datavec.push_back(*beg);	
// 		cout<<*beg<<endl;
	}
}
void SCAN::Scan::ReadSetConfigFile(int Vcth, string configfilename)
{
	int rangecheck=(Vcth<=255 && 0<=Vcth)?0:1;
	std::string line;
	vector<std::string> content;
	int cRunNumber;
	if ( boost::filesystem::exists( configfilename ) )
	{
		cbc1config.open( configfilename, std::fstream::out | std::fstream::in );
		if ( cbc1config.is_open() )
		{
			while (! cbc1config.eof() )
			{
				getline (cbc1config,line);
				if(line.find("VCth") != std::string::npos)
				{
					vector<string> tokenized_strings;
					this->Tokenizer(tokenized_strings,line,boost::char_separator<char>("\t"));
					if(rangecheck==0)
						tokenized_strings.back()=IntToHexStr(Vcth);
					else{
						cout<<"out of range"<<endl;return;}
					string result;
					for(auto it=tokenized_strings.begin(); it!=(tokenized_strings.end()-1); ++it)
					{
						result+=(*it+"\t");
					}result+=tokenized_strings.back();					
					content.push_back(result);
				}else
					content.push_back(line);
			}
			cbc1config.clear( );
			cbc1config.seekp( 0 );
			for(auto it=content.begin(); it!=(content.end()-1);++it)
			{
				cbc1config<<*it<<endl;
			}
			cbc1config<<content.back();			
			cbc1config.close();
		}
		else
			std::cout << "File not opened!" << std::endl;
	}
}
string SCAN::Scan::IntToHexStr(int x)
{
	std::stringstream stream;
	if(x<16)
		stream <<"0x0";
	else
		stream <<"0x";
	stream.setf ( std::ios::hex, std::ios::basefield );  // set hex as the basefield
	stream.setf ( std::ios::uppercase );
	stream<<x;
	return std::string( stream.str() );
}
void SCAN::Scan::preramp(bool up, KEITHLEY2410::Keithley2410&  k,KEITHLEY2410::Keithley2410&  k1)
{
  int rampdir= up ? 1 : -1;
  int no_of_steps=20;
  double V_stepsize, V_start;
  
  V_stepsize=fScanconfig.V_min/(double)no_of_steps;
  V_start=up? 0: fScanconfig.V_min;
  if(fScanconfig.V_min<0.00001 && fScanconfig.V_min>-0.00001)
	  no_of_steps=1;
  for(int i=0; i<no_of_steps; i++)
  {
	  std::this_thread::sleep_for(std::chrono::milliseconds(500));
	  double V=V_stepsize*(i+1)*rampdir+V_start;
	  k.SourVoltLev(to_string(V));
	  // k1.SourVoltLev(to_string(V));
  }
// 	this->FileGenerator();

}
