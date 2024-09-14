#pragma once

class CLoadingScreen {
public:
	static CLoadingScreen* GetInstance();
	void Deactivate();
	void Activate(int unk);
};