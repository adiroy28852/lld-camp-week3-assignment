#pragma once

#include <memory>
#include <mutex>
#include <queue>

#include "job.hpp"

struct JobQueue {
public:
    void push(std::unique_ptr<Job> job) {
        std::lock_guard<std::mutex> lock(m);
        q.push(std::move(job));
    }

    std::unique_ptr<Job> tryPop() {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            return nullptr;
        }

        std::unique_ptr<Job> job = std::move(q.front());
        q.pop();
        return job;
    }
    std::size_t size() {
        std::lock_guard<std::mutex> lock(m);
        return q.size();
    }

private:
    std::mutex m;
    std::queue<std::unique_ptr<Job>> q;
};


