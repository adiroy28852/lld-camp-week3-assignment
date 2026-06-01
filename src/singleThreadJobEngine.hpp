class SingleThreadJobEngine {
public:
    JobResult executeNow(Job& job) {
        return job.run();
    }
};