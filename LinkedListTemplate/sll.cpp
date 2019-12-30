#ifndef __SLL
#define __SLL

#include<iostream>
#include <climits>

namespace ilrd
{

	template <class T>
	class SLL
	{

	public:
		explicit SLL();									//Default CTOR
	    ~SLL();											//DTOR
		void PushFront(T data);
		void PopFront();
		T Front() const;								//Return MIN_INT when list is empty
		size_t Size() const;
		bool IsEmpty() const;

	private:
	    SLL(const SLL&);								//Uncopyable CCTOR
	    const SLL& operator=(const SLL&);				//Uncopyable Assignment Operator

	    struct Node
	    {
	    	explicit Node(T data, Node* next = 0);	//CTOR
	    	const T m_data;
	    	Node*     m_next;
	    };

		Node m_head;									//Head stub
	};
};

using namespace std ;
using namespace ilrd;

/*
*/
template<class T>
SLL<T>::~SLL()
{
    while (!IsEmpty())
    {
       PopFront();
    }
}

template<class T>
SLL<T>::SLL():m_head(0)
{}

template<class T>
SLL<T>::Node::Node(T data, Node* next):m_data(data), m_next(next)
{}

template<class T> void SLL<T>::PushFront(T data)
{
    Node* new_node = new Node(data, m_head.m_next);
    m_head.m_next = new_node;
}

template<class T> bool SLL<T>::IsEmpty() const
{
    return !Size();
}

template<class T>
T SLL<T>::Front() const								//Return INT_MIN when list is empty
{
    if (IsEmpty())
    {
        return INT_MIN;
    }
    return m_head.m_next->m_data;
}

template<class T> void SLL<T>::PopFront()
{
    Node* temp = m_head.m_next;

    if (temp)//temp address != 0
    {
        m_head.m_next = m_head.m_next->m_next;
        delete(temp);
    }
}

template<class T> size_t SLL<T>::Size() const
{
    size_t counter = 0;
    Node* runner = m_head.m_next;

    while (runner)
    {
        counter ++;
        runner = runner->m_next;
    }
    return counter;
}
