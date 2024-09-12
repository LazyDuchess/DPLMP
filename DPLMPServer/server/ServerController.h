#pragma once
#include "RakPeerInterface.h"

class ServerController {
public:
	static ServerController& GetInstance() {
		static ServerController instance;
		return instance;
	}
	void Connect();
	void Step();
	RakNet::RakPeerInterface* Server;

private:
	void HandlePackets();
};