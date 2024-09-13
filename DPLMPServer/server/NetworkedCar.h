#pragma once
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "UIDProvider.h"
#include "VehicleModel.h"
#include "PLMath.h"

class NetworkedCar {
public:
	NetworkedCar(tVehicleModelUID model, vec<float, 3> position, quat<float> rotation, UIDProvider* uidProvider, float r, float g, float b);
	void WriteFullState(RakNet::BitStream* stream);
	void WriteUpdate(RakNet::BitStream* stream);
	void ReadUpdate(RakNet::BitStream* stream);
	tVehicleModelUID VehicleModel;
	RakNet::RakNetGUID Owner;
	unsigned int UID;
	vec<float, 3> Position;
	quat<float> Rotation;
	float Color[3];
private:
	UIDProvider* _uidProvider;
};