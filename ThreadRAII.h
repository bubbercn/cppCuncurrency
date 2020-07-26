#include <thread>

using namespace std;

class ThreadRAII
{
public:
    enum class DtorAction
    {
        Join,
        Detach
    };
    ThreadRAII(thread &&t, DtorAction action) : action(action), t(move(t)) {}
    ~ThreadRAII()
    {
        if (t.joinable())
        {
            if (action == DtorAction::Join)
            {
                t.joinable();
            }
            else
            {
                t.detach();
            }
        }
    }
    thread &get()
    {
        return t;
    }

private:
    DtorAction action;
    thread t;
};