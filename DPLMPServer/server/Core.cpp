#include "Core.h"
#include "ServerController.h"
#include <iostream>

void Core::Initialize() {
	printf("Core::Initialize()\n");
	ServerController::GetInstance().Connect();
}

void Core::Step() {
	ServerController::GetInstance().Step();
}