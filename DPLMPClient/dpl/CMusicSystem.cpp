#include "CMusicSystem.h"

char CMusicSystem::GetSongCount() {
	char* addr = ((char*)(*(int*)((int)this + 0x2c)));
	if (addr != nullptr)
		return addr[0];
	return 0;
}

char CMusicSystem::GetCurrentSong() {
	char* addr = ((char*)(*(int*)((int)this + 0x2c)));
	if (addr != nullptr)
		return addr[1];
	return 0;
}

void CMusicSystem::SetCurrentSong(char index) {
	char* addr = ((char*)(*(int*)((int)this + 0x2c)));
	if (addr != nullptr)
		addr[1] = index;
}