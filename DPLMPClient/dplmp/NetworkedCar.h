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
	void UpdateTransforms(bool instant);
	CVehicle* Vehicle;
	tVehicleModelUID VehicleModel;
	OwnershipKinds OwnershipKind;
	RakNet::RakNetGUID Owner;
	unsigned int UID;
	vec<float, 3> Position;
	vec<float, 3> Velocity;
	vec<float, 4> AngularVelocity;
	quat<float> Rotation;
	vec<float, 3> Color;
	bool InPingRadius;
	float Steering;
	bool Handbrake;
	float Power;
	float Throttle;
	float RPM;
private:
	bool _wasInPingRadius;
	bool _requestedSpawn;
	void OwnedStep();
	void DoSpawnCar();
	void UpdatePingRadius();
	void OnEnterPingRadius();
	const float PingRadius = 10000.0f;
	const float LerpSpeed = 10.0f;
	const float SlerpSpeed = 10.0f;
};