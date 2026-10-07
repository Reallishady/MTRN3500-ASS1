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
		address = address->Trim();
		if (address->EndsWith("-d"))
			address = address->Substring(0, address->Length - 2)->Trim();

		GalilMngHndl = gcnew TcpClient(address, port); 
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

		command += "\r";

		try
		{
			array<Byte>^ sendData =
				Encoding::ASCII->GetBytes(command);

			GalilStream->Write(sendData, 0, sendData->Length);
			GalilStream->Flush();

			array<Byte>^ recvData = gcnew array<Byte>(2048);
			String^ response = "";

			while (true)
			{
				int count = GalilStream->Read(
					recvData, 0, recvData->Length);

				if (count == 0)
					throw gcnew Exception(
						"Connection closed by the controller.");

				response += Encoding::ASCII->GetString(
					recvData, 0, count);

				if (response->Contains(":") ||
					response->Contains("?"))
					break;
			}

			return response;
		}
		catch (Exception^ e)
		{
			throw gcnew Exception(
				"GCommand failed: " + e->Message, e);
		}
}