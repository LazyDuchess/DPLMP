#include "TimeController.h"
#include "PLMessageIdentifiers.h"
#include "BitStream.h"
#include "../dpl/CLifeEnvironment.h"
#include "Core.h"

TimeController::TimeController() {
	_timeStep = 0.0;
}

void TimeController::Step() {
	auto env = CLifeEnvironment::GetInstance();
	if (env == nullptr) return;
	env->TimeStep = 0.0;
	env->LifeTime += _timeStep * Core::FixedDeltaTime;
}

void TimeController::HandlePacket(RakNet::Packet* packet) {
	if (packet->data[0] != ID_TIMEOFDAY) return;
	auto env = CLifeEnvironment::GetInstance();
	if (env == nullptr) return;
	RakNet::BitStream bs(packet->data, packet->length, false);
	float timeValue = 0.0;
	float timeStep = 0.0;
	bs.IgnoreBytes(1);
	bs.Read(timeValue);
	bs.Read(timeStep);
	env->LifeTime = timeValue;
	_timeStep = timeStep;
}