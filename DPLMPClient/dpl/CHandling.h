#pragma once
#include "PHBaseObj.h"

class CVehicle;

class CHandling {
public:
	PHBaseObj* GetPhysicsObject();
	void GetOrientation(quat<float>* out);
	CVehicle* GetVehicle();
	// TODO: MAKE WORK FOR NON-CAR CHANDLINGS!!!
	float GetSteering();
	void SetSteering(float steering);
	bool GetHandbraking();
	void SetHandbraking(bool handbraking);
	float GetPower();
	void SetPower(float power);
	float GetThrottle();
	void SetThrottle(float throttle);
	float GetRPM();
	void SetRPM(float rpm);
};