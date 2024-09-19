#pragma once
#include "RakPeerInterface.h"
#include "NetworkedCharacter.h"

class Client {
public:
	Client(RakNet::RakNetGUID guid);
	RakNet::RakNetGUID GUID;
	NetworkedCharacter* Character;
};