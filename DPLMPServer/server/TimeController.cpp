#include "TimeController.h"
#include "Core.h"
#include <stdio.h>
#include "PLMessageIdentifiers.h"
#include "BitStream.h"
#include "ServerController.h"

TimeController::TimeController() {
	LifeTime = 0.0;
	TimeStep = TIMESTEP;
	_packetTimer = 0.0;
}

void TimeController::Step() {
	LifeTime += TimeStep * Core::FixedDeltaTime;
	_packetTimer += Core::FixedDeltaTime;
	if (_packetTimer >= PACKETINTERVAL) {
		SendTimePacket();
		_packetTimer = 0.0;
	}
}

void TimeController::SendTimePacket() {
	RakNet::BitStream bs;
	bs.Write((unsigned char) ID_TIMEOFDAY);
	bs.Write(LifeTime);
	bs.Write(TimeStep);
	ServerController::GetInstance().Broadcast(&bs, PacketPriority::LOW_PRIORITY, PacketReliability::UNRELIABLE, 0);
}