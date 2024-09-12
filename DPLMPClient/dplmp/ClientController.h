#pragma once
#include "RakPeerInterface.h"
#include "EventListener.h"

class ClientController : public EventListener {
public:
	void OnPlayerCreated();
	void OnEnterInGameState();
	void OnExitInGameState();
	void Connect();
	void Disconnect();
	RakNet::ConnectionState GetConnectionState();
	RakNet::RakPeerInterface* Client;
	RakNet::SystemAddress ServerAddress;
};