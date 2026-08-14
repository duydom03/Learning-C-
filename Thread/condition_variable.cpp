#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void producer()
{
    for(int i = 1; i <= 5; i++)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
            std::cout << "Produced: " << i << '\n';
        } // mutex được mở khóa tại đây

        cv.notify_one();
    }
}

void consumer()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, []{
            return !q.empty();
        });

        int value = q.front();
        q.pop();

        std::cout << "Consumed: " << value << '\n';

        if(value == 5)
            break;
    }
}