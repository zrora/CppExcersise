/***************************************************************************
                SharedPointer
                Amir Zror
                28/6/2018

*****************************************************************************/
#ifndef __SHARED_PTR_HPP__
#define __SHARED_PTR_HPP__

#include <cstring>

namespace ilrd
{

template<class T>
class SP
{
public:
    explicit SP(T* pointer = 0);
    SP(const SP<T>& other);               //cctor
    ~SP();

    SP& operator=(const SP& pointer);// sp1 = sp2;
    T& operator*();
    T* operator->();
    operator bool() const; // cast to boolian

    // template function for
    template<class newType>
    SP(const SP<newType>& other):m_sp(other.m_sp),m_counter(other.m_counter)
    {
        *m_counter = *m_counter + 1;
    }

    template<class newType>
    friend class SP;

private:
    T* m_sp;
    int* m_counter;
} ; //class sp

/*********************************
****Smartpointer template mathods***
*********************************/
template<class T>
SP<T>::SP(T* pointer): m_sp(pointer), m_counter(new int(1))
{}

template<class T>
SP<T>::~SP()
{
    *m_counter = *m_counter - 1;

    if(*m_counter == 0)
    {
        delete m_sp;
        m_sp = 0;

        delete m_counter;
        m_counter = 0;
    }
}

template<class T>
SP<T>& SP<T>::operator=(const SP& other)
{
    *other.m_counter = *other.m_counter + 1;// to prevent self assignment
    *m_counter = *m_counter - 1;

    if(*m_counter == 0)
    {
        delete m_sp;
        delete m_counter ;
    }

    m_sp = other.m_sp;
    m_counter = other.m_counter;


    return *this;
}

template<class T>
SP<T>::SP(const SP<T>& pointer): m_sp(pointer.m_sp),m_counter(pointer.m_counter)          //cctor
{
    *m_counter = *m_counter + 1;
}

template<class T>
T& SP<T>::operator*()
{
    return *m_sp;
}

template<class T>
T* SP<T>::operator->()
{
    return m_sp;
}

template<class T>
SP<T>::operator bool() const
{
    return m_sp;
}

}//namespace ilrd
#endif
