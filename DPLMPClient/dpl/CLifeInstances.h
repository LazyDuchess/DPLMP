#pragma once
class CLifeInstances {
public:
	void RemoveAll();
	static CLifeInstances* GetInstance();
};