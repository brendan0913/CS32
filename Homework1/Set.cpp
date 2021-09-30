#include "Set.h"

Set::Set() : m_size(0)
// Create an empty set (i.e., one with no items).
{
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
	if (m_size == DEFAULT_MAX_ITEMS) { return false; } // Leave the set unchanged
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
	ItemType temp;
	int longest = m_size;
	if (m_size < other.m_size)
		longest = other.m_size;
	for (int i = 0; i < longest; i++)
	{
		temp = m_array[i];
		m_array[i] = other.m_array[i];
		other.m_array[i] = temp;

	}
	int tempsize = m_size;
	m_size = other.m_size;
	other.m_size = tempsize;
}