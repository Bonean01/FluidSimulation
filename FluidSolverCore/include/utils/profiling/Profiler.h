#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "ProfilingData.h"

class ProfileScope;

class Profiler {
public:
	Profiler(const Profiler& other) = delete;
	void operator =(const Profiler&) = delete;

	Duration getTaskAverageDuration(const std::string& id) const {
		const ProfilingData& data = m_profilingDataMap.at(id);
		return data.totalDuration / data.calls;
	}

	typedef std::vector<std::string> ids;
	ids getIDs() const {
		ids res{};
		for (auto& entry : m_profilingDataMap) {
			res.push_back(entry.first);
		}
		return res;
	}

	static Profiler& getInstance() {
		static Profiler instance;
		return instance;
	}


private:
	Profiler() {}
	std::unordered_map<std::string, ProfilingData> m_profilingDataMap;

	friend class ScopeProfiler;
};
