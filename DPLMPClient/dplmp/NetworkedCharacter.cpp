#include "NetworkedCharacter.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "Core.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "../dpl/CCharacterManager.h"
#include "../dpl/CLifeSystem.h"

NetworkedCharacter::NetworkedCharacter() {
	Owner = RakNet::UNASSIGNED_RAKNET_GUID;
	UID = 0;
	Model = 29;
	Position = { 0,0,0 };
	Rotation = { 0,0,0,0 };
	Character = nullptr;
	Player = false;
}

NetworkedCharacter::~NetworkedCharacter() {
	if (Character != nullptr) {
		delete Character;
	}
}

void NetworkedCharacter::ReadFullState(RakNet::BitStream* stream) {
	stream->Read(Model);
	stream->Read(Player);
	stream->Read(Owner);
	ReadUpdate(stream);
}

void NetworkedCharacter::WriteUpdate(RakNet::BitStream* stream) {
	stream->Write(Position);
	stream->Write(Rotation);
}

void NetworkedCharacter::ReadUpdate(RakNet::BitStream* stream) {
	stream->Read(Position);
	stream->Read(Rotation);
}

void NetworkedCharacter::Step() {
	if (Character == nullptr) return;
	if (Owner == Core::GetClientController()->MyGUID) {
		mat<float, 4, 4>* charMatrix = Character->GetMatrix();
		Position = GetPosition(charMatrix);
		Rotation = GetQuaternionRotation(charMatrix);
		RakNet::BitStream bs;
		bs.Write((unsigned char)ID_CHARACTERCONTROLLER_UPDATE);
		bs.Write(UID);
		WriteUpdate(&bs);
		Core::GetClientController()->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::UNRELIABLE, 0);
	}
}

void NetworkedCharacter::FrameStep() {
	if (Character == nullptr) return;
	if (Owner == Core::GetClientController()->MyGUID) return;
	mat<float, 4, 4>* charMatrix = Character->GetMatrix();
	vec<float,3> curPosition = GetPosition(charMatrix);
	quat<float> curRotation = GetQuaternionRotation(charMatrix);
	curPosition = Lerp(curPosition, Position, Core::DeltaTime * LerpSpeed);
	curRotation = SLerp180Clamped(curRotation, Rotation, Core::DeltaTime * SlerpSpeed);
	SetPosition(charMatrix, curPosition);
	SetQuaternionRotation(charMatrix, curRotation);
}

void NetworkedCharacter::Spawn() {
	CCharacterManager* charManager = CCharacterManager::GetInstance();
	vec<float, 4> pos4 = { Position.a[0], Position.a[1], Position.a[2], 1.0 };
	if (Player && Owner == Core::GetClientController()->MyGUID)
	{
		Character = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
		printf("Spawning player character\n");
	}
	else
	{
		Character = charManager->CreateCharacter(Model, &pos4, 1.0);
		printf("Spawning character at %f %f %f\n", Position.a[0], Position.a[1], Position.a[2]);
	}
	Character->SwapSkin(Model, Model);
	mat<float,4,4>* charMatrix = Character->GetMatrix();
	SetPosition(charMatrix, Position);
	SetQuaternionRotation(charMatrix, Rotation);
}