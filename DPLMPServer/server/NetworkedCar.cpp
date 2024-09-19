#include "NetworkedCar.h"

NetworkedCar::NetworkedCar(tVehicleModelUID model, vec<float, 3> position, quat<float> rotation, UIDProvider* uidProvider, vec<float,3> color) {
	VehicleModel = model;
	Position = position;
	Rotation = rotation;
	Velocity = { 0,0,0 };
	AngularVelocity = { 0,0,0,0 };
	_uidProvider = uidProvider;
	UID = _uidProvider->GetUID();
	Owner = RakNet::UNASSIGNED_RAKNET_GUID;
	OwnershipKind = OwnershipKinds::Normal;
	Color = color;
	Steering = 0;
}

void NetworkedCar::WriteFullState(RakNet::BitStream* stream) {
	stream->Write(VehicleModel);
	WriteUpdate(stream);
	stream->Write(Owner);
	stream->Write(OwnershipKind);
	stream->Write(Color);
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