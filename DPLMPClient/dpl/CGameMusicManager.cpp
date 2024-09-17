#include "CGameMusicManager.h"

CGameMusicManager* CGameMusicManager::GetInstance() {
	return *(CGameMusicManager**)0x0070c6bc;
}

CMusicSystem* CGameMusicManager::GetMusicSystem() {
	return *(CMusicSystem**)((int)this + 0x28);
}