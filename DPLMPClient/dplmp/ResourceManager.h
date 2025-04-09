#pragma once
#include "../dpl/SpooledPackageType.h"
#include <map>

class ResourceManager {
public:
	ResourceManager();
	static ResourceManager* GetInstance();
	bool IsAssetLoaded(SpooledPackageType packageType, int id);
	void RequestAsset(SpooledPackageType packageType, int id);
	void ReleaseAsset(SpooledPackageType packageType, int id);
private:
	static ResourceManager* _instance;
	std::map<std::pair<SpooledPackageType, int>, int> _loadedAssets;
};