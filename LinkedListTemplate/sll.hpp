/*******************************************************************************
 * 				RD45++ - Singly Linked List - Header file
 ******************************************************************************/
#ifndef _SLL
#define _SLL
#include <cstddef>									//size_t

namespace ilrd
{

	template <class T> class SLL
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

#endif
