#pragma once
#include <unordered_set>

class UIDProvider {
public:
	UIDProvider();
	unsigned int GetUID();
	void FreeUID(unsigned int uid);
private:
	unsigned int _nextUID;
	std::unordered_set<unsigned int> _claimedUIDs;
};