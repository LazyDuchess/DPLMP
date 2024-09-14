#pragma once

class EventListener {
public:
	virtual void Step() {};
	virtual void FrameStep() {};
	virtual void OnPlayerCreated() {};
	virtual void OnEnterInGameState() {};
	virtual void OnExitInGameState() {};
};