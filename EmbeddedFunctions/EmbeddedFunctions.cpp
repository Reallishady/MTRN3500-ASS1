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
String^ EmbeddedFunctions::GCommand(String^ command)
{
    String^ response = "";

    if (command == nullptr || command->Length == 0) {
        throw gcnew ArgumentNullException("Command is empty");
    }

    if (client == nullptr || !client->Connected) {
        throw gcnew InvalidOperationException("Connection is not open.");
    }

    if (GalilStream == nullptr) {
        throw gcnew InvalidOperationException("Connection is not open.");
    }

    if (!command->EndsWith(";")) {
        command += ";";
    }

    array<uint8_t>^ sendData = Encoding::ASCII->GetBytes(command);

    try
    {
        GalilStream->Write(sendData, 0, sendData->Length);

        array<uint8_t>^ recvData = gcnew array<uint8_t>(1024);

        while (true)
        {
            int responseBytes = GalilStream->Read(recvData, 0, recvData->Length);
            if (responseBytes == 0)
            {
                throw gcnew Exception("Connection closed by the Galil Controller.");
            }

            response += Encoding::ASCII->GetString(recvData, 0, responseBytes);
            if (response->EndsWith(":") || response->EndsWith("?")) {
                break;
            }
        }

        return response;
    }
    catch (Exception^ e)
    {
        throw gcnew Exception("Error sending command: " + e->Message, e);
    }
}
