#include "../framework.h"
#include "ClientController.h"
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include "Core.h"
#include <stdio.h>
#include <mutex>
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "../dpl/CLifeSystem.h"
#include "../dpl/SpoolableResourceManager.h"
#include "../dpl/CLifeEventDataManager.h"
#include "../dpl/CLifeInstances.h"
#include "../../DPLMPCommon/PLMath.h"

std::mutex connectionMutex;

ClientController::ClientController() {
	_timeController = new TimeController();
	Client = nullptr;
	ServerAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
}

void ClientController::Connect() {
	Client = RakNet::RakPeerInterface::GetInstance();
	Client->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	ServerAddress.SetBinaryAddress("127.0.0.1");
	ServerAddress.SetPortHostOrder(7126);

	RakNet::SocketDescriptor socketDescriptor(0, 0);
	socketDescriptor.socketFamily = AF_INET;
	RakNet::StartupResult sr;
	sr = Client->Startup(4, &socketDescriptor, 1);
	if (sr != RakNet::RAKNET_STARTED)
	{
		printf("Client failed to start. Error=%i\n", sr);
	}

	printf("Started client on %s\n", Client->GetMyBoundAddress().ToString(true));

	RakNet::ConnectionAttemptResult result = Client->Connect("127.0.0.1", 7126, 0, 0);
	while (GetConnectionState() == RakNet::ConnectionState::IS_CONNECTING || GetConnectionState() == RakNet::ConnectionState::IS_PENDING)
	{
		RakSleep(100);
	}
	printf("Connection state: %i\n", GetConnectionState());
}

void ClientController::Disconnect() {
	if (Client == nullptr)
		return;
	Client->CloseConnection(ServerAddress, true);
}

RakNet::ConnectionState ClientController::GetConnectionState() {
	if (Client == nullptr)
		return RakNet::IS_NOT_CONNECTED;
	return Client->GetConnectionState(ServerAddress);
}

void ClientController::HandlePackets() {
	if (GetConnectionState() == RakNet::IS_CONNECTED) {
		RakNet::Packet* packet;
		for (packet = Client->Receive(); packet; Client->DeallocatePacket(packet), packet = Client->Receive())
		{
			if (packet->data[0] == ID_TIMEOFDAY)
				_timeController->HandlePacket(packet);
		}
	}
}

void ClientController::Step() {
	HandlePackets();
	_timeController->Step();
	// EXPERIMENTS
	// Crashes when going near a side mission area. Might work if the save has no side missions available?
	/*
	if (((GetAsyncKeyState(VK_NUMPAD1) & 0x8001) == 0x8001))
	{
		CLifeEventDataManager::GetInstance()->EndAllLifeEvents();
	}*/
}

void ClientController::OnPlayerCreated() {
	printf("Player Created!\n");
}

void ClientController::OnEnterInGameState() {
    printf("Now In-Game!\n");
    SpoolableResourceManager::GetInstance()->SetEntityPriority(SpooledPackageType::Characters, 123, SpoolPriority::Request);
    CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->SwapSkin(123, 123);
    Connect();
}

void ClientController::OnExitInGameState() {
	printf("No longer In-Game!\n");
	Disconnect();
}