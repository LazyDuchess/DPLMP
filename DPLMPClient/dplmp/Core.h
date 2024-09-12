#pragma once
#include "EventListener.h"
#include "ClientController.h"
#include <vector>
#include <memory>

class Core {
public:
	static bool InGame;
	static std::shared_ptr<ClientController> GetClientController();
	static void Initialize();
	static void RegisterEventListener(std::shared_ptr<EventListener> listener);
private:
	static std::shared_ptr<ClientController> _clientController;
};