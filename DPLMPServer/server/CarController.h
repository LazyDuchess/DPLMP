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
private:
	void BroadcastOwnership(NetworkedCar* car);
	UIDProvider _carUIDProvider;
	std::map<unsigned int, NetworkedCar*> _cars;
};