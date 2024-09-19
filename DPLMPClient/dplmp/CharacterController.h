#pragma once
#include <RakNetTypes.h>
#include <map>
#include "NetworkedCharacter.h"

class CharacterController {
public:
	CharacterController();
	void Step();
	void FrameStep();
	void HandlePacket(RakNet::Packet* packet);
	void OnDisconnect();
private:
	std::map<unsigned int, NetworkedCharacter*> _charByIndex;
};