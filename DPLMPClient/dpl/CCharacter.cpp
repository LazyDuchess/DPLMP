#include "CCharacter.h"
#include "CVehicle.h"

void CCharacter::SetModel(int model)
{
	*(int*)((int)this + 0x744) = model;
	*(int*)((int)this + 0x764) = model;
}

void CCharacter::SetAnimType(int type)
{
	*(int*)((int)this + 0x63C) = type;
}

void CCharacter::SwapSkin(int animations, int skin) 
{
	((void(__thiscall*)(CCharacter*, int, int))0x004eb4fc)(this, animations, skin);
}

mat<float, 4, 4>* CCharacter::GetMatrix()
{
	return (mat<float, 4, 4>*)((int)this + 16);
}

CVehicle* CCharacter::GetVehicle()
{
	return *(CVehicle**)((int)this + 0x688);
}

void CCharacter::EnterVehicle(CVehicle* pVehicle, int nDoor, bool toDriverSeat, bool unk2)
{
	((void(__thiscall*)(CCharacter*, CVehicle*, int, bool, bool))0x4EBF46)(this, pVehicle, nDoor, toDriverSeat, unk2);
}

void CCharacter::EnterVehicleImmediate(CVehicle* pVehicle, int nDoor, bool unk)
{
	((void(__thiscall*)(CCharacter*, CVehicle*, int, bool))0x004ef9ee)(this, pVehicle, nDoor, unk);
}

void CCharacter::ExitVehicle()
{
	((void(__thiscall*)(CCharacter*, int, int, int))0x004eb560)(this, 0, 0, 0);
}

int CCharacter::GetCarSeat() {
	return *(int*)((int)this + 0x5d0);
}