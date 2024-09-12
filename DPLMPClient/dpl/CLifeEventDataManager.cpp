#include "CLifeEventDataManager.h"

void CLifeEventDataManager::EndAllLifeEvents() {
	((void(__thiscall*)(CLifeEventDataManager*))0x0046e288)(this);
}

CLifeEventDataManager* CLifeEventDataManager::GetInstance() {
	return *(CLifeEventDataManager**)0x0070C7C8;
}