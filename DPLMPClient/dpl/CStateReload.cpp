#include "CStateReload.h"

CStateReload* CStateReload::GetInstance()
{
	return *(CStateReload**)0x0070C780;
}

void CStateReload::OnEnterState()
{
	((void(__thiscall*)(CStateReload*))0x004a941a)(this);
}