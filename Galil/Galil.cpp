#include "Galil.h"
Galil::Galil() {

	Functions = new EmbeddedFunctions();
	g = GCon();
	Functions->GOpen("192.168.0.120", &g);	
	ControlParameters[0] = 0.0; 
	ControlParameters[1] = 0.0; 
	ControlParameters[2] = 0.0; 
	setPoint = 0;
}
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
	Functions = Funcs;
	g = GCon();
	Functions->GOpen(address, &g);
	ControlParameters[0] = 0.0;
	ControlParameters[1] = 0.0;
	ControlParameters[2] = 0.0;
	setPoint = 0;
}
Galil::Galil(const Galil& other) {
	Functions = new EmbeddedFunctions();
	g = GCon();
	Functions->GOpen("192.168.0.120", &g);
	ControlParameters[0] = other.ControlParameters[0];
	ControlParameters[1] = other.ControlParameters[1];
	ControlParameters[2] = other.ControlParameters[2];
	setPoint = other.setPoint;
}
Galil::~Galil() {
	Functions->GClose(g);
	delete Functions;
}
void Galil::DigitalOutput(uint16_t value) {
	const unsigned low = value & 0xFF;
	const unsigned high = (value >> 8) & 0xFF;
	const std::string cmd = "OP " + std::to_string(low) + "," + std::to_string(high);
	char buf[G_] = {};
	Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
}
void Galil::DigitalByteOutput(bool bank, uint8_t value) {
	if (bank) {
		
		
		const std::string cmd = "OP " +  std::to_string(0) + "," + std::to_string(value);
		char buf[G_] = {};
		Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	}
	else {
		
		const std::string cmd = "OP " + std::to_string(value);
		char buf[128] = {};
		Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
	}
}
void Galil::DigitalBitOutput(bool val, uint8_t bit) {
	const std::string cmd = "SB " + std::to_string(bit);
	char buf[128] = {};
	Functions->GCommand(g, cmd.c_str(), buf, sizeof(buf), nullptr);
}
uint16_t Galil::DigitalInput() {
	uint16_t x = {};
	for (int i = 0; i < 16; i++) {
		if (DigitalBitInput(i)) {

		}
	
	
}

uint8_t Galil::DigitalByteInput(bool bank) {
	
}
bool Galil::DigitalBitInput(uint8_t bit) {
	const std::string cmd = "MG @IN[" + std::to_string(bit) + "];";
	char buf[G_SMALL_BUFFER] = {};
	Functins->GCommand(g,cmd.c_str(), buf, sizeof(buf), nullptr);
	return std::atoi(buf) == 1;
}

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
