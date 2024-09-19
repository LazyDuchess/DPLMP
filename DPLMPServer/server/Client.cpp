#include "Client.h"

Client::Client(RakNet::RakNetGUID guid) {
	GUID = guid;
	Character = nullptr;
}