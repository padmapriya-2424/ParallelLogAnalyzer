#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "log_parser.h"
void processChunk(const std::string& filePath,
                  long long start,
                  long long end,
                  Stats& localStats) {
    std::ifstream file(filePath);
    file.seekg(start);

    std::string line;

    // If not at beginning, skip partial line
    if (start != 0) {
        std::getline(file, line);
    }

    while (file.tellg() < end && std::getline(file, line)) {
        processLine(line, localStats);
    }
}
void runMultiThread(const std::string& filePath, int numThreads) {
    std::ifstream file(filePath, std::ios::ate);
    long long fileSize = file.tellg();
    file.close();

    long long chunkSize = fileSize / numThreads;

    std::vector<std::thread> threads;
    std::vector<Stats> threadStats(numThreads);

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; i++) {
        long long start = i * chunkSize;
        long long end = (i == numThreads - 1) ? fileSize : start + chunkSize;

        threads.emplace_back(processChunk,
                             filePath,
                             start,
                             end,
                             std::ref(threadStats[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    Stats finalStats;
    for (auto& s : threadStats) {
        finalStats.error4xx += s.error4xx;
        finalStats.error5xx += s.error5xx;
        finalStats.totalResponseTime += s.totalResponseTime;
        finalStats.requestCount += s.requestCount;

        for (auto& p : s.ipCount) {
            finalStats.ipCount[p.first] += p.second;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = endTime - startTime;

    std::cout << "\nMulti Thread Results (" << numThreads << " threads)\n";
    std::cout << "---------------------------------\n";
    std::cout << "Time: " << diff.count() << " sec\n";
    std::cout << "4xx Errors: " << finalStats.error4xx << "\n";
    std::cout << "5xx Errors: " << finalStats.error5xx << "\n";
    std::cout << "Avg Response Time: "
              << (double)finalStats.totalResponseTime / finalStats.requestCount
              << " ms\n";
}
