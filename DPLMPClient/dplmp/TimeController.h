#pragma once
#include "RakPeerInterface.h"

class TimeController {
public:
	TimeController();
	void HandlePacket(RakNet::Packet* packet);
	void FrameStep();
private:
	float _timeStep;
	float _currentHour;
};