#include "CLifeInstances.h"

void CLifeInstances::RemoveAll() {
    ((void(__thiscall*)(CLifeInstances*))0x0046edb5)(this);
}

CLifeInstances* CLifeInstances::GetInstance()
{
    return ((CLifeInstances * (__cdecl*)())0x46E91F)();
}