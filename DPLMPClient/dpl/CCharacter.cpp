#include "CCharacter.h"

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