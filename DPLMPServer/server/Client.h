#pragma once
#include "RakPeerInterface.h"

class Client {
public:
	Client(RakNet::RakNetGUID guid);
	RakNet::RakNetGUID GUID;
};