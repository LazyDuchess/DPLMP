#include "ResourceManager.h"
#include "../dpl/SpoolableResourceManager.h"

ResourceManager* ResourceManager::_instance = nullptr;

ResourceManager::ResourceManager() {
	_instance = this;
	std::map<std::pair<SpooledPackageType, int>, int> _loadedAssets;
}

ResourceManager* ResourceManager::GetInstance() {
	return _instance;
}

bool ResourceManager::IsAssetLoaded(SpooledPackageType packageType, int id) {
	return SpoolableResourceManager::GetInstance()->IsEntityLoaded(packageType, id);
}

void ResourceManager::RequestAsset(SpooledPackageType packageType, int id) {

}

void ResourceManager::ReleaseAsset(SpooledPackageType packageType, int id) {

}