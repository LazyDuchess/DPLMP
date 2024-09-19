#pragma once
#include "../../DPLMPCommon/PLMath.h"
#include "CCharacter.h"

class CCharacterManager {
public:
	static CCharacterManager* GetInstance();
	CCharacter* CreateCharacter(int model, vec<float, 4>* position, float health);
};