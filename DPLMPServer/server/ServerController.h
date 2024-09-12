#pragma once
#include "RakPeerInterface.h"
#include "TimeController.h"

class ServerController {
public:
	ServerController();
	static ServerController& GetInstance() {
		static ServerController instance;
		return instance;
	}
	void Connect();
	void Step();
	RakNet::RakPeerInterface* Server;

private:
	void HandlePackets();
	TimeController* _timeController;
};