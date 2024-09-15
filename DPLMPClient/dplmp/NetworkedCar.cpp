#include "NetworkedCar.h"
#include "Core.h"
#include "../dpl/SpoolableResourceManager.h"
#include "../dpl/CVehicleManager.h"
#include "../dpl/CLifeSystem.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "../dpl/LifeAcquirableVehicleManager.h"


NetworkedCar::NetworkedCar() {
	Vehicle = nullptr;
	VehicleModel = tVehicleModelUID_Andec;
	Owner = RakNet::UNASSIGNED_RAKNET_GUID;
	OwnershipKind = OwnershipKinds::Normal;
	UID = 0;
	Position = { 0,0,0 };
	VisualRotation = { 0,0,0,0 };
	Color = { 1,1,1 };
	Velocity = { 0,0,0 };
	_requestedSpawn = false;
	_stepsSinceSpawn = 0;
}

void NetworkedCar::UpdateTransforms() {
	PHBaseObj* phys = Vehicle->GetPhysicsObject();
	mat<float, 4, 4>* matrix = phys->GetMatrix();
	SetPosition(matrix, Position);
	SetQuaternionRotation(matrix, VisualRotation);
	phys->SetPosition(Position);
	phys->SetRotation(VisualRotation);
	phys->SetVelocity(Velocity);
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
	stream->Write(VisualRotation);
}

void NetworkedCar::ReadUpdate(RakNet::BitStream* stream) {
	stream->Read(Position);
	stream->Read(Velocity);
	stream->Read(VisualRotation);
}

void NetworkedCar::FrameStep() {
	if (Vehicle == nullptr) return;
	ClientController* client = Core::GetClientController();
	if (ShouldBeNetworkedByLocalPlayer()) return;
	UpdateTransforms();
}

void NetworkedCar::OwnedStep() {
	if (Vehicle == nullptr) return;
	PHBaseObj* phys = Vehicle->GetPhysicsObject();
	mat<float,4,4>* matrix = phys->GetMatrix();
	mat<float, 4, 4> newMatrix = *matrix;
	Position = GetPosition(&newMatrix);
	VisualRotation = GetQuaternionRotation(&newMatrix);
	Velocity = phys->GetVelocity();
	RakNet::BitStream bs;
	bs.Write((unsigned char)ID_CARCONTROLLER_UPDATE);
	bs.Write(UID);
	WriteUpdate(&bs);
	Core::GetClientController()->Send(&bs, PacketPriority::MEDIUM_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

void NetworkedCar::DoSpawnCar() {
	_stepsSinceSpawn = 0;
	CVehicleManager* vehicleManager = CVehicleManager::GetInstance();
	_requestedSpawn = false;
	CVehicle* veh;
	CVehicle** result = vehicleManager->CreateVehicle(&veh, VehicleModel, Position, VisualRotation.a[1], false, true, true);
	Vehicle = *result;
	Vehicle->SetColor(Color);
	LifeAcquirableVehicleManager::GetInstance()->AddVehicle(Vehicle, 1);
	UpdateTransforms();
}

void NetworkedCar::Step() {
	if (_requestedSpawn) {
		SpoolableResourceManager* resourceManager = SpoolableResourceManager::GetInstance();
		if (resourceManager->IsEntityLoaded(SpooledPackageType::Vehicles, VehicleModel)) {
			DoSpawnCar();
		}
	}
	if (Vehicle == nullptr) return;
	_stepsSinceSpawn++;
	ClientController* client = Core::GetClientController();
	CVehicle* myVehicle = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle();
	
	if (myVehicle == Vehicle && OwnershipKind != OwnershipKinds::Driving)
		RequestOwnership();
	else if (myVehicle != Vehicle && OwnershipKind == OwnershipKinds::Driving && Owner == client->MyGUID)
		ReleaseOwnership();

	if (ShouldBeNetworkedByLocalPlayer())
		OwnedStep();
}

void NetworkedCar::RequestSpawnCar() {
	if (_requestedSpawn) return;
	if (Vehicle != nullptr) return;
	_requestedSpawn = true;
	SpoolableResourceManager* resourceManager = SpoolableResourceManager::GetInstance();
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
	if (_stepsSinceSpawn <= SpawnNetworkStepCooldown) return false;
	if (Vehicle == nullptr) return false;
	if (Core::GetClientController()->MyGUID == Owner)
		return true;
	else
		return false;
	CCharacter* player = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	if (player->GetVehicle() == Vehicle) return true;
	vec<float,3> playerPosition = GetPosition(player->GetMatrix());

	float dx = Position.a[0] - playerPosition.a[0];
	float dy = Position.a[1] - playerPosition.a[1];
	float dz = Position.a[2] - playerPosition.a[2];

	float dist = sqrt(dx * dx + dy * dy + dz * dz);
	return dist <= NetworkDistance;
}