#include <atomic>
#include <chrono>
#include <exception>
#include <functional>
#include <iostream>
#include <thread>

#include "blockingJobQueue.hpp"
#include "job.hpp"
#include "jobs.hpp"
#include "singleThreadJobEngine.hpp"


void producer(JobQueue& q, std::atomic<bool>& done) {
    for (int i = 0; i < 100000; i++) {
        if (i % 3 == 0) {
            q.push(std::make_unique<SumRangeJob>(i, i + 100));
        } else if (i % 3 == 1) {
            q.push(std::make_unique<PrimeCountJob>(i, i + 100));
        } else {
            q.push(std::make_unique<FailingJob>());
        }
    }
    done = true;
}


int32_t main() {
    SingleThreadJobEngine engine;
    JobQueue jobQueue;
    std::atomic<bool> producerDone{false};
/*
    PHASE 1 code here
    SumRangeJob sumJob{1, 1000};
    JobResult sumResult = engine.executeNow(sumJob);
    std::cout << "Sum from 1 to 100: " << sumResult.value
              << " (" << sumResult.message << ")\n";

    PrimeCountJob primeJob{1, 1000};
    JobResult primeResult = engine.executeNow(primeJob);
    std::cout << "Number of primes from 1 to 100: " << primeResult.value
              << " (" << primeResult.message << ")\n";

    FailingJob failJob;
    try {
        engine.executeNow(failJob);
    } catch (const std::exception& ex) {
        std::cout << "FailingJob threw exception: " << ex.what() << '\n';
    }
*/
    std::thread producerThread(producer, std::ref(jobQueue), std::ref(producerDone));

    while (true) {
        auto job = jobQueue.tryPop();
        if (job) {
            try {
                auto result = engine.executeNow(*job);
                std::cout << "Job result: " << result.value
                          << " (" << result.message << ")\n";
            } catch (const std::exception& ex) {
                std::cout << "Job threw exception: " << ex.what() << '\n';
            }
            continue;
        }

        if (producerDone.load()) {
            break;
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    producerThread.join();
    return 0;
}