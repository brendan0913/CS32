#include "Vampire.h"
#include "globals.h"
#include "Arena.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        std::cout << "***** A vampire must be created in some Arena!" << std::endl;
        std::exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        std::cout << "***** Vampire created with invalid coordinates (" << r << ","
            << c << ")!" << std::endl;
        std::exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = INITIAL_VAMPIRE_HEALTH;
    m_idleTurnsRemaining = 0;
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
    return m_col;
}

bool Vampire::isDead() const
{
    return m_health == 0;
}

void Vampire::move()
{
    if (m_idleTurnsRemaining > 0)
    {
        m_idleTurnsRemaining--;
        return;
    }

    // Attempt to move in a random direction; if can't move, don't move
    if (attemptMove(*m_arena, randInt(0, NUMDIRS - 1), m_row, m_col))
    {
        if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        {
            m_arena->setCellStatus(m_row, m_col, EMPTY);
            m_health--;
        }
    }

    if (m_health < INITIAL_VAMPIRE_HEALTH)
        m_idleTurnsRemaining = POISONED_IDLE_TIME;
}