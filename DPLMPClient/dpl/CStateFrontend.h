#pragma once

class CStateFrontend {
public:
	static CStateFrontend* GetInstance();
	void EnterGame();
};