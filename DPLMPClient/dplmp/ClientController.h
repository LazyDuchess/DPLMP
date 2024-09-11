#pragma once
#include "RakPeerInterface.h"

class ClientController {
public:
	static ClientController& GetInstance() {
		static ClientController instance;
		return instance;
	}
	void Connect();
	void GameJoined();
	void Step();
	RakNet::RakPeerInterface* Client;
};