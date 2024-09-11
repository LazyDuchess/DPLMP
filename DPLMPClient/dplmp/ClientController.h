#pragma once

class ClientController {
public:
	static ClientController& GetInstance() {
		static ClientController instance;
		return instance;
	}
	void Connect();
};