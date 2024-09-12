#pragma once

class CLifeEnvironment {
public:
	static CLifeEnvironment* GetInstance();
private:
	char _pad[0x110];
public:
	float LifeTime;
private:
	char _pad2[0x4];
public:
	float TimeStep;
};