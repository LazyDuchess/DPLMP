#include "NetworkedCar.h"

NetworkedCar::NetworkedCar(tVehicleModelUID model, vec<float, 3> position, quat<float> rotation, UIDProvider* uidProvider, vec<float,3> color) {
	VehicleModel = model;
	Position = position;
	Rotation = rotation;
	Velocity = { 0,0,0 };
	_uidProvider = uidProvider;
	UID = _uidProvider->GetUID();
	Owner = RakNet::UNASSIGNED_RAKNET_GUID;
	OwnershipKind = OwnershipKinds::Normal;
	Color = color;
}

void NetworkedCar::WriteFullState(RakNet::BitStream* stream) {
	stream->Write(UID);
	stream->Write(VehicleModel);
	stream->Write(Position);
	stream->Write(Velocity);
	stream->Write(Rotation);
	stream->Write(Owner);
	stream->Write(OwnershipKind);
	stream->Write(Color);
}

void NetworkedCar::WriteUpdate(RakNet::BitStream* stream) {
	stream->Write(UID);
	stream->Write(Position);
	stream->Write(Velocity);
	stream->Write(Rotation);
}

void NetworkedCar::ReadUpdate(RakNet::BitStream* stream) {
    stream->Read(Position);
    stream->Read(Velocity);
    stream->Read(Rotation);
}