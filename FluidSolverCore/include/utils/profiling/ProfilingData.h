#pragma once

#include <chrono>

typedef std::chrono::duration<double, std::milli> Duration;
struct ProfilingData {
	int calls = 0;
	Duration totalDuration{};
};
