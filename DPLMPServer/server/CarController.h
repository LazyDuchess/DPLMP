#pragma once
#include "RakPeerInterface.h"
#include "UIDProvider.h"
#include "NetworkedCar.h"
#include "Client.h"
#include "map"

class CarController {
public:
	void Initialize();
	void Step();
	void HandleClientConnected(Client* client);
	void HandleClientDisconnected(Client* client);
	void HandlePacket(RakNet::Packet* packet);
	NetworkedCar* CreateVehicle(tVehicleModelUID model, vec<float, 3> position, quat<float> rotation, vec<float, 3> color);
private:
	void BroadcastOwnership(NetworkedCar* car);
	UIDProvider _carUIDProvider;
	std::map<unsigned int, NetworkedCar*> _cars;
	void SpawnTestCars();
	void UpdateCarOwnership(NetworkedCar* car);
};