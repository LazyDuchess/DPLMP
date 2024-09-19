#pragma once
#include "RakPeerInterface.h"
#include "TimeController.h"
#include "Client.h"
#include "CarController.h"
#include "CharacterController.h"
#include <map>

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
	void Send(const RakNet::BitStream* bitStream, RakNet::RakNetGUID clientGuid, PacketPriority priority, PacketReliability reliability, char orderingChannel);
	RakNet::RakPeerInterface* Server;
	std::map<RakNet::RakNetGUID, Client*> Clients;
private:
	void HandleClientConnected(RakNet::RakNetGUID guid);
	void HandleClientDisconnected(RakNet::RakNetGUID guid);
	void HandlePackets();
	TimeController* _timeController;
	CarController* _carController;
	CharacterController* _characterController;
};