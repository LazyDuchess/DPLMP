#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "../../DPLMPCommon/VehicleModel.h"
#include "CVehicle.h"

class CVehicleManager {
public:
	static CVehicleManager* GetInstance();
	CVehicle** CreateVehicle(CVehicle** pVehicle, tVehicleModelUID tVehicle, vec<float,3> position, float fRotation, bool unk1, bool unk2, bool unk3);
};