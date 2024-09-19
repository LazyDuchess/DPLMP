#pragma once
#include "RakPeerInterface.h"
#include "EventListener.h"
#include "TimeController.h"
#include "CarController.h"
#include "CharacterController.h"
#include <string.h>

class ClientController : public EventListener {
public:
	ClientController();
	void OnPlayerCreated();
	void OnEnterInGameState();
	void OnExitInGameState();
	void Connect();
	void Disconnect();
	void Step();
	void FrameStep();
	void Send(const RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel);
	RakNet::ConnectionState GetConnectionState();
	RakNet::RakPeerInterface* Client;
	RakNet::RakNetGUID MyGUID;
	std::string ServerHost;
	unsigned short ServerPort;
private:
	void HandlePackets();
	RakNet::SystemAddress _serverAddress;
	TimeController* _timeController;
	CarController* _carController;
	CharacterController* _charController;
};