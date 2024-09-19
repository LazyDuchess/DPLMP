#include "CCharacterManager.h"

CCharacterManager* CCharacterManager::GetInstance() {
	return *(CCharacterManager**)0x0070C6E0;
}

void CCharacterManager::CreateCharacter(int model, vec<float, 4>* position, float health) {
	char unkArray[0x34];
	memset(unkArray, 0, 0x34);
	((void(__thiscall*)(CCharacterManager*, int, int, vec<float,4>*, float, int, int, char*, int, int, vec<float,4>*, float, float))0x004f723b)(this, 0x0019F730, model, position, 1.570796371, 1, 5, unkArray, 0, 0, position, 0.0, health);
}