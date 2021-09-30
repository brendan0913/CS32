#include "newSet.h"
#include <iostream>
#include <cstdlib>

Set::Set() : m_size(0), max_size(DEFAULT_MAX_ITEMS)
// Create an empty set (i.e., one with no items).
{
	m_array = new ItemType[max_size];
}

Set::Set(int s) : m_size(0), max_size(s) // creates empty set with specific max size
{
	if (s < 0)
	{
		std::cout << "Cannot create a set of negative size." << std::endl;
		exit(1);
	}
	m_array = new ItemType[max_size];
}

Set::Set(const Set& other) : m_size(other.m_size), max_size(other.max_size) // copy constructor
{
	m_array = new ItemType[max_size];
	for (int i = 0; i < m_size; i++)
	{
		m_array[i] = other.m_array[i];
	}
}

Set& Set::operator=(const Set& other)
{
	if (&other == this) // if the variable is assigned to itself, then it does nothing
	{
		return *this;
	}
	Set temp = other;
	swap(temp);

	return *this; // returns reference to itself
}

bool Set::empty() const
// Return true if the set is empty, otherwise false.
{
	return m_size == 0;
}

int Set::size() const
// Return the number of items in the set.
{
	return m_size;
}

bool Set::insert(const ItemType& value)
// Insert value into the set if it is not already present.  Return
// true if the value is actually inserted.  
{
	if (m_size == max_size) { return false; } // Leave the set unchanged
		// and return false if the value was not inserted (perhaps because it
		// was already in the set or because the set has a fixed capacity and
		// is full).
	else
	{
		for (int i = 0; i < size(); i++)
		{
			if (m_array[i] == value)
			{
				return false;
			}
		}
		m_array[size()] = value;
		m_size++;
		return true;
	}
}

bool Set::erase(const ItemType& value)
// Remove the value from the set if it is present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.
{
	bool flag = false;
	for (int i = 0; i < size(); i++)
	{
		if (flag)
		{
			m_array[i - 1] = m_array[i];
		}
		else if (m_array[i] == value) { flag = true; }
	}
	if (flag) { m_size--; }
	if (flag == false) { return false; }
	return true;
}


bool Set::contains(const ItemType& value) const
// Return true if the value is in the set, otherwise false.
{
	for (int i = 0; i < size(); i++)
	{
		if (m_array[i] == value)
		{
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
// If 0 <= i < size(), copy into value the item in the set that is
// strictly greater than exactly i items in the set and return true.
// Otherwise, leave value unchanged and return false.
{
	if (i < 0 || i >= size())
	{
		return false;
	}
	int count = 0;
	for (int k = 0; k < size(); k++)
	{
		for (int j = 0; j < size(); j++)
		{
			if (m_array[k] > m_array[j])
			{
				count++;
			}
		}
		if (count == i)
		{
			value = m_array[k];
			return true;
		}
		count = 0;
	}
	return false;
}

void Set::swap(Set& other)
// Exchange the contents of this set with the other one.
{
	ItemType* temp = m_array;
	m_array = other.m_array;
	other.m_array = temp;

	int tempsize = m_size;
	m_size = other.m_size;
	other.m_size = tempsize;

	int tempmsize = max_size;
	max_size = other.max_size;
	other.max_size = tempmsize;
}

Set::~Set() // destructor
{
	delete[] m_array;
}