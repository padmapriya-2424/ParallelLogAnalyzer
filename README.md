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

## Observed Performance Results (5 Million Log Lines)

The following values came from a local run using the generated 5-million-line
file, four threads/workers, and the current implementation. Timings are
machine-, compiler-, and filesystem-dependent, so they are representative
rather than guaranteed.

| Approach | Time Taken |
|--------|-----------|
| Single-threaded | 1.63 sec |
| Naive multi-threaded | 1.50 sec |
| **Producer–Consumer** | **0.58 sec** |

The same run produced 1,000,596 4xx errors, 999,368 5xx errors, and an
average response time of 298.53 ms in all three approaches.

The producer-consumer run was approximately 2.8× faster than the
single-threaded run in this measurement.  
✔ Correct results across all versions

---

### Key Learnings
  Parallelism is not always faster
  I/O-bound workloads need architectural changes

### Author
  Padmapriya C
