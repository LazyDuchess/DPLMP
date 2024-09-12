#include "Core.h"
#include "ServerController.h"
#include <iostream>
#include <chrono>
#include "CommonDefines.h"

typedef std::chrono::steady_clock steady_clock;
float Core::FixedDeltaTime = 0.0;
steady_clock::time_point beginTimePoint;

void Core::Initialize() {
	beginTimePoint = steady_clock::now();
	printf("Core::Initialize()\n");
	ServerController::GetInstance().Connect();
}

void Core::Step() {
	std::chrono::duration<float> delta = steady_clock::now() - beginTimePoint;
	Core::FixedDeltaTime = delta.count();
	if (Core::FixedDeltaTime >= TICKRATE){
		ServerController::GetInstance().Step();
		beginTimePoint = steady_clock::now();
	}
}