#include "../framework.h"
#include "ClientController.h"
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include "Core.h"
#include <stdio.h>
#include <mutex>
#include "../dpl/CLifeEnvironment.h"
#include "PLMessageIdentifiers.h"

std::mutex connectionMutex;

ClientController::ClientController() {
	Client = nullptr;
	ServerAddress = nullptr;
	_timeController = new TimeController();
}

void ConnectThread() {
	connectionMutex.lock();
	auto clientController = Core::GetClientController();
	clientController->Client = RakNet::RakPeerInterface::GetInstance();
	clientController->Client->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	clientController->ServerAddress.SetBinaryAddress("127.0.0.1");
	clientController->ServerAddress.SetPortHostOrder(7126);

	RakNet::SocketDescriptor socketDescriptor(0, 0);
	socketDescriptor.socketFamily = AF_INET;
	RakNet::StartupResult sr;
	sr = clientController->Client->Startup(4, &socketDescriptor, 1);
	if (sr != RakNet::RAKNET_STARTED)
	{
		printf("Client failed to start. Error=%i\n", sr);
	}

	printf("Started client on %s\n", clientController->Client->GetMyBoundAddress().ToString(true));

	RakNet::ConnectionAttemptResult result = clientController->Client->Connect("127.0.0.1", 7126, 0, 0);
	while (clientController->GetConnectionState() == RakNet::ConnectionState::IS_CONNECTING || clientController->GetConnectionState() == RakNet::ConnectionState::IS_PENDING)
	{
		RakSleep(100);
	}
	printf("Connection state: %i\n", clientController->GetConnectionState());
	connectionMutex.unlock();
}

void ClientController::Connect() {
	auto threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectThread, 0, 0, 0);
}

void ClientController::Disconnect() {
	connectionMutex.lock();
	if (Client == nullptr)
		return;
	Client->CloseConnection(ServerAddress, true);
	connectionMutex.unlock();
}

RakNet::ConnectionState ClientController::GetConnectionState() {
	if (Client == nullptr)
		return RakNet::IS_NOT_CONNECTED;
	return Client->GetConnectionState(ServerAddress);
}

void ClientController::OnPlayerCreated() {
	printf("Player Created!\n");
}

void ClientController::OnEnterInGameState() {
	printf("Now In-Game!\n");
	Connect();
}

void ClientController::OnExitInGameState() {
	printf("No longer In-Game!\n");
	Disconnect();
}

void ClientController::HandlePackets() {
	RakNet::Packet* packet;
	for (packet = Client->Receive(); packet; Client->DeallocatePacket(packet), packet = Client->Receive())
	{
		if (packet->data[0] == ID_TIMEOFDAY)
			_timeController->HandlePacket(packet);
	}
}

void ClientController::Step() {
	HandlePackets();
	if (!Core::InGame) return;
	auto env = CLifeEnvironment::GetInstance();
	if (env == nullptr) return;
	printf("Current time: %f\n", env->LifeTime);
}