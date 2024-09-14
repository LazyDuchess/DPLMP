#pragma once
#include "map"
#include "NetworkedCar.h"

class CarController {
public:
	CarController();
	void Step();
	void FrameStep();
	void HandlePacket(RakNet::Packet* packet);
	void OnDisconnect();
private:
	std::map<unsigned int, NetworkedCar*> _cars;
};