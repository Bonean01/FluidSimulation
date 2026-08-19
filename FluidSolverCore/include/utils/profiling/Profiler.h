#pragma once

#include <unordered_map>
#include <string>

#include "ProfilingData.h"

class ProfileScope;

class Profiler {
public:
	Profiler(const Profiler& other) = delete;
	void operator =(const Profiler&) = delete;

	Duration getAverageDuration(const std::string& id) const {
		const ProfilingData& data = profilingDataMap.at(id);
		return data.totalDuration / data.calls;
	}

	static Profiler& getInstance() {
		static Profiler* instance;
		return *instance;
	}


private:
	Profiler() {}
	std::unordered_map<std::string, ProfilingData> profilingDataMap;

	friend class ProfileScope;
};












