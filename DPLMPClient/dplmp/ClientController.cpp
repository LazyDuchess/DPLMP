#include "../framework.h"
#include "ClientController.h"
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include <stdio.h>

RakNet::RakPeerInterface* client;

void ConnectThread() {
	const char* address = "127.0.0.1";
	client = RakNet::RakPeerInterface::GetInstance();
	client->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	RakNet::SocketDescriptor socketDescriptor(0, 0);
	socketDescriptor.socketFamily = AF_INET;
	RakNet::StartupResult sr;
	sr = client->Startup(4, &socketDescriptor, 1);
	if (sr != RakNet::RAKNET_STARTED)
	{
		printf("Client failed to start. Error=%i\n", sr);
	}

	printf("Started client on %s\n", client->GetMyBoundAddress().ToString(true));

	RakNet::ConnectionAttemptResult result = client->Connect(address, 7126, 0, 0);
	while (result == RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		RakSleep(100);
	}
	printf("Connection result: %i\n", result);
}

void ClientController::Connect() {
	auto threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectThread, 0, 0, 0);
}