#pragma once
#include "map"
#include "NetworkedCar.h"

class CarController {
public:
	CarController();
	static CarController* GetInstance();
	void Step();
	void FrameStep();
	void HandlePacket(RakNet::Packet* packet);
	void OnDisconnect();
	void UpdateAllTransforms();
	NetworkedCar* GetCarForVehicle(CVehicle* vehicle);
	std::map<unsigned int, NetworkedCar*> Cars;
private:
	static CarController* _instance;
};