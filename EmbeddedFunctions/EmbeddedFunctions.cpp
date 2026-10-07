#include "EmbeddedFunctions.h"
EmbeddedFunctions::EmbeddedFunctions() = default;
EmbeddedFunctions::EmbeddedFunctions() { GClose(); }

void EmbeddedFunctions::GOpen(String^ address, const int port) {
	if (Client != nullptr) GClose();
	try {
		Client = gcnew TcpClient(address, port); 
		Client->SendTimeout = 500;
		Client->ReceiveTimeout = 500;
		Client->NoDelay = true;
		Stream = Client->GetStream();
	}
	catch (Exception^ e) {  
		GClose();
		throw gcnew Exception("GOpen failed: " + e->Message, e);
	}
}
void EmbeddedFunctions::GClose() {
	
	if (Stream != nullptr) Stream->Close();
	if (Client != nullptr) Client->Close();
	
	Stream = nullptr;
	Client = nullptr;
}
String^ EmbeddedFunctions::GCommand(String^ command) {
	if (stream == nullptr)
		throw gcnew InvalidOperationException("GCommand called before GOpen");
	try {
		array<uint_8> SendData = Encoding::ASCII->GetBytes(command + "\r");
		stream->Write(SendData, 0, SendData->Length);
		array<Byte>^ RecvData = gcnew array<Byte>(2048);
		String^ response = "";
	}
}