#pragma once
#include "../DPLMPCommon/PLMath.h"
#include "Client.h"
#include "BitStream.h"
#include "NetworkedCharacter.h"
#include "UIDProvider.h"
#include "RakPeerInterface.h"
#include "map"

class CharacterController {
public:
	void HandleClientConnected(Client* client);
	void Step();
	void HandlePacket(RakNet::Packet* packet);
	NetworkedCharacter* CreatePlayerCharacter(Client* client, vec<float, 3> position, quat<float> rotation, int model);
private:
	void BroadcastCharacters();
	UIDProvider _charUIDProvider;
	std::map<unsigned int, NetworkedCharacter*> _characters;
};