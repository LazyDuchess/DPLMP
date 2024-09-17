#pragma once
#include "CMusicSystem.h"

class CGameMusicManager {
public:
	static CGameMusicManager* GetInstance();
	CMusicSystem* GetMusicSystem();
};