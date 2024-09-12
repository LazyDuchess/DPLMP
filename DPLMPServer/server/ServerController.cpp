#include "ServerController.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "RakSleep.h"
#include <stdio.h>

ServerController::ServerController() {
	_timeController = new TimeController();
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
}

void ServerController::HandlePackets() {
	RakNet::Packet* packet;
	for (packet = Server->Receive(); packet; Server->DeallocatePacket(packet), packet = Server->Receive())
	{
		if (packet->data[0] == ID_CONNECTION_LOST)
			printf("ID_CONNECTION_LOST from %s\n", packet->systemAddress.ToString());
		else if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION)
			printf("ID_DISCONNECTION_NOTIFICATION from %s\n", packet->systemAddress.ToString());
		else if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			printf("ID_CONNECTION_REQUEST_ACCEPTED from %s\n", packet->systemAddress.ToString());
		else if (packet->data[0] == ID_NEW_INCOMING_CONNECTION)
			printf("ID_NEW_INCOMING_CONNECTION from %s\n", packet->systemAddress.ToString());
	}
}

void ServerController::Step() {
	HandlePackets();
	_timeController->Step();
}

void ServerController::Broadcast(const RakNet::BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel) {
	Server->Send(bitStream, priority, reliability, reliability, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}