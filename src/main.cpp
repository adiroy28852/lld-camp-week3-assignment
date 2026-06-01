#include <iostream>
#include <cstdint>
#include "job.hpp"
#include "jobs.hpp"
#include "singleThreadJobEngine.hpp"

int32_t main() {
    SingleThreadJobEngine engine;

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

    return 0;
}