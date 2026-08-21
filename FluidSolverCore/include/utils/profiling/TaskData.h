#pragma once

#include <chrono>

typedef std::chrono::duration<double, std::milli> Duration;
struct TaskData {
	int calls = 0;
	Duration totalDuration{};
};
