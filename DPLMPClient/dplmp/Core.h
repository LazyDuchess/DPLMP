#pragma once
#include "EventListener.h"
#include "ClientController.h"
#include <vector>
#include <memory>

class Core {
public:
	static bool InGame;
	static ClientController* GetClientController();
	static void Initialize();
	static void RegisterEventListener(EventListener* listener);
private:
	static ClientController* _clientController;
};