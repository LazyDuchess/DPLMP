#include "CLoadingScreen.h"

void CLoadingScreen::Deactivate() {
	((void(__thiscall*)(CLoadingScreen*))0x004a7732)(this);
}

CLoadingScreen* CLoadingScreen::GetInstance() {
	return *(CLoadingScreen**)0x0070c798;
}