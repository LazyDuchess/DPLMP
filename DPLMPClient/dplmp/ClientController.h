#pragma once
#include "RakPeerInterface.h"
#include "EventListener.h"
#include "TimeController.h"

class ClientController : public EventListener {
public:
	ClientController();
	void OnPlayerCreated();
	void OnEnterInGameState();
	void OnExitInGameState();
	void Connect();
	void Disconnect();
	void Step();
	RakNet::ConnectionState GetConnectionState();
	RakNet::RakPeerInterface* Client;
	RakNet::SystemAddress ServerAddress;
private:
	TimeController* _timeController;
	void HandlePackets();
};