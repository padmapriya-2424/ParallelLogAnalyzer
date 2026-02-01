### Parallel Log Analyzer in C++

A high-performance log analysis engine built in C++ to process millions of log records efficiently using multithreading and producer–consumer architecture.

### Problem Statement
Modern backend systems generate huge log files (GBs). Processing them sequentially is slow and under-utilizes multi-core CPUs.

### Features Implemented

✔ Count **4xx and 5xx error codes**  
✔ Compute **average response time**  
✔ Handle **millions of log lines**  
✔ Compare **three architectures**:
- Single-threaded  
- Naive multi-threaded (file chunking)  
- Optimized producer–consumer model  

✔ Measure and benchmark performance

### Log Format
  IP METHOD ENDPOINT STATUS_CODE RESPONSE_TIME(ms)
Example:
  192.168.1.2 GET /api/data 500 340

## Architecture

### 1️⃣ Single-Threaded (Baseline)
- Reads file line by line
- Processes logs sequentially
- Used as a performance baseline

### 2️⃣ Naive Multi-Threaded (I/O Bound)
- Splits file into byte chunks
- Each thread reads its own file segment
- Slower due to **disk I/O contention**

### 3️⃣ Producer–Consumer (Optimized ✅)
### Technologies Used
 
- **C++17**
- `std::thread`
- `std::mutex`
- `std::condition_variable`
- STL containers (`vector`, `unordered_map`, `queue`)
- File I/O (`ifstream`, `ofstream`)
- Performance benchmarking (`std::chrono`)
- OS concepts:
  - I/O-bound vs CPU-bound workloads
  - Synchronization
  - Thread coordination

---

## Performance Results (5 Million Log Lines)

| Approach | Time Taken |
|--------|-----------|
| Single-threaded | ~7.8 sec |
| Naive multi-threaded | ~10.2 sec |
| **Producer–Consumer** | **~3.6 sec** 🚀 |

✔ ~2× speedup over single-thread  
✔ Correct results across all versions

---

### Key Learnings
  Parallelism is not always faster
  I/O-bound workloads need architectural changes

### Author
  Padmapriya C
