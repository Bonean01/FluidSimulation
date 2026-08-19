#pragma once

#include <chrono>

typedef std::chrono::duration<double, std::milli> Duration;
struct ProfilingData {
	int calls;
	Duration totalDuration;
};
