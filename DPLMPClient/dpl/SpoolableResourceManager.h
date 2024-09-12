#pragma once
#include "SpoolPriority.h"
#include "SpooledPackageType.h"

class SpoolableResourceManager {
public:
	void SetEntityPriority(SpooledPackageType packageType, int ID, SpoolPriority priority);
	void RequestEntity(SpooledPackageType packageType, int ID, SpoolPriority priority, int unk2);
	bool IsEntityLoaded(SpooledPackageType packageType, int ID);
	bool IsEntityPresent(SpooledPackageType packageType, int ID);
	static SpoolableResourceManager* GetInstance();
};