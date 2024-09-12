#pragma once
#define TIMESTEP 1.0
#define PACKETINTERVAL 3.0

class TimeController {
public:
	TimeController();
	void Step();
	float LifeTime;
	float TimeStep;
private:
	float _packetTimer;
	void SendTimePacket();
};