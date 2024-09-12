#pragma once

class Core {
public:
	static void Initialize();
	static void Step();
	static float FixedDeltaTime;
};