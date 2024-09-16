#pragma once
#include "EventListener.h"
#include "ClientController.h"
#include <vector>
#include <memory>
#include "ini.h"

class Core {
public:
	static bool InGame;
	static ClientController* GetClientController();
	static void Initialize();
	static void RegisterEventListener(EventListener* listener);
	static float FixedDeltaTime;
	static void AllowMissionObjectCreation(bool allow);
	static mINI::INIStructure Ini;
private:
	static ClientController* _clientController;
};