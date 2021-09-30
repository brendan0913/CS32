#include "SSNSet.h"
#include "Set.h"

#include <iostream>

SSNSet::SSNSet()
// Create an empty SSN set.
{

}

bool SSNSet::add(unsigned long ssn)
// Add an SSN to the SSNSet.  Return true if and only if the SSN
 // was actually added.
{
	if (m_set.contains(ssn))
	{
		return false;
	}
	else
	{
		m_set.insert(ssn);
		return true;
	}
}

int SSNSet::size() const // Return the number of SSNs in the SSNSet.
{
	return m_set.size();
}

void SSNSet::print() const
// Write to cout every SSN in the SSNSet exactly once, one per
// line.  Write no other text.
{
	unsigned long u;
	for (int i = 0; i < m_set.size(); i++)
	{
		m_set.get(i, u);
		std::cout << u << std::endl;
	}
}
