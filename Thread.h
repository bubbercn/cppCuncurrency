#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void foo()
{
    this_thread::sleep_for(chrono::seconds(1));
}

void Test()
{
    thread t1(foo);

    thread t2([]() {
        foo();
    });

    cout << "t1's id: " << t1.get_id() << endl;
    cout << "t2's id: " << t2.get_id() << endl;
}