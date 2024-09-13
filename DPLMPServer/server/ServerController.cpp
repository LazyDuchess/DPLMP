#include "ServerController.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "RakSleep.h"
#include "PLMessageIdentifiers.h"
#include <stdio.h>

ServerController::ServerController() {
	Server = nullptr;
	_timeController = new TimeController();
	_carController = new CarController();
	Clients = std::map<RakNet::RakNetGUID, Client*>();
}

void ServerController::Connect() {
	Server = RakNet::RakPeerInterface::GetInstance();
	printf("Working as server\n");
	int socketFamily;
	socketFamily = AF_INET;
	Server->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	RakNet::SocketDescriptor socketDescriptor(7126, 0);
	socketDescriptor.socketFamily = socketFamily;
	Server->SetMaximumIncomingConnections(4);
	RakNet::StartupResult sr;
	sr = Server->Startup(4, &socketDescriptor, 1);
	if (sr != RakNet::RAKNET_STARTED)
	{
		printf("Server failed to start. Error=%i\n", sr);
		return;
	}
	printf("Started server on %s\n", Server->GetMyBoundAddress().ToString(true));
	_carController->Initialize();
}

void ServerController::HandleClientDisconnected(RakNet::RakNetGUID guid) {
	Client* client = Clients[guid];
	_carController->HandleClientDisconnected(client);
	delete client;
	Clients.erase(guid);
}

void ServerController::HandleClientConnected(RakNet::RakNetGUID guid) {
	Clients.insert(std::pair<RakNet::RakNetGUID, Client*>(guid, new Client(guid)));
	_carController->HandleClientConnected(Clients[guid]);
}

void ServerController::HandlePackets() {
	RakNet::Packet* packet;
	for (packet = Server->Receive(); packet; Server->DeallocatePacket(packet), packet = Server->Receive())
	{
		switch (packet->data[0]) {
		case ID_DISCONNECTION_NOTIFICATION:
		case ID_CONNECTION_LOST:
			printf("Client left from %s\n", packet->systemAddress.ToString());
			HandleClientDisconnected(packet->guid);
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("Client joined from %s\n", packet->systemAddress.ToString());
			HandleClientConnected(packet->guid);
			break;
		case ID_CARCONTROLLER_MAKEMEOWNER:
		case ID_CARCONTROLLER_RELEASEOWNERSHIP:
			_carController->HandlePacket(packet);
			break;
		}
	}
}

void ServerController::Step() {
	HandlePackets();
	_timeController->Step();
}

void ServerController::Send(const RakNet::BitStream* bitStream, RakNet::RakNetGUID clientGuid, PacketPriority priority, PacketReliability reliability, char orderingChannel) {
	Server->Send(bitStream, priority, reliability, reliability, clientGuid, false);
}

void ServerController::Broadcast(const RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel) {
	Server->Send(bitStream, priority, reliability, reliability, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}