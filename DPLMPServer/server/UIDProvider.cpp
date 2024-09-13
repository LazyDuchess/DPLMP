#include "UIDProvider.h"
#include <limits>

UIDProvider::UIDProvider() {
	_nextUID = 1;
}

unsigned int UIDProvider::GetUID() {
	if (_nextUID == std::numeric_limits<unsigned int>::max())
		_nextUID = 1;
	while (_claimedUIDs.find(_nextUID) != _claimedUIDs.end()) {
		_nextUID++;
	}
	_claimedUIDs.insert(_nextUID);
	return _nextUID;
}

void UIDProvider::FreeUID(unsigned int uid) {
	_claimedUIDs.erase(uid);
}