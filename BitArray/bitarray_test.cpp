//#include <cstring>
#include <iostream>           //  cout
#include "bitarray.hpp"

using  std::cout;
using  std::cin;
using  std::endl;
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
    }
}

int Create(void);
int Operators(void);
int Functions(void);
int Proxy(void);

int main()
{
    RunTest(&Create);
    RunTest(&Operators);
    RunTest(&Functions);
    RunTest(&Proxy);

    return 0;
}

//Ctor, Set, Get, assignment op
int Create(void)
{
    BitArray<64> a;
    BitArray<64> b;
    BitArray<64> c;

                        //BitArray<65>d;
                        //a=d; //will not compile

    a.Set(0, true);

    if(! a.Get(0))
    {
        return 1;
    }
    b = a;
    if(! b.Get(0))
    {
        return 1;
    }

    a = b = c;          //zero all instances

    if ((a.Get(0) || b.Get(0) || c.Get(0)))
    {
        return 1;
    }

    return 0;

}
//|=,&=,^=,==,!=
int Operators()
{
    BitArray<65> a;
    BitArray<65> b;

    a.Set(0, true);
    b.Set(1, true);

    a |= b;         //(binary)1 | (binary)10  == (binary)11== 3=(dec)

    if(! (a.Get(0) && a.Get(1)))
    {
        return 1;
    }

    //a=7void
    a.Set(2, true);
    //turns off second bit
    a ^= b;
    if(! (a.Get(0) && a.Get(2) && (! a.Get(1) )))
    {
        return 1;
    }

    //will set all a bits off.
    a &= b;
    if((a.Get(0) && a.Get(2) && a.Get(1)))
    {
        return 1;
    }

    if (!(a == b))
    {
        return 1;
    }

    if(!(a != a))
    {
        return 1;
    }



    return 0;
}
int Functions()
{
    BitArray<65> a;
    BitArray<65> b;

    a.FlipAll();

    for (size_t i = 0; i < 65; i++)
    {
        if(!a.Get(i))
        {
            return 1;
        }
    }
    //check garbage was cleaned
    for (size_t i = 65; i < 128; i++)
    {
        if(a.Get(i))
        {
            return 1;
        }
    }

    b.Set(1, true);
    b.FlipBit(0);//turns b from 2 to 3
    if(!b.Get(0))
    {
        return 1;
    }

    if (b.CountTrueBits() != 2)
    {
        return 1;
    }

    //shift left operator
    BitArray<192> c;
    c.Set(63, true);
    c.Set(127, true);
    c.Set(191, true);
    //c == 0000...1,000..1,000...1
    c<<=1;
    //c == 00000000, 10000000, 1000000
    if(!(c.Get(64) && c.Get(128) ))
    {
        return 0;
    }

    return 0;
}

int Proxy()
{
    BitArray<65> a;
    a.Set(1, true);

    if(a[1] == false)
    {
        return 1;
    }

    if(false == a[1] )
    {
        return 1;
    }

    a[2] = true;

    if((a[1] != a[2]))
    {
        return 1;
    }
    return 0;
}
