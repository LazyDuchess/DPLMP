#pragma once
#include "EventListener.h"
#include "NetworkedCar.h"

class ExtrasController : public EventListener {
public:
	void Step();
private:
	NetworkedCar* GetCarToEnterPassenger();
	void OnPassengerButtonPressed();
	const float PassengerCarDistance = 20.0;
};