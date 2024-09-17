#pragma once
#include "PHBaseObj.h"

class CVehicle;

class CHandling {
public:
	PHBaseObj* GetPhysicsObject();
	void GetOrientation(quat<float>* out);
	CVehicle* GetVehicle();
};