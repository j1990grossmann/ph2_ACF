#include "Initserial.h"
// void INITSERIAL::Serial::Initialise(AsyncSerial& serial, INITSERIAL::SerialSettingsMap& fSerialSettingMap)
// {
// // 	for(auto it: fSerialSettingMap)
// // 	{
// // 		cout<<it.first<<"\t"<<it.second<<endl;
// // 	}
// 	map <string,boost::asio::serial_port_base::parity::type > paritymap;
// 	map <string,boost::asio::serial_port_base::flow_control::type > flow_controlmap;
// 	map <string,boost::asio::serial_port_base::stop_bits::type > stop_bitsmap;
// 	
// 	paritymap["none"]=boost::asio::serial_port_base::parity::none;
// 	paritymap["odd"]=boost::asio::serial_port_base::parity::odd;
// 	paritymap["even"]=boost::asio::serial_port_base::parity::even;
// 	
// 	flow_controlmap["none"]=boost::asio::serial_port_base::flow_control::none;
// 	flow_controlmap["software"]=boost::asio::serial_port_base::flow_control::software;
// 	flow_controlmap["hardware"]=boost::asio::serial_port_base::flow_control::hardware;
// 
// 	stop_bitsmap["one"]=boost::asio::serial_port_base::stop_bits::one;
// 	stop_bitsmap["onepointfive"]=boost::asio::serial_port_base::stop_bits::onepointfive;
// 	stop_bitsmap["two"]=boost::asio::serial_port_base::stop_bits::two;
// 	string devname = fSerialSettingMap["devname"].empty() ? "/dev/Hameg4040" : fSerialSettingMap["devname"];
// 	// string devname = fSerialSettingMap["devname"].empty() ? "/dev/Hameg4040" : "/dev/Hameg4040";
// 	unsigned int baudrate = fSerialSettingMap["baudrate"].empty() ? 115200 : atoi(fSerialSettingMap["baudrate"].c_str());
// 
// 	boost::asio::serial_port_base::parity            parity( fSerialSettingMap["parity"].empty() ? boost::asio::serial_port_base::parity::none : paritymap[fSerialSettingMap["parity"]]);
// 	boost::asio::serial_port_base::character_size    character_size = fSerialSettingMap["character_size"].empty() ? (boost::asio::serial_port_base::character_size)(8) : (boost::asio::serial_port_base::character_size)(atoi(fSerialSettingMap["character_size"].c_str()));
// 	boost::asio::serial_port_base::flow_control      flow_control(fSerialSettingMap["flow_control"].empty() ? boost::asio::serial_port_base::flow_control::none : flow_controlmap[fSerialSettingMap["flow_control"]]);
// 	boost::asio::serial_port_base::stop_bits         stop_bits(fSerialSettingMap["stop_bits"].empty() ? boost::asio::serial_port_base::stop_bits::one : stop_bitsmap[fSerialSettingMap["stop_bits"]]);
// 	endline = "\n";
// 	try{
// 		serial.open(devname,baudrate, parity, character_size, flow_control, stop_bits);
// 	}catch(boost::system::system_error& e){
// 		std::cout<<"boost error catched serial not open  "<<e.what()<<std::endl;
// 		exit(1);
// 	}
// }
void INITSERIAL::Serial::Initialise(shared_ptr<BufferedAsyncSerial> serial, INITSERIAL::SerialSettingsMap& fSerialSettingMap)
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
// 		serial=new AsyncSerial();
		serial->open(devname,baudrate, parity, character_size, flow_control, stop_bits);
	}catch(boost::system::system_error& e){
		std::cout<<"boost error catched serial not open  "<<e.what()<<std::endl;
		exit(1);
	}
}