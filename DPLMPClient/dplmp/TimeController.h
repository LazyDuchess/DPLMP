#pragma once
#include "RakPeerInterface.h"

class TimeController {
public:
	TimeController();
	void HandlePacket(RakNet::Packet* packet);
	void Step();
private:
	float _timeStep;
};