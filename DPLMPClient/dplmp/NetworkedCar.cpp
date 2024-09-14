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
	Rotation = { 0,0,0,0 };
	Color = { 1,1,1 };
	Velocity = { 0,0,0 };
	_requestedSpawn = false;
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
	stream->Write(Rotation);
}

void NetworkedCar::ReadUpdate(RakNet::BitStream* stream) {
	stream->Read(Position);
	stream->Read(Velocity);
	stream->Read(Rotation);
}

void NetworkedCar::FrameStep() {
	if (Vehicle == nullptr) return;
	ClientController* client = Core::GetClientController();
	if (Owner == client->MyGUID) return;
	CHandling* carHandling = nullptr;
	Vehicle->GetHandling(&carHandling);
	PHBaseObj* phys = carHandling->GetPhysicsObject();
	phys->SetPosition(Position);
	phys->SetRotation(Rotation);
	phys->SetVelocity(Velocity);
}

void NetworkedCar::OwnedStep() {
	if (Vehicle == nullptr) return;
	CHandling* carHandling = nullptr;
	Vehicle->GetHandling(&carHandling);
	PHBaseObj* phys = carHandling->GetPhysicsObject();
	Position = phys->GetPosition();
	Rotation = phys->GetRotation();
	Velocity = phys->GetVelocity();
	RakNet::BitStream bs;
	bs.Write((unsigned char)ID_CARCONTROLLER_UPDATE);
	bs.Write(UID);
	WriteUpdate(&bs);
	Core::GetClientController()->Send(&bs, PacketPriority::MEDIUM_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

void NetworkedCar::DoSpawnCar() {
	CVehicleManager* vehicleManager = CVehicleManager::GetInstance();
	_requestedSpawn = false;
	CVehicle* veh;
	CVehicle** result = vehicleManager->CreateVehicle(&veh, VehicleModel, Position, Rotation.a[1], false, true, true);
	Vehicle = *result;
	Vehicle->SetColor(Color);
	LifeAcquirableVehicleManager::GetInstance()->AddVehicle(Vehicle, 1);
	CHandling* carHandling = nullptr;
	Vehicle->GetHandling(&carHandling);
	PHBaseObj* phys = carHandling->GetPhysicsObject();
	phys->SetPosition(Position);
	phys->SetRotation(Rotation);
	phys->SetVelocity(Velocity);
}

void NetworkedCar::Step() {
	if (_requestedSpawn) {
		SpoolableResourceManager* resourceManager = SpoolableResourceManager::GetInstance();
		if (resourceManager->IsEntityLoaded(SpooledPackageType::Vehicles, VehicleModel)) {
			DoSpawnCar();
		}
	}
	if (Vehicle == nullptr) return;
	ClientController* client = Core::GetClientController();
	CVehicle* myVehicle = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle();
	
	if (myVehicle == Vehicle && OwnershipKind != OwnershipKinds::Driving)
		RequestOwnership();
	else if (myVehicle != Vehicle && OwnershipKind == OwnershipKinds::Driving && Owner == client->MyGUID)
		ReleaseOwnership();

	if (Owner == client->MyGUID)
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