#include "CStateFrontend.h"

CStateFrontend* CStateFrontend::GetInstance() {
	return *(CStateFrontend**)0x0070C750;
}

void CStateFrontend::EnterGame() {
	((void(__thiscall*)(CStateFrontend*))0x0045a4d9)(this);
}