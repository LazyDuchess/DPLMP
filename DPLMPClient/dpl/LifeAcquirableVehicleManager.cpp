#include "LifeAcquirableVehicleManager.h"

void LifeAcquirableVehicleManager::AddVehicle(CVehicle* pVehicle, int a2)
{
	((void(__thiscall*)(LifeAcquirableVehicleManager*, CVehicle*, int))0x4A2D97)(this, pVehicle, a2);
}

LifeAcquirableVehicleManager* LifeAcquirableVehicleManager::GetInstance()
{
	return  ((LifeAcquirableVehicleManager * (__cdecl*)())0x4A2CA1)();
}