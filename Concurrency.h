#ifndef CONCURRENCY
#define CONCURRENCY
#endif // !CONCURRENCY

#include <mutex>
#include <atomic>
#include <functional>
#include <condition_variable>

class ConcurrencyPractice
{
public:
    ConcurrencyPractice() = default;
    ConcurrencyPractice(const int val) : m_length(val), m_numberState(0){}
    ConcurrencyPractice(const ConcurrencyPractice& rhs) = default;
    ConcurrencyPractice(ConcurrencyPractice&& rhs) = default;
    ConcurrencyPractice& operator=(const ConcurrencyPractice& rhs) = default;
    ConcurrencyPractice& operator=(ConcurrencyPractice&& rhs) = default;
    ~ConcurrencyPractice() = default;

    void first(std::function<void()> printFirst);
    void second(std::function<void()> printSecond);
    void third(std::function<void()> printThird);

    void printZero(std::function<void(int)> printNumber);
    void printEven(std::function<void(int)> printNumber);
    void printOdd(std::function<void(int)> printNumber);

private:
    std::mutex m_mtx;
    std::condition_variable m_cond;
    int m_cnt = 1;
    int m_length = 0;
    //std::atomic<size_t> m_numberState;
    int m_numberState = 0;
};

