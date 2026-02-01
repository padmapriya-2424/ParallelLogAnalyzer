#include "log_parser.h"
#include <sstream>

void processLine(const std::string& line, Stats& stats) {
    std::istringstream iss(line);

    std::string ip, method, endpoint;
    int status, responseTime;

    iss >> ip >> method >> endpoint >> status >> responseTime;

    stats.ipCount[ip]++;
    stats.totalResponseTime += responseTime;
    stats.requestCount++;

    if (status >= 400 && status < 500)
        stats.error4xx++;
    else if (status >= 500)
        stats.error5xx++;
}
