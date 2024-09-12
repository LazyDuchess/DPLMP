#pragma once
#include "CPlayerDriverBehaviour.h"

class CLifePlayer {
private:
	char pad[0x8];
public:
	CPlayerDriverBehaviour* DriverBehaviour;
};