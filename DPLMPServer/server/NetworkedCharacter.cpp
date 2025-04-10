#include "NetworkedCharacter.h"

NetworkedCharacter::NetworkedCharacter(int model, vec<float, 3> position, quat<float> rotation, UIDProvider* uidProvider) {
	_uidProvider = uidProvider;
	UID = _uidProvider->GetUID();
	Position = position;
	Rotation = rotation;
	Player = false;
	Model = model;
	Owner = RakNet::UNASSIGNED_RAKNET_GUID;
	CarUID = 0;
	CarSeat = -1;
	Health = 1;
}

void NetworkedCharacter::WriteFullState(RakNet::BitStream* stream) {
	stream->Write(Model);
	stream->Write(Player);
	stream->Write(Owner);
	WriteUpdate(stream);
}

void NetworkedCharacter::WriteUpdate(RakNet::BitStream* stream) {
	stream->Write(Position);
	stream->Write(Rotation);
	stream->Write(CarUID);
	stream->Write(CarSeat);
	stream->Write(Health);
}

void NetworkedCharacter::ReadUpdate(RakNet::BitStream* stream) {
	stream->Read(Position);
	stream->Read(Rotation);
	stream->Read(CarUID);
	stream->Read(CarSeat);
	stream->Read(Health);
}