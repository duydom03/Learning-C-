#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void task1()
{
    cout << "Thread 1 is running. ID: "
         << this_thread::get_id() << endl;
}

void task2()
{
    cout << "Thread 2 is running. ID: "
         << this_thread::get_id() << endl;
}

int main()
{
    cout << "Main Thread ID: "
         << this_thread::get_id() << endl;

    thread t1(task1);
    thread t2(task2);

    cout << "t1 ID: " << t1.get_id() << endl;
    cout << "t2 ID: " << t2.get_id() << endl;

    if (t1.joinable())
    {
        t1.join();
        cout << "t1 joined" << endl;
    }

    if (t2.joinable())
    {
        t2.detach();
        cout << "t2 detached" << endl;
    }

    // Chờ thread detach chạy xong
    this_thread::sleep_for(chrono::seconds(1));

    cout << "Main thread finished." << endl;

    return 0;
}