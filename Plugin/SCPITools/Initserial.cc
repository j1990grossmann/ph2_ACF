#include "Initserial.h"
using namespace INITSERIAL;
void Serial::ParseSettingsXML(const string& pFilename, ostream& os)
{
    SerialSettingsMap tmpSerialSettingsMap;
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
            if(attr=nSetting.attribute( "rs232" )){
                tmpSerialSettingsMap[nSetting.attribute( "rs232" ).value()]=nSetting.first_child().value();
                cout << RED << "Setting" << RESET << " --" << BOLDCYAN << nSetting.attribute( "rs232" ).value() << RESET << ":" << BOLDYELLOW << nSetting.first_child().value()  << RESET << std:: endl;
            }
        }
    }
    gSerialSettingsmap=tmpSerialSettingsMap;
}
void Serial::Initialise(SerialSettingsMap& fSerialSettingMap)
{
	map <string,boost::asio::serial_port_base::parity::type > paritymap;
	map <string,boost::asio::serial_port_base::flow_control::type > flow_controlmap;
	map <string,boost::asio::serial_port_base::stop_bits::type > stop_bitsmap;
	
	paritymap["none"]=boost::asio::serial_port_base::parity::none;
	paritymap["odd"]=boost::asio::serial_port_base::parity::odd;
	paritymap["even"]=boost::asio::serial_port_base::parity::even;
	
	flow_controlmap["none"]=boost::asio::serial_port_base::flow_control::none;
	flow_controlmap["software"]=boost::asio::serial_port_base::flow_control::software;
	flow_controlmap["hardware"]=boost::asio::serial_port_base::flow_control::hardware;

	stop_bitsmap["one"]=boost::asio::serial_port_base::stop_bits::one;
	stop_bitsmap["onepointfive"]=boost::asio::serial_port_base::stop_bits::onepointfive;
	stop_bitsmap["two"]=boost::asio::serial_port_base::stop_bits::two;
	string devname = fSerialSettingMap["devname"].empty() ? "/dev/Hameg4040" : fSerialSettingMap["devname"];
	// string devname = fSerialSettingMap["devname"].empty() ? "/dev/Hameg4040" : "/dev/Hameg4040";
	unsigned int baudrate = fSerialSettingMap["baudrate"].empty() ? 115200 : atoi(fSerialSettingMap["baudrate"].c_str());

	boost::asio::serial_port_base::parity            parity( fSerialSettingMap["parity"].empty() ? boost::asio::serial_port_base::parity::none : paritymap[fSerialSettingMap["parity"]]);
	boost::asio::serial_port_base::character_size    character_size = fSerialSettingMap["character_size"].empty() ? (boost::asio::serial_port_base::character_size)(8) : (boost::asio::serial_port_base::character_size)(atoi(fSerialSettingMap["character_size"].c_str()));
	boost::asio::serial_port_base::flow_control      flow_control(fSerialSettingMap["flow_control"].empty() ? boost::asio::serial_port_base::flow_control::none : flow_controlmap[fSerialSettingMap["flow_control"]]);
	boost::asio::serial_port_base::stop_bits         stop_bits(fSerialSettingMap["stop_bits"].empty() ? boost::asio::serial_port_base::stop_bits::one : stop_bitsmap[fSerialSettingMap["stop_bits"]]);
	endline = "\n";
	try{
		serial->open(devname,baudrate, parity, character_size, flow_control, stop_bits);
	}catch(boost::system::system_error& e){
		std::cout<<"boost error catched serial not open  "<<e.what()<<std::endl;
		exit(1);
	}
}
void Serial::EmptyBuffer()
{
	string read_str="a";
	while(!read_str.empty())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
		read_str=serial->readStringUntil();	
	}	
}
