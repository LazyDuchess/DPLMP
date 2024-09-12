#include "TimeController.h"
#include "PLMessageIdentifiers.h"
#include "BitStream.h"
#include "../dpl/CLifeEnvironment.h"

void TimeController::HandlePacket(RakNet::Packet* packet) {
	if (packet->data[0] != ID_TIMEOFDAY) return;
	auto env = CLifeEnvironment::GetInstance();
	if (env == nullptr) return;
	RakNet::BitStream bs(packet->data, packet->length, false);
	float timeValue = 0.0;
	bs.Read(timeValue);
	env->LifeTime = timeValue;
}