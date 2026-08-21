#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "TaskData.h"
#include "ProfilerResults.h"

class ProfileScope;

class Profiler {
public:
	Profiler(const Profiler& other) = delete;
	void operator =(const Profiler&) = delete;

	Duration getTaskAverageDuration(const std::string& id) const {
		const TaskData& data = m_profilingDataMap.at(id);
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


	ProfilerResults getResults() const {
		std::vector<TaskResults> taskResults{};
		Duration total{};
		for (auto& id : getIDs()) {
			Duration averageDuration = getTaskAverageDuration(id);
			TaskData taskData = m_profilingDataMap.at(id);
			total += taskData.totalDuration;
			taskResults.emplace_back(id, taskData, averageDuration);
		}
		return {taskResults, total};
	}

	static Profiler& getInstance() {
		static Profiler instance;
		return instance;
	}


private:
	Profiler() {}
	std::unordered_map<std::string, TaskData> m_profilingDataMap;

	friend class ScopeProfiler;
};
