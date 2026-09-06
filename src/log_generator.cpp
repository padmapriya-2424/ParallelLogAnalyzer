#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    std::ofstream file("data/big_logs.txt");

    if (!file.is_open()) {
        std::cout << "Failed to create log file\n";
        return 1;
    }

    std::vector<std::string> ips = {
        "192.168.1.1", "192.168.1.2", "10.0.0.1",
        "10.0.0.2", "172.16.0.1"
    };

    std::vector<int> statusCodes = {200, 200, 200, 404, 500};

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const int TOTAL_LINES = 5'000'000;  // 5 million lines

    for (int i = 0; i < TOTAL_LINES; i++) {
        std::string ip = ips[rand() % ips.size()];
        int status = statusCodes[rand() % statusCodes.size()];
        int responseTime = 50 + rand() % 500;

        file << ip << " GET /api/data "
             << status << " "
             << responseTime << "\n";
    }

    file.close();
    std::cout << "Generated " << TOTAL_LINES << " log lines\n";

    return 0;
}
