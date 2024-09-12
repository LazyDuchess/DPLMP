#pragma once
#include "EventListener.h"
#include "ClientController.h"
#include <vector>

class Core {
public:
	static ClientController* GetClientController();
	static bool InGame;
	static void Initialize();
	static void RegisterEventListener(EventListener* listener);
private:
	static ClientController* _clientController;
};