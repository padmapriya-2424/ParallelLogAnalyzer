#include <string>

void runSingleThread(const std::string&);
void runMultiThread(const std::string&, int);
void runProducerConsumer(const std::string&, int);

int main() {
    std::string path = "data/big_logs.txt";

    runSingleThread(path);
    runMultiThread(path, 4);
    runProducerConsumer(path, 4);

    return 0;
}
