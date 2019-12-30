#include <iostream>

namespace ilrd
{
class RCString;//fwd decleration
//std::ostream& operator<<(std::ostream& os, const RCString& rcs);

class RCString
{
private:
    class RC; //forward decleration;

public:
    class CharProxy;

    explicit RCString(const char* str="");
    ~RCString();                 //dtor
    RCString(const RCString& rcs);//cctor

    const RCString& operator=(const RCString& rcs);//rcs1 = rcs2
    const RCString& operator=(const char* str);//rcs1 = "rcstring"

    RCString::CharProxy operator[](size_t index);//rcs[2] = 'a'

    char operator[](size_t index)const;//


private:
//    int counter;
    void Set(char c, int index);
    char Get(int index);

     RC *m_rc;
};

class RCString::CharProxy
{
public:
    explicit CharProxy(RCString& rcs, int index);
    operator char()const;
    RCString::CharProxy operator=(char c);
    CharProxy operator=(CharProxy c);


private:
    RCString& m_rcs;
    int m_index;
};


}//namespace ilrd
