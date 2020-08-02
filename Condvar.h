#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

mutex aMutex;
bool isDataReady = false;
condition_variable condVar;

void PrintString(const char *s)
{
    unique_lock<mutex> lock(aMutex);
    cout << s << endl;
}

void ProcessData()
{
    PrintString("Waiting for data...");
    {
        unique_lock<mutex> lock(aMutex);
        condVar.wait(lock, [] { return isDataReady; });
        // while (![] { return isDataReady; })
        // {
        //     // <-- time window -->
        //     condVar.wait(lock);
        // }
    }
    PrintString("Got data. Processing data...");
}

void PrepareData()
{
    PrintString("Preparing data...");
    PrintString("Data is ready!");
    {
        unique_lock<mutex> lock(aMutex);
        isDataReady = true;
    }
    condVar.notify_one();
}

void Test()
{
    cout << "Start!" << endl;

    thread dataProcessor(ProcessData);
    thread dataProducer(PrepareData);

    dataProcessor.join();
    dataProducer.join();

    cout << "Finished!" << endl;
}