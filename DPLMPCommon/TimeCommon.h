#pragma once
#include <corecrt_math.h>

float ClampHours(float hours) {
	float div = hours / 24.0;
	hours -= floor(div) * 24.0;
	return hours;
}

float LifeTimeToHours(float lifeTime) {
	return ClampHours(lifeTime / 3600.0);
}

float HoursToLifeTime(float hours) {
	return ClampHours(hours) * 3600.0f;
}