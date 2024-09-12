#include "CCharacterManager.h"

CCharacterManager* CCharacterManager::GetInstance() {
	return *(CCharacterManager**)0x0070C6E0;
}