#pragma once
#define TIMESTEP 0.250
#define PACKETINTERVAL 5.0

class TimeController {
public:
	TimeController();
	void Step();
	float LifeTime;
private:
	float _packetTimer;
	void SendTimePacket();
};