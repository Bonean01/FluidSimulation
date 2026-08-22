#pragma once

#include "Profiler.h"
#include "TaskData.h"

#include <iostream>

typedef std::chrono::steady_clock::time_point Instant;
class ScopeProfiler {
public:
	ScopeProfiler(const std::string& id) : m_id(id) {
		m_start = std::chrono::steady_clock::now();
	}

	~ScopeProfiler() {
		Instant end = std::chrono::steady_clock::now();
		Duration elapsed = std::chrono::duration<double, std::milli>(end - m_start);

		Profiler& profiler = Profiler::getInstance();
		TaskData& data = profiler.m_taskDataByID[m_id];
		data.calls++;
		data.totalDuration += elapsed;
	}


private:
	Instant m_start;
	std::string m_id;
};
