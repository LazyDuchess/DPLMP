#pragma once
#include <RakNetTypes.h>
#include <map>
#include "NetworkedCharacter.h"

class CharacterController {
public:
	CharacterController();
	static CharacterController* GetInstance();
	void Step();
	void FrameStep();
	void HandlePacket(RakNet::Packet* packet);
	void OnDisconnect();
	void SendEnterVehiclePacket(unsigned int uid, unsigned int vehicleId, int seat, bool toDriverSeat);
	NetworkedCharacter* GetLocalCharacter();
private:
	std::map<unsigned int, NetworkedCharacter*> _charByIndex;
	static CharacterController* _instance;
};