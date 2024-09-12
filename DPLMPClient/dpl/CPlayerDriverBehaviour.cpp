#include "CPlayerDriverBehaviour.h"

CCharacter* CPlayerDriverBehaviour::GetCharacter()
{
    return *(CCharacter**)((int)this + 0x2C4);
}