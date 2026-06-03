#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "job.hpp"

struct JobQueue {
public:
    void push(std::unique_ptr<Job> job) {
        std::lock_guard<std::mutex> lock(m);
        q.push(std::move(job));
        cv.notify_one();
    }

    std::unique_ptr<Job> tryPop() {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&]{return !q.empty() or stopped;});
        if(stopped and q.empty()) {
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

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(m);
            stopped = true;
        }
        cv.notify_all();
    };
private:
    std::mutex m;
    std::queue<std::unique_ptr<Job>> q;
    std::condition_variable cv;
    bool stopped = false;
};


