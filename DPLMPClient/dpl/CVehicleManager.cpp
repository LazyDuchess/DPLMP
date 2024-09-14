#include "CVehicleManager.h"

CVehicleManager* CVehicleManager::GetInstance() {
	return (CVehicleManager*)(*(int*)(0x70C830) - 4);
}

CVehicle** CVehicleManager::CreateVehicle(CVehicle** pVehicle, tVehicleModelUID tVehicle, vec<float, 3> position, float fRotation, bool unk1, bool unk2, bool unk3)
{
	return ((CVehicle * *(__thiscall*)(CVehicleManager*, CVehicle**, tVehicleModelUID, float, float, float, float, float, bool, bool, bool))0x5B154B)(this, pVehicle, tVehicle, position.a[0], position.a[1], position.a[2], 1, fRotation, unk1, unk2, unk3);
}