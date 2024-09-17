#pragma once

class CStateReload {
public:
	static CStateReload* GetInstance();
	void OnEnterState();
};