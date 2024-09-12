#pragma once
#include "RakPeerInterface.h"

class TimeController {
public:
	void HandlePacket(RakNet::Packet* packet);
};