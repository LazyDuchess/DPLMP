#pragma once

class CLifeEnvironment {
public:
	static CLifeEnvironment* GetInstance();
private:
	char _pad[0x110];
public:
	float LifeTime;
};