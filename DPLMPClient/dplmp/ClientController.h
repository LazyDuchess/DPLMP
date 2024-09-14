#pragma once
#include "RakPeerInterface.h"
#include "EventListener.h"
#include "TimeController.h"
#include "CarController.h"

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
	RakNet::SystemAddress ServerAddress;
	RakNet::RakNetGUID MyGUID;
private:
	void HandlePackets();
	TimeController* _timeController;
	CarController* _carController;
};