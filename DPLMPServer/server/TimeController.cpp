#include "TimeController.h"
#include "Core.h"
#include <stdio.h>

TimeController::TimeController() {
	LifeTime = 0.0;
	_packetTimer = 0.0;
}

void TimeController::Step() {
	LifeTime += TIMESTEP * Core::FixedDeltaTime;
	_packetTimer += Core::FixedDeltaTime;
}