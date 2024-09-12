#pragma once
class CLifeEventDataManager {
public:
	void EndAllLifeEvents();
	static CLifeEventDataManager* GetInstance();
};