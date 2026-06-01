#pragma once

#include <string>

struct JobResult {
    bool success;
    std::string message;
    int value;
};

class Job {
public:
    virtual ~Job() = default;
    virtual JobResult run() = 0;
};