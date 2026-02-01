#include <fstream>
#include <iostream>
#include <chrono>
#include "log_parser.h"

void runSingleThread(const std::string& filePath) {
    Stats stats;
    std::ifstream file(filePath);
    std::string line;

    auto start = std::chrono::high_resolution_clock::now();

    while (getline(file, line)) {
        processLine(line, stats);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Single Thread Results\n";
    std::cout << "---------------------\n";
    std::cout << "Time: " << diff.count() << " sec\n";
    std::cout << "4xx Errors: " << stats.error4xx << "\n";
    std::cout << "5xx Errors: " << stats.error5xx << "\n";
    std::cout << "Avg Response Time: "
              << (double)stats.totalResponseTime / stats.requestCount << " ms\n";
}
