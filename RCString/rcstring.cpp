#include "rcstring.hpp"
#include <cstring>

namespace ilrd
{


class RCString::RC
{
public:
    RC(const char* str);
    ~RC();
    RC(const RC& rcse);

    void Inc();
    void Dec();
    int Counter();
    char* GetString();

private:
    int m_counter;
    char* m_string;
};


RCString::CharProxy::CharProxy(RCString& rcs, int index): m_rcs(rcs), m_index(index)
{}

RCString::RCString(const char* str): m_rc(new (std::nothrow) RC(str))
{}

RCString::RC::RC(const char* str):m_counter(1), m_string(new (std::nothrow) char[strlen(str) + 1])
{
    strcpy(m_string, str);
}

RCString::RCString(const RCString &str):m_rc(str.m_rc)
{
    m_rc->Inc();
}

void RCString::RC::Inc()
{
    m_counter++;
}

void RCString::RC::Dec()
{
    m_counter--;
}

int RCString::RC::Counter()
{
    return m_counter;
}

char* RCString::RC::GetString()
{
    return m_string;
}

RCString::~RCString()
{
    m_rc->Dec();
    if(m_rc->Counter() == 0)
    {
        // delete(m_rc->GetString());
        delete m_rc;
        std::cout << "rcstring dtor" << '\n';
    }

}

RCString::RC::~RC()
{
    delete [] m_string;
    std::cout << "rc dtor" << '\n';
}

const RCString& RCString::operator=(const RCString& rcs)
{
//check if were not pointing to the same data anyway
    if(m_rc->GetString() != rcs.m_rc->GetString())
    {
        //reduce counter from this .
        m_rc->Dec();
        //if there are no more items....
        if(m_rc->Counter() == 0)
        {
            delete(m_rc);
        }

        this->m_rc = rcs.m_rc;//point to same string
        rcs.m_rc->Inc();      //increse counter to rcs.
    }

    return rcs;
}

const RCString& RCString::operator=(const char* str)
{

    m_rc->Dec();

    if(m_rc->Counter() == 0)
    {
        delete(m_rc);
    }

    this->m_rc = new RCString::RC(str);
    return *this;

}

void RCString::Set(char c, int index)
{
    if(m_rc->Counter() > 1)
    {
        m_rc->Dec();
        m_rc = new RC(m_rc->GetString());
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
