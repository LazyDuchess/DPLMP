#include "TimeController.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "BitStream.h"
#include "../dpl/CLifeEnvironment.h"
#include "Core.h"
#include "../../DPLMPCommon/TimeCommon.h"

TimeController::TimeController() {
	_timeStep = 0.0;
	_currentHour = 0.0;
}

void TimeController::FrameStep() {
	auto env = CLifeEnvironment::GetInstance();
	if (env == nullptr) return;
	env->TimeStep = 0.0;
	_currentHour = ClampHours(_currentHour + (_timeStep * Core::DeltaTime));
	env->LifeTime = HoursToLifeTime(_currentHour);
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
	_currentHour = timeValue;
	_timeStep = timeStep;
}