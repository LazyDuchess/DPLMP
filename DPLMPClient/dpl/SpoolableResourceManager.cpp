#include "SpoolableResourceManager.h"
#include "SpoolPriority.h"
#include "SpooledPackageType.h"

void SpoolableResourceManager::SetEntityPriority(SpooledPackageType packageType, int ID, SpoolPriority priority)
{
	((void(__thiscall*)(SpoolableResourceManager*, int, int, int))0x5492B8)(this, packageType, ID, priority);
}

void SpoolableResourceManager::RequestEntity(SpooledPackageType packageType, int ID, SpoolPriority priority, int unk2)
{
	((void(__thiscall*)(SpoolableResourceManager*, int, int, int))0x549284)(this, packageType, ID, priority);
}

bool SpoolableResourceManager::IsEntityLoaded(SpooledPackageType packageType, int ID)
{
	return ((bool(__thiscall*)(SpoolableResourceManager*, int, int))0x5481F7)(this, packageType, ID);
}

bool SpoolableResourceManager::IsEntityPresent(SpooledPackageType packageType, int ID)
{
	return ((bool(__thiscall*)(SpoolableResourceManager*, int, int))0x547F5E)(this, packageType, ID);
}

SpoolableResourceManager* SpoolableResourceManager::GetInstance()
{
	return *(SpoolableResourceManager**)(0x70C64C);
}