#include "NetworkedCar.h"
#include "Core.h"
#include "../dpl/SpoolableResourceManager.h"
#include "../dpl/CVehicleManager.h"
#include "../dpl/CLifeSystem.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "../dpl/LifeAcquirableVehicleManager.h"
#include "../dpl/CHandling.h"


NetworkedCar::NetworkedCar() {
	Vehicle = nullptr;
	VehicleModel = tVehicleModelUID_Andec;
	Owner = RakNet::UNASSIGNED_RAKNET_GUID;
	OwnershipKind = OwnershipKinds::Normal;
	UID = 0;
	Position = { 0,0,0 };
	Rotation = { 0,0,0,0 };
	Color = { 1,1,1 };
	Velocity = { 0,0,0 };
	AngularVelocity = { 0,0,0,0 };
	_requestedSpawn = false;
	InPingRadius = false;
	_wasInPingRadius = false;
	Steering = 0;
	Handbrake = false;
	Power = 0;
	Throttle = 0;
	RPM = 0;
}

void NetworkedCar::UpdateTransforms(bool instant) {
	CHandling* handling = nullptr;
	Vehicle->GetHandling(&handling);
	PHBaseObj* phys = Vehicle->GetPhysicsObject();
	vec<float, 3> smoothPos = Position;
	quat<float> smoothRot = Rotation;
	if (!instant) {
		smoothPos = Lerp(phys->GetPosition(), Position, Core::DeltaTime * LerpSpeed);
		// this doesn't work great :(
		smoothRot = SLerp180Clamped(phys->GetRotation(), Rotation, Core::DeltaTime * SlerpSpeed);
	}
	vec<float, 4> pos4d = { smoothPos.a[0], smoothPos.a[1], smoothPos.a[2], 1.0 };
	phys->SetPositionAndOrientation(&pos4d, &smoothRot, InPingRadius);
	phys->SetVelocity(Velocity);
	phys->SetAngularVelocity(&AngularVelocity);
	handling->SetSteering(Steering);
	handling->SetHandbraking(Handbrake);
	handling->SetPower(Power);
	handling->SetThrottle(Throttle);
	handling->SetRPM(RPM);
}

void NetworkedCar::ReadFullState(RakNet::BitStream* stream) {
	stream->Read(VehicleModel);
	ReadUpdate(stream);
	stream->Read(Owner);
	stream->Read(OwnershipKind);
	stream->Read(Color);
}

void NetworkedCar::WriteUpdate(RakNet::BitStream* stream) {
	stream->Write(Position);
	stream->Write(Velocity);
	stream->Write(AngularVelocity);
	stream->Write(Rotation);
	stream->Write(Steering);
	stream->Write(Handbrake);
	stream->Write(Power);
	stream->Write(Throttle);
	stream->Write(RPM);
}

void NetworkedCar::ReadUpdate(RakNet::BitStream* stream) {
	stream->Read(Position);
	stream->Read(Velocity);
	stream->Read(AngularVelocity);
	stream->Read(Rotation);
	stream->Read(Steering);
	stream->Read(Handbrake);
	stream->Read(Power);
	stream->Read(Throttle);
	stream->Read(RPM);
}

void NetworkedCar::FrameStep() {
	if (Vehicle == nullptr) return;
	ClientController* client = Core::GetClientController();
	if (ShouldBeNetworkedByLocalPlayer()) return;
	UpdateTransforms(false);
}

void NetworkedCar::OwnedStep() {
	if (Vehicle == nullptr) return;
	CHandling* handling = nullptr;
	Vehicle->GetHandling(&handling);
	PHBaseObj* phys = Vehicle->GetPhysicsObject();
	mat<float,4,4>* matrix = phys->GetMatrix();
	Position = GetPosition(matrix);
	Rotation = phys->GetRotation();
	Velocity = phys->GetVelocity();
	phys->GetAngularVelocity(&AngularVelocity);
	Steering = handling->GetSteering();
	Handbrake = handling->GetHandbraking();
	Power = handling->GetPower();
	Throttle = handling->GetThrottle();
	RPM = handling->GetRPM();
	RakNet::BitStream bs;
	bs.Write((unsigned char)ID_CARCONTROLLER_UPDATE);
	bs.Write(UID);
	WriteUpdate(&bs);
	Core::GetClientController()->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

void NetworkedCar::DoSpawnCar() {
	CVehicleManager* vehicleManager = CVehicleManager::GetInstance();
	_requestedSpawn = false;
	CVehicle* veh;
	CVehicle** result = vehicleManager->CreateVehicle(&veh, VehicleModel, Position, Rotation.a[1], false, true, true);
	Vehicle = *result;
	Vehicle->SetColor(Color);
	LifeAcquirableVehicleManager::GetInstance()->AddVehicle(Vehicle, 1);
	UpdateTransforms(true);
}

void NetworkedCar::Step() {
	UpdatePingRadius();

	if (_requestedSpawn) {
		SpoolableResourceManager* resourceManager = SpoolableResourceManager::GetInstance();
		if (resourceManager->IsEntityLoaded(SpooledPackageType::Vehicles, VehicleModel)) {
			DoSpawnCar();
		}
	}
	if (Vehicle == nullptr) return;

	if (!InPingRadius)
		UpdateTransforms(true);

	ClientController* client = Core::GetClientController();
	CCharacter* myCharacter = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	CVehicle* myVehicle = myCharacter->GetVehicle();

	if (myVehicle == Vehicle && OwnershipKind != OwnershipKinds::Driving && myCharacter->GetCarSeat() == 0)
		RequestOwnership();
	else if ((myVehicle != Vehicle || myCharacter->GetCarSeat() != 0) && OwnershipKind == OwnershipKinds::Driving && Owner == client->MyGUID)
		ReleaseOwnership();

	if (ShouldBeNetworkedByLocalPlayer() && InPingRadius)
		OwnedStep();
}

void NetworkedCar::RequestSpawnCar() {
	if (_requestedSpawn) return;
	if (Vehicle != nullptr) return;
	_requestedSpawn = true;
	SpoolableResourceManager* resourceManager = SpoolableResourceManager::GetInstance();
	if (resourceManager->IsEntityLoaded(SpooledPackageType::Vehicles, VehicleModel))
		DoSpawnCar();
	else
		resourceManager->SetEntityPriority(SpooledPackageType::Vehicles, VehicleModel, SpoolPriority::Request);
}

void NetworkedCar::RequestOwnership() {
	RakNet::BitStream bs;
	bs.Write((unsigned char)ID_CARCONTROLLER_MAKEMEOWNER);
	bs.Write(UID);
	Core::GetClientController()->Send(&bs, PacketPriority::MEDIUM_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

void NetworkedCar::ReleaseOwnership() {
	RakNet::BitStream bs;
	bs.Write((unsigned char)ID_CARCONTROLLER_RELEASEOWNERSHIP);
	bs.Write(UID);
	Core::GetClientController()->Send(&bs, PacketPriority::MEDIUM_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

NetworkedCar::~NetworkedCar() {
	if (Vehicle != nullptr)
		delete Vehicle;
}

bool NetworkedCar::ShouldBeNetworkedByLocalPlayer() {
	if (Vehicle == nullptr) return false;
	if (Core::GetClientController()->MyGUID == Owner)
		return true;
	else
		return false;
	CCharacter* player = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	if (player->GetVehicle() == Vehicle && player->GetCarSeat() == 0) return true;
	return true;
}

void NetworkedCar::UpdatePingRadius() {
	CCharacter* player = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	if (player == nullptr) {
		InPingRadius = false;
		_wasInPingRadius = false;
		return;
	}
	vec<float, 3> playerPos = GetPosition(player->GetMatrix());
	vec<float, 2> playerPos2d = { playerPos.a[0], playerPos.a[2] };
	vec<float, 2> carPos2d = { Position.a[0], Position.a[2] };
	vec<float, 2> diff = playerPos2d - carPos2d;
	float dist = mag_sqr(diff);
	InPingRadius = dist < PingRadius;
	if (InPingRadius && !_wasInPingRadius)
		OnEnterPingRadius();
	_wasInPingRadius = InPingRadius;
}

void NetworkedCar::OnEnterPingRadius() {
	if (Vehicle == nullptr) return;
	Vehicle->GetPhysicsObject()->WakeUp();
}