#include <iostream>     // cout
#include <stdlib.h>     //  exit()
#include "sll.cpp"

using namespace std;
using namespace ilrd;

inline void RunTest(int (*test_func)(void))
{
    int result = test_func();
    if (0 == result)
    {
        cout << "[\033[0;92mOK\033[0m]" << endl;
    }
    else
    {
        cout << "[\033[0;91mFAIL!\033[0m]" << endl;
        exit(result);
    }
}

int Flow1(void);

int main()
{
    RunTest(&Flow1);
    return 0;
}

int Flow1(void)
{
    size_t i = 0;
    const int count = 5;

    SLL<int> linked_list;
    // SLL linked_list2 (linked_list);
    for (; i < count; i++)
    {
       linked_list.PushFront(i);
       std::cout << "size = " <<linked_list.Size()<<std::endl;
    }

    if (linked_list.Size() != count)
    {
        return 1;
    }

    for (i = 0; i < count; i++)
    {
        linked_list.PopFront();
        std::cout << "size = " <<linked_list.Size()<<std::endl;

    }
    if (!linked_list.IsEmpty())
    {
        return 1;
    }


    return 0;
}
