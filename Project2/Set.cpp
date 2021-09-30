#include "Set.h"
#include <iostream>

Set::Set() : m_size(0) // creates empty set, circular doubly linked list
{
	head = new Node;
	head->next = head;
	head->prev = head;
}

Set::Set(const Set& other) : m_size(other.m_size)
{
	if (other.head->next == other.head && other.head->prev == other.head) // if the set is empty
	{
		head = new Node; // creates new circular empty set if the other set is also empty
		head->prev = head;
		head->next = head;
	}
	else // non empty list
	{
		head = new Node;
		Node* thisCurrent = head;
		Node* otherCurrent = other.head->next; // sets otherCurrent to the node after head since the while loop condition is for it to not be head
		while (otherCurrent != other.head) // while not going back to the front of the list, prevents loop from coming all the way back to head
		{
			thisCurrent->next = new Node; // starts this current at next since otherCurrent is at head->next
			thisCurrent->next->val = otherCurrent->val;
			thisCurrent->next->prev = thisCurrent;
			thisCurrent->next->next = head; // the next node after this current next points to head

			thisCurrent = thisCurrent->next;
			otherCurrent = otherCurrent->next;
		}
		head->prev = thisCurrent; // points head's previous arrow to the last node that is not head, so it is circular
	}
}

Set& Set::operator=(const Set& rhs)
{
	if (this != &rhs)
	{
		Set temp(rhs); // copy swap approach
		swap(temp);
	}
	return *this;
}

bool Set::empty() const
{
	return size() == 0;
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{
	if (contains(value)) // if the value is already in the set
	{
		return false;
	}
	// if not in the set
	Node* p = new Node; // creates new Node* ptr
	p->val = value; // set val = value

	if (head->next == head && head->prev == head) // if the set is empty
	{
		p->next = head; //since there is no tail, p points directly to head
		p->prev = head;
		head->next = p;
		head->prev = p; // since head->prev is basically the tail, it points to p
		m_size++;
		return true;
	}

	Node* now = head->next; //starts at the node after head in order to enter the loop, which must end when the current node is head to only go through the list once
	while (now != head)
	{
		if (value < now->val) // if the inserted value is less than the current node's value
		{
			p->next = now; // the new node that has value points to the current node
			p->prev = now->prev; // the new node's previous arrow points to where the current node's previous was pointing
			p->prev->next = p; // the node before the new node now points to the new node
			p->next->prev = p; // the previous of the node after the new node points back to the new node
			m_size++;

			return true;
		}
		now = now->next; // keeps traversing through the list
	}
	if (value > head->prev->val) // the new value must be inserted at the end of the list if it is greater than all previous elements, so if it is greater than the last node (which it should be)
	{
		p->next = head; // points the new node p next to head
		p->prev = head->prev; // points the new node's previous arrow to where head previous was previously pointing (the last node)
		head->prev->next = p; // sets the last node's next to p
		head->prev = p; // points head previous to the new last node
		m_size++;
		return true;
	}
	return false;
}

bool Set::erase(const ItemType& value)
{
	if (head->next == head && head->prev == head) // if it is empty, there is nothing to erase
	{
		return false;
	}
	if (!contains(value)) // if it doesnt contain the desired value to erase
	{
		return false;
	}
	else
	{
		Node* p = head->next; // sets p to the first node after head
		while (p != head)
		{
			if (p->val == value) // if it finds the value, the value to delete is now at node p
			{
				break;
			}
			p = p->next;
		}
		Node* getrekt = p;
		p->prev->next = p->next; // sets the previous's node's next to where the to be deleted node is pointing next
		p->next->prev = p->prev; // sets the next node's previous to where the to be deleted node is pointing behind it
		delete getrekt; // deletes the node
		m_size--;
		return true;
	}
}

bool Set::contains(const ItemType& value) const
{
	Node* p = head->next; // first node after head
	while (p != head)
	{
		if (p->val == value)
		{
			return true;
		}
		p = p->next; // traverses through list
	}
	return false;
}

bool Set::get(int pos, ItemType& value) const
{
	Node* p = head->next;
	int count = 0;
	while (p != head)
	{
		if (count == pos) // since the list is already sorted, the number of items the value is greater than is the same as its position in the list
		{
			value = p->val;
			return true;
		}
		p = p->next;
		count++;
	}
	return false;
}

void Set::dump() const
{
	Node* p = head->next;
	while (p != head)
	{
		std::cerr << p->val << std::endl;
		p = p->next;
	}
	std::cerr << '\n';
	p = head->prev;
	while (p != head)
	{
		std::cerr << p->val << std::endl;
		p = p->prev;
	}
	std::cerr << '\n';
}

void Set::swap(Set& other)
{
	Node* tempData = head; // swaps the heads
	head = other.head;
	other.head = tempData;

	int tempSize = m_size; // swaps the sizes 
	m_size = other.m_size;
	other.m_size = tempSize;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set temp = s1; // makes temp = set in case of aliasing
	for (int i = 0; i < s2.size(); i++)
	{
		ItemType x;
		s2.get(i, x); // goes through the entire list of s2
		temp.insert(x); // since insert checks if the set contains the value and does not insert it if it is already there, only one copy of each element will be in the result
	}
	result = temp;
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set temp = s1; // makes temp = set in case of aliasing
	for (int i = 0; i < s2.size(); i++)
	{
		ItemType x;
		s2.get(i, x);
		temp.erase(x); // since erase does not erase if the value is already there, it only erases one copy of s2's values that are in temp, which is s1
	}
	result = temp;
}

Set::~Set()
{
	Node* p = head->next; 
	while (p != head)
	{
		Node *n = p->next; // makes node n equal to the second node after head
		delete p; //deletes the node prior to n
		p = n; // points p to where n is pointing
	}
	delete head;
}