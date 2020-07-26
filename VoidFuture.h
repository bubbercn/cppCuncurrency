#include <iostream>
#include <thread>
#include <future>
using namespace std;

mutex aMutex;
promise<void> data;

void PrintString(const char* s)
{
    unique_lock<mutex> lock(aMutex);
    cout << s << endl;
}

void ProcessData()
{
    PrintString("Waiting for data...");
    {
        try
        {
            ::data.get_future().get();
            PrintString("Got data. Processing data...");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            PrintString("Data is lost!");
        }
    }
}

void PrepareData()
{
    try
    {
        PrintString("Preparing data...");
        //std::string().at(1);
        PrintString("Data is ready!");
    }
    catch(const std::exception& e)
    {
        PrintString(e.what());
        ::data.set_exception(current_exception());
        return;
    }
    
    ::data.set_value();
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