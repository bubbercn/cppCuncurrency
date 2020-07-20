#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

mutex aMutex;
bool isDataReady = false;
condition_variable condVar;

void PrintString(const char* s)
{
    unique_lock<mutex> lock(aMutex);
    cout << s << endl;
}

void ProcessData()
{
    PrintString("Waiting for data...");
    {
        unique_lock<mutex> lock(aMutex);
        while (![] { return isDataReady; }()) 
        {
            // <-- time window -->
            condVar.wait(lock);
        }
    }
    PrintString("Got data. Processing data...");
}

void PrepareData()
{
    PrintString("Preparing data...");
    {
        unique_lock<mutex> lock(aMutex);
        isDataReady = true;
    }
    PrintString("Data is ready!");
    condVar.notify_one();
}

int main()
{
    cout << "Start!" << endl;

    thread dataProcessor(ProcessData);
    thread dataProducer(PrepareData);

    // dataProcessor.join();
    // dataProducer.join();

    dataProcessor.detach();
    dataProducer.detach();

    cout << "Finished!" << endl;
}