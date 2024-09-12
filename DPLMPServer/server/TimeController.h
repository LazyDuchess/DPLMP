#pragma once
#define TIMESTEP 0.016
#define PACKETINTERVAL 3.0

class TimeController {
public:
	TimeController();
	void Step();
	float CurrentHour;
	float TimeStep;
private:
	float _packetTimer;
	void SendTimePacket();
};