#pragma once
#include "CLifePlayer.h"

class CLifeSystem {
private:
	char pad[0x8];
public:
	CLifePlayer* Player;
	static CLifeSystem* GetInstance();
};