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