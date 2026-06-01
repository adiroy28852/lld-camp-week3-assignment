#pragma once

#include <cstdint>
#include <stdexcept>
#include "job.hpp"

class SumRangeJob : public Job {
    int32_t l;
    int32_t r;

public:
    SumRangeJob(int32_t l_, int32_t r_) : l(l_), r(r_) {}

    JobResult run() override {
        int32_t sum = 0;
        for (int32_t i = l; i <= r; i++) {
            sum += i;
        }
        return JobResult{true, "OK", sum};
    }
};

class PrimeCountJob : public Job {
    int32_t l;
    int32_t r;

    bool isPrime(int32_t n) const {
        if (n <= 1) return false;
        for (int32_t i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

public:
    PrimeCountJob(int32_t l_, int32_t r_) : l(l_), r(r_) {}

    JobResult run() override {
        int32_t count = 0;
        for (int32_t i = l; i <= r; i++) {
            if (isPrime(i)) {
                count++;
            }
        }
        return JobResult{true, "OK", count};
    }
};

class FailingJob : public Job {
public:
    JobResult run() override {
        throw std::runtime_error("This job is designed to fail");
    }
};