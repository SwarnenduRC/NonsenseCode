#include "Concurrency.h"

#include <thread>
#include <chrono>

using namespace std;

void ConcurrencyPractice::first(std::function<void()> printFirst)
{
    unique_lock<mutex> lock(m_mtx);
    m_cond.wait(lock, [this] { return m_cnt == 1; });
    printFirst();
    m_cnt = 2;
    m_cond.notify_all();
}

void ConcurrencyPractice::second(std::function<void()> printSecond)
{
    unique_lock<mutex> lock(m_mtx);
    m_cond.wait(lock, [this] { return m_cnt == 2; });
    printSecond();
    m_cnt = 3;
    m_cond.notify_all();
}

void ConcurrencyPractice::third(std::function<void()> printThird)
{
    unique_lock<mutex> lock(m_mtx);
    m_cond.wait(lock, [this] { return m_cnt == 3; });
    printThird();
    m_cnt = 1;
    m_cond.notify_all();
}

void ConcurrencyPractice::printZero(std::function<void(int)> printNumber)
{
    if (m_length)
    {
        for (auto idx = 1; idx <= m_length; ++idx)
        {
            unique_lock<mutex> lock(m_mtx);
            m_cond.wait(lock, [this] { return m_numberState == 0; });
            /*while (m_numberState.load(memory_order::memory_order_acquire) != 0)
                this_thread::yield();*/
            printNumber(0);
            if (idx & 1)
                m_numberState = 1;
            else
                m_numberState = 2;
            m_cond.notify_all();
            /*if (idx & 1)
                m_numberState.store(1, memory_order::memory_order_release);
            else
                m_numberState.store(2, memory_order::memory_order_release);*/
        }
    }
}

void ConcurrencyPractice::printEven(std::function<void(int)> printNumber)
{
    if (m_length)
    {
        for (auto idx = 2; idx <= m_length; idx += 2)
        {
            unique_lock<mutex> lock(m_mtx);
            m_cond.wait(lock, [this] { return m_numberState == 2; });
            /*while (m_numberState.load(memory_order::memory_order_acquire) != 2)
                this_thread::yield();*/
            printNumber(idx);
            m_numberState = 0;
            m_cond.notify_all();
            /*m_numberState.store(0, memory_order::memory_order_release);*/
        }
    }
}

void ConcurrencyPractice::printOdd(std::function<void(int)> printNumber)
{
    if (m_length)
    {
        for (auto idx = 1; idx <= m_length; idx += 2)
        {
            unique_lock<mutex> lock(m_mtx);
            m_cond.wait(lock, [this] { return m_numberState == 1; });
            /*while (m_numberState.load(memory_order::memory_order_acquire) != 1)
                this_thread::yield();*/

            printNumber(idx);
            m_numberState = 0;
            m_cond.notify_all();
            /*m_numberState.store(0, memory_order::memory_order_release);*/
        }
    }
}

