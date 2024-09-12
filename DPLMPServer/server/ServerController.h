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
	void Broadcast(const RakNet::BitStream* bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel);
	RakNet::RakPeerInterface* Server;

private:
	void HandlePackets();
	TimeController* _timeController;
};