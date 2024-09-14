#pragma once
#include "../dpl/CVehicle.h"
#include "../../DPLMPCommon/VehicleModel.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "../../DPLMPCommon/OwnershipKinds.h"
#include <chrono>

typedef std::chrono::steady_clock steady_clock;

class NetworkedCar {
public:
	NetworkedCar();
	~NetworkedCar();
	void ReadFullState(RakNet::BitStream* stream);
	void ReadUpdate(RakNet::BitStream* stream);
	void WriteUpdate(RakNet::BitStream* stream);
	void FrameStep();
	void Step();
	void RequestSpawnCar();
	void RequestOwnership();
	void ReleaseOwnership();
	bool ShouldBeNetworkedByLocalPlayer();
	CVehicle* Vehicle;
	tVehicleModelUID VehicleModel;
	OwnershipKinds OwnershipKind;
	RakNet::RakNetGUID Owner;
	unsigned int UID;
	vec<float, 3> Position;
	vec<float, 3> Velocity;
	quat<float> Rotation;
	vec<float, 3> Color;
private:
	steady_clock::time_point _timeSpawned;
	bool _requestedSpawn;
	void OwnedStep();
	void DoSpawnCar();
	const float NetworkDistance = 300.0;
	const float SpawnNetworkCooldown = 0.25;
};