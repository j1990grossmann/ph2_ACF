#include "HV_control.h"
#include <boost/concept_check.hpp>

void HV_CONTROL::Controller::ParseSettingsXML(const string& pFilename, ostream& os)
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
		pugi::xml_attribute attr;
		for ( pugi::xml_node nSetting = nSettings.child( "Setting" ); nSetting; nSetting = nSetting.next_sibling() )
		{
			if(attr=nSetting.attribute( "rs232_file" )){
				if((string)nSetting.attribute( "rs232_file" ).value()=="Keithley")
				{
					fHV_control.SerialFileKeithley=nSetting.first_child().value();
				}
				if((string)nSetting.attribute( "rs232_file" ).value()=="Keithley1")
				{
					fHV_control.SerialFileKeithley1=nSetting.first_child().value();
				}
				cout << RED << "Setting" << RESET << " --" << BOLDCYAN << nSetting.attribute( "rs232_file" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
			}
		}
		for ( pugi::xml_node nSetting = nSettings.child( "Voltages" ).child("Voltage"); nSetting; nSetting = nSetting.next_sibling())
		{				
			if((string)nSetting.attribute("Id").as_string()=="V_1"){
				cout << RED << "V_1" << RESET << " --" << BOLDCYAN << nSetting.attribute( "V_1").value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fHV_control.V_1=atof(nSetting.first_child().value());
			}
			if((string)nSetting.attribute("Id").as_string()=="V_2"){
				cout << RED << "V_2" << RESET << " --" << BOLDCYAN << nSetting.attribute( "V_2" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fHV_control.V_2=atof(nSetting.first_child().value());
			}
			if((string)nSetting.attribute("Id").as_string()=="step"){
				cout << RED << "step" << RESET << " --" << BOLDCYAN << nSetting.attribute( "step" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fHV_control.Steps=atoi(nSetting.first_child().value());
			}
			// 			cout << RED << "Voltage_min" << RESET << " --" << BOLDCYAN << nSetting.attribute("Id").as_string() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
		}
		for ( pugi::xml_node nSetting = nSettings.child( "CurrentsMax" ).child("CurrentMax"); nSetting; nSetting = nSetting.next_sibling() )
		{	
			if((string)nSetting.attribute("Id").as_string()=="I_max_1"){
				cout << RED << "I_max_1" << RESET << " --" << BOLDCYAN << nSetting.attribute( "I_max_1").value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fHV_control.I_compliance1=atof(nSetting.first_child().value());
			}
			if((string)nSetting.attribute("Id").as_string()=="I_max_2"){
				cout << RED << "I_max_2" << RESET << " --" << BOLDCYAN << nSetting.attribute( "I_max_2").value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
				fHV_control.I_compliance2=atof(nSetting.first_child().value());
			}
		}
		for ( pugi::xml_node nSetting = nSettings.child( "Wait" ).child("Setting"); nSetting; nSetting = nSetting.next_sibling() )
		{				
			if(attr=nSetting.attribute( "Wait" )){
				if((string)nSetting.attribute( "Wait" ).value()=="Wait_ms" )
					fHV_control.Wait_ms=atof(nSetting.first_child().value());
				if((string)nSetting.attribute( "Wait" ).value()=="Wait_ms_ramp" )
					fHV_control.Wait_ms_ramp=atof(nSetting.first_child().value());
				cout << RED << "Vcth" << RESET << " --" << BOLDCYAN << (string)nSetting.attribute( "Vcth" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
			}
		}
		std::cout<<"parsed file"<<std::endl;
		printf("%d %e %e %e %e %d %d\n", fHV_control.Steps, fHV_control.V_1, fHV_control.V_2, fHV_control.I_compliance1, fHV_control.I_compliance2, fHV_control.Wait_ms, fHV_control.Wait_ms_ramp);
		// 		printf("%s %s\n", fScanconfig.SerialFileKeithley.c_str(), fScanconfig.SerialFileHameg.c_str());
	}
}

void HV_CONTROL::Controller::ramp(bool up, volatile sig_atomic_t& stop)
{
	char linestring[10000];
	
	int V_dir= up ? 1 : -1;
	int no_of_steps=fHV_control.Steps;
	double V_stepsize_1, V_stepsize_2, V_start_1, V_start_2;
	
	KEITHLEY2410::Keithley2410  k(fHV_control.SerialFileKeithley);
// 	KEITHLEY2410::Keithley2410  k1(fHV_control.SerialFileKeithley1);
	
	if(up)
	{
		V_stepsize_1=fHV_control.V_1/(double)no_of_steps;
		V_stepsize_2=fHV_control.V_2/(double)no_of_steps;
		V_start_1=0;
		V_start_2=0;
		
		k.Configure();
		k.SenseCurrProt(to_string(fHV_control.I_compliance1));
		k.Outp(1);
		
// 		k1.Configure();
// 		k1.SenseCurrProt(to_string(fHV_control.I_compliance2));
// 		k1.Outp(1);
	}else
	{
		string readstr, readstr1;
		
		k.Read(readstr);
// 		k1.Read(readstr1);
		
		k.Read(readstr);
// 		k1.Read(readstr1);

		Tokenizer(datavec, readstr,boost::char_separator<char>(","));
// 		Tokenizer(datavec1, readstr1,boost::char_separator<char>(","));
		V_start_1=atof(datavec.at(0).c_str());
// 		V_start_2=atof(datavec1.at(0).c_str());
		// cout<<"startvalues rampdown"<<atof(datavec.at(0).c_str())<<"\t"<<atof(datavec.at(1).c_str())<<endl;
		cout<<"startvalues rampdown"<<datavec.at(0).c_str()<<"\t"<<datavec.at(1).c_str()<<endl;
		cout<<"readsrt"<<readstr<<endl;
// 		cout<<readstr1<<endl;
		V_stepsize_1=V_start_1/(double)no_of_steps;
// 		V_stepsize_2=V_start_2/(double)no_of_steps;
	}
	
	this->FileGenerator();
	
	string readstr;
	string readstr1("0,0");
	
	// 	pugi::xml_node node = cFile.append_child();
	// 	node.set_name("Noise_VS_Bias");
	
	// 	cout<<readstr<<endl;
	
	// Now rampup/rampdown
	for(int i=0; i<no_of_steps; i++)
	{
		double V=V_start_1+V_stepsize_1*(i+1)*V_dir;
		double V1=V_start_2+V_stepsize_2*(i+1)*V_dir;
		k.SourVoltLev(to_string(V));
// 		k1.SourVoltLev(to_string(V));
		
		std::this_thread::sleep_for(std::chrono::milliseconds(fHV_control.Wait_ms_ramp));
		
		// 		pugi::xml_node descr = node.append_child();
		// 		descr.set_name("Measurement");
		// 		descr.append_attribute("V_set") = V;
		
	}
	if(up)
		while(!stop)
		{
			string raw_file_name;
			raw_file_name="IV_Curve";
			
			k.Read(readstr);
// 			k1.Read(readstr1);
			Tokenizer(datavec, readstr,boost::char_separator<char>(","));
// 			Tokenizer(datavec1, readstr1,boost::char_separator<char>(","));
			
			std::chrono::time_point<std::chrono::system_clock> start, end;
			// Start the run here
			start = std::chrono::system_clock::now();
			
			std::this_thread::sleep_for(std::chrono::milliseconds(fHV_control.Wait_ms));
			
			end = std::chrono::system_clock::now();
			
			std::time_t end_time = std::chrono::system_clock::to_time_t(end);
			std::time_t start_time = std::chrono::system_clock::to_time_t(start);
			
			struct tm *end_time_tm;
			struct tm *start_time_tm;
			
			int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();
			double Rate;
			if(elapsed_milliseconds!=0)
				Rate=(1000)/(elapsed_milliseconds);
			else
				Rate=66666666666;
			end_time_tm = localtime(&end_time);
			start_time_tm = localtime(&start_time);char buffer [80];char buffer1 [80];
			strftime (buffer,80,"%FT%TZ",start_time_tm);
			strftime (buffer1,80,"%FT%TZ",end_time_tm);
			
			printf("Time %s V1 %s V2 %s I1 %s I2 %s \n",buffer1, datavec.at(0).c_str(), datavec.at(1).c_str(), datavec1.at(0).c_str(),datavec1.at(1).c_str());
			ivFile<<buffer1<<"\t"<<datavec.at(0)<<"\t"<<datavec.at(1)<<endl;
			ivFile1<<buffer1<<"\t"<<datavec1.at(0)<<"\t"<<datavec1.at(1)<<endl;
			// 				pugi::xml_node param = descr.append_child();
			// 				param.set_name("Vcth");
			// 				param.append_attribute("Filename");
			// 				param.attribute("Filename").xml_attribute::set_value(raw_file_name.c_str());
			// 				param.append_attribute("Vcth");
			// 				param.attribute("Vcth").xml_attribute::set_value(Vcth);
			// 				param.append_attribute("V");
			// 				param.attribute("V").set_value(datavec.at(0).c_str());
			// 				param.append_attribute("I");
			// 				param.attribute("I").set_value(datavec.at(1).c_str());
			// 				param.append_attribute("V1");
			// 				param.attribute("V1").set_value(datavec1.at(0).c_str());
			// 				param.append_attribute("I1");
			// 				param.attribute("I1").set_value(datavec1.at(1).c_str());
			// 				param.append_attribute("Nevents");
			// 				param.attribute("Nevents").set_value(fScanconfig.No_events);
			// 				param.append_attribute("Start");
			// 				param.attribute("Start").set_value(buffer);
			// 				param.append_attribute("Stop");
			// 				param.attribute("Stop").set_value(buffer1);
			// 				param.append_attribute("Rate_Hz");
			// 				param.attribute("Rate_Hz").set_value(Rate);
			// 				param.append_attribute("Position_x");
			// 				param.attribute("Position_x").set_value(cPosX.c_str());
			// 				param.append_attribute("Position_z");
			// 				param.attribute("Position_z").set_value(cPosZ.c_str());
			// 				param.append_attribute("Angle");
			// 				param.attribute("Angle").set_value(cAngle.c_str());
			// 				cFile.save_file(Data_name.c_str());
		}
		if(stop)
		{
			cout<<"Safely exit the programm"<<endl;	
		}
		if(!up)
		{
			k.SourVoltLev("0");
// 			k1.SourVoltLev("0");
			std::this_thread::sleep_for(std::chrono::seconds(1));
			k.Read(readstr);
			k.Outp(0);
			
// 			k1.Read(readstr);
// 			k1.Outp(0);
		}	
}
void HV_CONTROL::Controller::FileGenerator()
{
	
	RUNCOUNTER::Runcounter runcounter;string rundstring(runcounter.GetFileName());
	fOutFilename.append(rundstring.c_str());
	string dirname=	fOutFilename;
	string IV_name =fOutFilename+"/"+rundstring+"_iv.txt";
	string IV_name1 =fOutFilename+"/"+rundstring+"_iv1.txt";
	Data_name=fOutFilename+"/"+rundstring+".xml";
	
	boost::filesystem::create_directory(dirname);
	// 	 	printf("%s\n",fOutFilename.c_str());
	printf("%s\n",dirname.c_str());
	// 	 	printf("%s\n",Data_name.c_str());
	// 	 	printf("%s\n",IV_name.c_str());
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
	time_t ltime;struct tm *newtime;
	time(&ltime);  newtime = localtime(&ltime);	
	ivFile<<"Run started at:"<<endl<<asctime(newtime);
	ivFile<<"Time Voltage [V]   Current [A]    Stepsize[V]:\t"<<setprecision(3)<<scientific<<"\tCompliance[A]: "<<fHV_control.I_compliance1<<endl;
	
// 	ivFile1<<"Run started at:"<<endl<<asctime(newtime);
// 	ivFile1<<"Time Voltage [V]   Current [A]    Stepsize[V]:\t"<<setprecision(3)<<scientific<<"\tCompliance[A]: "<<fHV_control.I_compliance2<<endl;
}
void HV_CONTROL::Controller::Tokenizer(vector< string >& datavec, string& datastring, boost::char_separator< char > sep)
{
	datavec.clear();
	boost::tokenizer< boost::char_separator<char> > tok(datastring, sep);
	for(boost::tokenizer< boost::char_separator<char> >::iterator beg = tok.begin(); beg != tok.end(); ++beg)
	{
		datavec.push_back(*beg);	
		// 		cout<<*beg<<endl;
	}
}

string HV_CONTROL::Controller::IntToHexStr(int x)
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

