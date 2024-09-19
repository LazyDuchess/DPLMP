#pragma once
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "UIDProvider.h"
#include "VehicleModel.h"
#include "PLMath.h"
#include "OwnershipKinds.h"

class NetworkedCar {
public:
	NetworkedCar(tVehicleModelUID model, vec<float, 3> position, quat<float> rotation, UIDProvider* uidProvider, vec<float,3> color);
	void WriteFullState(RakNet::BitStream* stream);
	void WriteUpdate(RakNet::BitStream* stream);
	void ReadUpdate(RakNet::BitStream* stream);
	tVehicleModelUID VehicleModel;
	RakNet::RakNetGUID Owner;
	OwnershipKinds OwnershipKind;
	unsigned int UID;
	vec<float, 3> Position;
	vec<float, 3> Velocity;
	quat<float> Rotation;
	vec<float, 3> Color;
	float Steering;
	bool Handbrake;
	float Power;
	float Throttle;
	float RPM;
private:
	UIDProvider* _uidProvider;
};