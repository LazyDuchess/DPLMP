#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "CVehicle.h"

class CCharacter {
public:
	void SetModel(int model);
	void SetAnimType(int type);
	void SwapSkin(int animations, int skin);
	mat<float, 4, 4>* GetMatrix();
	CVehicle* GetVehicle();
	void EnterVehicle(CVehicle* pVehicle, int nDoor, bool toDriverSeat, bool unk2 = true);
	void ExitVehicle();
	void EnterVehicleImmediate(CVehicle* pVehicle, int nDoor, bool unk);
	int GetCarSeat();
	float GetHealth();
	void SetHealth(float health);
};