#include "CHandling.h"

PHBaseObj* CHandling::GetPhysicsObject() {
	return *(PHBaseObj**)((int)this + 0xA0);
}