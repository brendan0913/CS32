#ifndef SSNSet_H
#define SSNSet_H

#include "Set.h"

class SSNSet
{
public:
    SSNSet();          // Create an empty SSN set.

    bool add(unsigned long ssn);
    // Add an SSN to the SSNSet.  Return true if and only if the SSN
    // was actually added.

    int size() const;  // Return the number of SSNs in the SSNSet.

    void print() const;
    // Write to cout every SSN in the SSNSet exactly once, one per
    // line.  Write no other text.

private:
    Set m_set;
};

#endif // !SSNSet_H
