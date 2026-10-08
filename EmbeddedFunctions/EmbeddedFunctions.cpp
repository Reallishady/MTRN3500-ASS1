#include "EmbeddedFunctions.h"
EmbeddedFunctions::EmbeddedFunctions() {
	GalilMngHndl = nullptr;
	GalilStream = nullptr;
}
EmbeddedFunctions::~EmbeddedFunctions() {
	GClose();
}

void EmbeddedFunctions::GOpen(String^ address, const int port) {
	if (GalilMngHndl != nullptr) GClose();
	try {
		address = address->Replace(" -d", "");

		GalilMngHndl = gcnew TcpClient();
		GalilMngHndl->SendTimeout = 500;
		GalilMngHndl->ReceiveTimeout = 500;
		GalilMngHndl->NoDelay = true;

		GalilMngHndl->Connect(address, port);
		GalilStream = GalilMngHndl->GetStream();
	}
	catch (Exception^ e) {  
		GClose();
		throw gcnew Exception("GOpen failed: " + e->Message, e);
	}
}
void EmbeddedFunctions::GClose() {
	
	if (GalilStream != nullptr) GalilStream->Close();
	if (GalilMngHndl != nullptr) GalilMngHndl->Close();
	
	GalilStream = nullptr;
	GalilMngHndl = nullptr;
}
String^ EmbeddedFunctions::GCommand(String^ command) {
	if (command == nullptr || command->Length == 0)
		throw gcnew ArgumentException("Command cannot be empty.");
	if (GalilStream == nullptr)
		throw gcnew InvalidOperationException("GCommand called before GOpen");
	if (!command->EndsWith(";"))
		command += ";";
	
		try
		{
			array<uint8_t>^ sendData =
				Encoding::ASCII->GetBytes(command);

			GalilStream->Write(sendData, 0, sendData->Length);

			array<uint8_t>^ recvData = gcnew array<uint8_t>(2048);
			String^ response = "";
			Threading::Thread::Sleep(10);
			GalilStream->Read(recvData, 0, recvData->Length);
			Threading::Thread::Sleep(10);
			response = Encoding::ASCII->GetString(recvData);

			

			return response;
		}
		catch (Exception^ e)
		{
			throw gcnew Exception(
				"GCommand failed: " + e->Message, e);
		}
}