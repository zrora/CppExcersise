#include <iostream>
#include"shared_ptr.hpp"

using namespace ilrd;
using std::cout;
using std::endl;

/*********************************
***Classes to apply tests on******
********************************/
class Amir
{
public:
    Amir(int num = 5);
    int m_number;
};

class AmirDer:public Amir
{
public:
    AmirDer(int num);
    int b;
};

Amir::Amir(int num):
        m_number(num)
{}

 AmirDer::AmirDer(int num):
          Amir(), b(num)
 {}

void function(const SP<Amir>& amir)
{
    std::cout << "/* Amir function  */" << '\n';
}

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

int Sharing(void);
int PointerFunctionalities(void);
int UpCasting(void);

int main()
{
    RunTest(&Sharing);
    RunTest(&PointerFunctionalities);
    RunTest(&UpCasting);

    return 0;
}

//Ctor,  assignment op, cctor
int Sharing()
{
    SP<Amir> ptr1(new Amir(2));
    SP<Amir> ptr2(new Amir(3));
    SP<Amir> ptr3(ptr2);

    ptr1 = ptr2;

    if(ptr1->m_number != 3)
    {
        return 1;
    }

    if(ptr3->m_number != 3)
    {
        return 1;
    }

    return 0;
}

// * , -> , bool()
int PointerFunctionalities()
{
    SP<int> intptr(new int(2));
    SP<Amir> amirptr(new Amir(2));

    if(amirptr->m_number != 2)
    {
        return 1;
    }

    if(*intptr != 2)
    {
        return 1;
    }

    if(!intptr)
    {
        return 1;
    }

    return 0;
}

int UpCasting()
{
    SP<Amir> amirptr(new Amir(2));
    SP<AmirDer> amirderptr(new AmirDer(5));

    //upcasting
    std::cout << "calling function from superclass:" << '\n';
    function(amirptr);

    std::cout << "calling from derived instance:" << '\n';
    function(amirderptr);

    amirptr = amirderptr  ;

    // amirderptr = amirptr  ;//will not compile


    return 0;
}
