#include "Galil.h"

Galil::Galil() {

	Functions = new EmbeddedFunctions();
	g = GCon();
	Functions->GOpen("192.168.0.120 -d", &g);
	ControlParameters[0] = 0.0; 
	ControlParameters[1] = 0.0; 
	ControlParameters[2] = 0.0; 
	setPoint = 0;
	lastReturn = G_NO_ERROR;
	lastResponse.clear();
}
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
	Functions = Funcs;
	g = GCon();
	Functions->GOpen(address, &g);
	ControlParameters[0] = 0.0;
	ControlParameters[1] = 0.0;
	ControlParameters[2] = 0.0;
	setPoint = 0;
	lastReturn = G_NO_ERROR;
	lastResponse.clear();
}
Galil::Galil(const Galil& other) {
	Functions = new EmbeddedFunctions();
	g = GCon();
	Functions->GOpen("192.168.0.120 -d", &g);
	ControlParameters[0] = other.ControlParameters[0];
	ControlParameters[1] = other.ControlParameters[1];
	ControlParameters[2] = other.ControlParameters[2];
	setPoint = other.setPoint;
	lastReturn = G_NO_ERROR;
	lastResponse.clear();
}
Galil::~Galil() {
	Functions->GClose(g);
	g = 0;
	delete Functions;
}
void Galil::DigitalOutput(uint16_t value) {
	const unsigned low = value & 0xFF;
	const unsigned high = (value >> 8) & 0xFF;
	const std::string cmd = "OP " + std::to_string(low) + "," + std::to_string(high);
	char buf[G_SMALL_BUFFER] = {};
	lastReturn = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	lastResponse = buf;
}
void Galil::DigitalByteOutput(bool bank, uint8_t value) {
	char buf[G_SMALL_BUFFER] = {};
	if (bank) {
		
		const std::string cmd = "OP " +  std::to_string(0) + "," + std::to_string(value);
		lastReturn  = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	}
	else {
		
		const std::string cmd = "OP " + std::to_string(value);;
		lastReturn = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	}
	lastResponse = buf;
}
void Galil::DigitalBitOutput(bool val, uint8_t bit)
{
	std::string cmd = (val ? "SB " : "CB ") + std::to_string(bit);

	char buf[G_SMALL_BUFFER] = {};

	lastReturn = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);

	lastResponse = buf;
}
uint16_t Galil::DigitalInput() {
	uint16_t x = {};
	for (int i = 0; i < 16; i++) {
		if (DigitalBitInput(i)) {
			x |= (1 << i);
		}
	}
	return x;
}


uint8_t Galil::DigitalByteInput(bool bank) {
	uint8_t x = {};
	const int offset = bank ? 8 : 0;   
	for (int i = 0; i < 8; i++) {
		if (DigitalBitInput(offset + i)) {
			x |= (1 << i);            
		}
	}
	return x;
}
bool Galil::DigitalBitInput(uint8_t bit) {
	const std::string cmd = "MG @IN[" + std::to_string(bit) + "]";
	char buf[G_SMALL_BUFFER] = {};
	Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	return std::atoi(buf) == 1;
}
bool Galil::CheckSuccessfulWrite() {
	return lastReturn == G_NO_ERROR && !lastResponse.empty() && lastResponse[0] != '?';
}
float Galil::AnalogInput(uint8_t channel) {
	const std::string cmd = "MG @AN[" + std::to_string(channel) + "]";
	char buf[G_SMALL_BUFFER] = {};
	Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	return static_cast<float>(atof(buf));
	
}
void Galil::AnalogOutput(uint8_t channel, double voltage) {
	const std::string cmd = "AO " + std::to_string(channel) + "," + std::to_string(voltage);
	char buf[G_SMALL_BUFFER] = {};
	lastReturn = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	lastResponse = buf;
}


void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
	const std::string cmd = "AQ " + std::to_string(channel) + "," + std::to_string(range);

	char buf[G_SMALL_BUFFER] = {};

	lastReturn = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	lastResponse = buf;
}
void Galil::WriteEncoder() {
	const std::string cmd = "WE " + std::to_string(0);
	char buf[G_SMALL_BUFFER] = {};

	lastReturn = Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	lastResponse = buf;
}
int Galil::ReadEncoder() {
	const std::string cmd = "QE " + std::to_string(0);
	char buf[G_SMALL_BUFFER] = {};

	Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	return std::atoi(buf);
}
void Galil::setSetPoint(int s) {
	setPoint = s;
}
double Galil::getSetPoint() {

	return setPoint;
}
void Galil::setKp(double gain) {
	ControlParameters[0] = gain;
}
double Galil::getKp() {
	return ControlParameters[0];
}
void Galil::setKi(double gain) {
	ControlParameters[1] = gain;
}
double Galil::getKi() {
	return ControlParameters[1];
}
void Galil::setKd(double gain) {
	ControlParameters[2] = gain;
}
double Galil::getKd() {
	return ControlParameters[2];
}
std::ostream& operator<<(std::ostream& output, Galil& galil) {
	char info[G_SMALL_BUFFER] = {};
	char ver[G_SMALL_BUFFER] = {};

	GReturn req_1 = galil.Functions->GInfo(galil.g, info, sizeof(info));
	GReturn req_2 = galil.Functions->GVersion(ver, sizeof(ver));

	if (req_1 == G_NO_ERROR) output << info << "\n\n";
	else                   output << "GInfo failed (" << req_1 << ")\n\n";

	if (req_2 == G_NO_ERROR) output << ver << "\n\n";
	else                   output << "GVersion failed (" << req_2 << ")\n\n";

	return output;   
}
Galil& Galil::operator=(const Galil& other) {
	if (this == &other) return *this;      
	
	Functions->GClose(g);
	delete Functions;
	
	Functions = new EmbeddedFunctions();
	g = GCon();
	Functions->GOpen("192.168.0.120", &g);

	for (int i = 0; i < 3; i++) ControlParameters[i] = other.ControlParameters[i];
	setPoint = other.setPoint;
	lastReturn = G_NO_ERROR;
	lastResponse.clear();

	return *this;
}