#pragma once
#include "CVehicle.h"

class LifeAcquirableVehicleManager {
public:
	static LifeAcquirableVehicleManager* GetInstance();
	void AddVehicle(CVehicle* pVehicle, int a2);
};