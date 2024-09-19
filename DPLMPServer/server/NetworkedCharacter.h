#pragma once
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "UIDProvider.h"
#include "../DPLMPCommon/PLMath.h"

class NetworkedCharacter {
public:
	NetworkedCharacter(int model, vec<float, 3> position, quat<float> rotation, UIDProvider* uidProvider);
	void WriteFullState(RakNet::BitStream* stream);
	void WriteUpdate(RakNet::BitStream* stream);
	void ReadUpdate(RakNet::BitStream* stream);
	int Model;
	RakNet::RakNetGUID Owner;
	unsigned int UID;
	vec<float, 3> Position;
	quat<float> Rotation;
	bool Player;
	unsigned int CarUID;
	int CarSeat;
	float Health;
private:
	UIDProvider* _uidProvider;
};