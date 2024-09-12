#include "TimeController.h"
#include "Core.h"
#include <stdio.h>
#include "PLMessageIdentifiers.h"
#include "BitStream.h"
#include "ServerController.h"

TimeController::TimeController() {
	LifeTime = 0.0;
	_packetTimer = 0.0;
}

void TimeController::Step() {
	LifeTime += TIMESTEP * Core::FixedDeltaTime;
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
	ServerController::GetInstance().Broadcast(&bs, PacketPriority::LOW_PRIORITY, PacketReliability::UNRELIABLE, 0);
}