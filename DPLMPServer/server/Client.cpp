#include "Client.h"

Client::Client(RakNet::RakNetGUID guid) {
	GUID = guid;
	Character = nullptr;
}

void Client::SetCharacter(NetworkedCharacter* character) {
	Character = character;
	character->Owner = GUID;
}