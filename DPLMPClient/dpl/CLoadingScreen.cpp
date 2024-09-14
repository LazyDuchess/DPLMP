#include "CLoadingScreen.h"

void CLoadingScreen::Deactivate() {
	((void(__thiscall*)(CLoadingScreen*))0x004a7732)(this);
}

void CLoadingScreen::Activate(int unk) {
	((void(__thiscall*)(CLoadingScreen*, int))0x004a76ef)(this, unk);
}

CLoadingScreen* CLoadingScreen::GetInstance() {
	return *(CLoadingScreen**)0x0070c798;
}