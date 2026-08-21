#pragma once

#include <vector>

#include "TaskData.h"

struct TaskResults {
    std::string id;
    TaskData taskData;
    Duration averageDuration;
};

struct ProfilerResults {
    std::vector<TaskResults> taskResults;
    Duration totalDuration;
};
