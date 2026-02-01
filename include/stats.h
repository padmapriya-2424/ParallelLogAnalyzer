#ifndef STATS_H
#define STATS_H

#include <unordered_map>
#include <string>

struct Stats {
    std::unordered_map<std::string, int> ipCount;
    int error4xx = 0;
    int error5xx = 0;
    long long totalResponseTime = 0;
    int requestCount = 0;
};

#endif
