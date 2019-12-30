#include "rcstring.hpp"
#include <cstring>
#include <new>
#include <iostream>


namespace ilrd
{


class RCString::RC
{
public:
    explicit RC(const char* str);
    ~RC();
    RC(const RC& rcse);

    void Inc();
    void Dec();
    int Counter();
    char* GetString();

private:
    int m_counter;//will be deleted
};


RCString::CharProxy::CharProxy(RCString& rcs, int index): m_rcs(rcs), m_index(index)
{}

RCString::RCString(const char* str): m_rc(static_cast<RC*>(::operator new (sizeof(int)+ strlen(str) + 1)))
{
    new (m_rc) RC(str);
    m_rc++;

}

RCString::RC::RC(const char* str):m_counter(1)
{
    strcpy(reinterpret_cast<char*>(this + 1), str);
}

RCString::RCString(const RCString &str):m_rc(str.m_rc)
{
    m_rc->Inc();
}

void RCString::RC::Inc()
{
    ((this - 1)->m_counter)++;
}

void RCString::RC::Dec()
{
    --((this - 1)->m_counter);
}

int RCString::RC::Counter()
{
    return (*reinterpret_cast<int*>(this - 1));
}

char* RCString::RC::GetString()
{
    return (reinterpret_cast<char*>(this )) ;
}

RCString::~RCString()
{
    m_rc->Dec();
    std::cout << m_rc->Counter() << '\n';
    if(m_rc->Counter() == 0)
    {
    //    std::cout << m_rc->GetString() << '\n';
        // ::operator delete ((m_rc - 1));
        delete ((m_rc - 1));
    }
}

RCString::RC::~RC(){}

const RCString& RCString::operator=(const RCString& rcs)
{
//check if were not pointing to the same data anyway
    if(m_rc != rcs.m_rc)
    {
        //reduce counter from this .
        m_rc->Dec();
        //if there are no more items....
        if(m_rc->Counter() == 0)
        {
            ::operator delete (m_rc - 1);// delete everything
        }

        m_rc = rcs.m_rc;//point to same string

        rcs.m_rc->Inc();      //increse counter to rcs.
    }

    return rcs;
}

const RCString& RCString::operator=(const char* str)
{
    m_rc->Dec();

    if(m_rc->Counter() == 0)
    {
        delete(m_rc - 1);
    }

    m_rc = (static_cast<RC*>(::operator new (sizeof(int)+ strlen(str) + 1)));
    new (m_rc) RC(str);      //spread the data

    m_rc ++;
    return *this;
}

void RCString::Set(char c, int index)
{
    if(m_rc->Counter() > 1)
    {
        m_rc->Dec();

        char* temp = reinterpret_cast<char*>(m_rc);
        int size = strlen(temp);
        m_rc -= 1;

        m_rc = (static_cast<RC*>(::operator new (sizeof(int)+ size + 1)));

        new (m_rc) RC(temp);      //spread the data

        m_rc += 1;
        // m_rc = new RC(m_rc->GetString());
    }
    m_rc->GetString()[index] = c;
}

char RCString::Get(int index)
{
    return m_rc->GetString()[index];
}

RCString::CharProxy RCString::operator[](size_t index)
{
    return CharProxy (*this, index);
}

char RCString::operator[](size_t index)const
{
    return m_rc->GetString()[index];
}

RCString::CharProxy::operator char()const
{
    return m_rcs.m_rc->GetString()[m_index];
}

RCString::CharProxy RCString::CharProxy::operator=(char c)
{
    m_rcs.Set(c, m_index);
    return *this;
}

RCString::CharProxy RCString::CharProxy::operator=(CharProxy c)
{
    *this = static_cast<char>(c);
    return *this;
}




}//ilrd
