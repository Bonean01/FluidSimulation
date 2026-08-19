#pragma once

#include "Profiler.h"
#include "ProfilingData.h"

typedef std::chrono::steady_clock::time_point Instant;
class ProfileScope {
public:
	ProfileScope(const std::string& id) : m_id(id) {
		m_start = std::chrono::steady_clock::now();
	}

	~ProfileScope() {
		Instant end = std::chrono::steady_clock::now();
		Duration elapsed = std::chrono::duration<double, std::milli>(end - m_start);

		Profiler& profiler = Profiler::getInstance();
		ProfilingData& data = profiler.profilingDataMap[m_id];

		data.calls++;
		data.totalDuration += elapsed;
	}


private:
	Instant m_start;
	std::string m_id;
};
