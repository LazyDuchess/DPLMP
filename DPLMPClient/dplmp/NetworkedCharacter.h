#pragma once
#include "../dpl/CCharacter.h"
#include "BitStream.h"
#include "RakPeerInterface.h"

class NetworkedCharacter {
public:
	NetworkedCharacter();
	~NetworkedCharacter();
	void ReadFullState(RakNet::BitStream* stream);
	void ReadUpdate(RakNet::BitStream* stream);
	void WriteUpdate(RakNet::BitStream* stream);
	void FrameStep();
	void Step();
	void Spawn();
	int Model;
	CCharacter* Character;
	RakNet::RakNetGUID Owner;
	unsigned int UID;
	vec<float, 3> Position;
	quat<float> Rotation;
	bool Player;
private:
	const float LerpSpeed = 10.0f;
	const float SlerpSpeed = 10.0f;
};