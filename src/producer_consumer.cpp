#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "log_parser.h"
std::queue<std::vector<std::string>> taskQueue;
std::mutex queueMutex;
std::condition_variable cv;
bool finishedReading = false;
void producer(const std::string& filePath, int batchSize) {
    std::ifstream file(filePath);
    std::string line;
    std::vector<std::string> batch;

    while (std::getline(file, line)) {
        batch.push_back(line);

        if (batch.size() == batchSize) {
            std::unique_lock<std::mutex> lock(queueMutex);
            taskQueue.push(std::move(batch));
            lock.unlock();
            cv.notify_one();
            batch.clear();
        }
    }

    if (!batch.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskQueue.push(std::move(batch));
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        finishedReading = true;
    }
    cv.notify_all();
}
void consumer(Stats& localStats) {
    while (true) {
        std::vector<std::string> batch;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [] {
                return !taskQueue.empty() || finishedReading;
            });

            if (taskQueue.empty() && finishedReading)
                break;

            batch = std::move(taskQueue.front());
            taskQueue.pop();
        }

        for (const auto& line : batch) {
            processLine(line, localStats);
        }
    }
}
void runProducerConsumer(const std::string& filePath, int numWorkers) {
    const int BATCH_SIZE = 1000;

    std::vector<std::thread> workers;
    std::vector<Stats> workerStats(numWorkers);

    auto start = std::chrono::high_resolution_clock::now();

    std::thread prod(producer, filePath, BATCH_SIZE);

    for (int i = 0; i < numWorkers; i++) {
        workers.emplace_back(consumer, std::ref(workerStats[i]));
    }

    prod.join();
    for (auto& w : workers) {
        w.join();
    }

    Stats finalStats;
    for (auto& s : workerStats) {
        finalStats.error4xx += s.error4xx;
        finalStats.error5xx += s.error5xx;
        finalStats.totalResponseTime += s.totalResponseTime;
        finalStats.requestCount += s.requestCount;

        for (auto& p : s.ipCount) {
            finalStats.ipCount[p.first] += p.second;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "\nProducer-Consumer Results (" << numWorkers << " workers)\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Time: " << diff.count() << " sec\n";
    std::cout << "4xx Errors: " << finalStats.error4xx << "\n";
    std::cout << "5xx Errors: " << finalStats.error5xx << "\n";
    std::cout << "Avg Response Time: "
              << (double)finalStats.totalResponseTime / finalStats.requestCount
              << " ms\n";
}
