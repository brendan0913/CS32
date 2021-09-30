#include "Arena.h"
#include "globals.h"
#include "Vampire.h"
#include "Player.h"
#include "History.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols) : m_history(nRows, nCols) // initialzier list for the m_history data member, sets the history grid to have the same number of rows and cols as the
                                                             // arena grid
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        std::cout << "***** Arena created with invalid size " << nRows << " by "
            << nCols << "!" << std::endl;
        std::exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nVampires = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    for (int k = 0; k < m_nVampires; k++)
        delete m_vampires[k];
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::vampireCount() const
{
    return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r - 1][c - 1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nVampires; k++)
    {
        Vampire* vp = m_vampires[k];
        if (vp->row() == r && vp->col() == c)
            count++;
    }
    return count;
}

void Arena::display(std::string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill displayGrid with dots (empty) and stars (poisoned blood vials)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

    // Indicate each vampire's position
    for (int k = 0; k < m_nVampires; k++)
    {
        const Vampire* vp = m_vampires[k];
        char& gridChar = displayGrid[vp->row() - 1][vp->col() - 1];
        switch (gridChar)
        {
        case '.':  gridChar = 'V'; break;
        case 'V':  gridChar = '2'; break;
        case '9':  break;
        default:   gridChar++; break;  // '2' through '8'
        }
    }

    // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            std::cout << displayGrid[r - 1][c - 1];
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Write message, vampire, and player info
    if (msg != "")
        std::cout << msg << std::endl;
    std::cout << "There are " << vampireCount() << " vampires remaining." << std::endl;
    if (m_player == nullptr)
        std::cout << "There is no player!" << std::endl;
    else if (m_player->isDead())
        std::cout << "The player is dead." << std::endl;
    std::cout << m_turns << " turns have been taken." << std::endl;
}

History& Arena::history()
{
    return m_history;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    if (status == HAS_POISON && getCellStatus(r, c) == EMPTY) // if the poison is dropped and the cell was previously empty, the history record function is called
    { // calling it only when the cell was previously empty ensures that record is called only after a vampire eats the vial and a new poison is dropped
        m_history.record(r, c);
    }
    m_grid[r - 1][c - 1] = status;
}

bool Arena::addVampire(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a vampire on a spot with a poisoned blood vial
    if (getCellStatus(r, c) != EMPTY)
        return false;

    // Don't add a vampire on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    if (m_nVampires == MAXVAMPIRES)
        return false;
    m_vampires[m_nVampires] = new Vampire(this, r, c);
    m_nVampires++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a vampire
    if (numberOfVampiresAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveVampires()
{
    // Move all vampires
    for (int k = m_nVampires - 1; k >= 0; k--)
    {
        Vampire* vp = m_vampires[k];
        vp->move();

        if (m_player != nullptr &&
            vp->row() == m_player->row() && vp->col() == m_player->col())
            m_player->setDead();

        if (vp->isDead())
        {
            delete vp;

            // The order of Vampire pointers in the m_vampires array is
            // irrelevant, so it's easiest to move the last pointer to
            // replace the one pointing to the now-deleted vampire.  Since
            // we are traversing the array from last to first, we know this
            // last pointer does not point to a dead vampire.

            m_vampires[k] = m_vampires[m_nVampires - 1];
            m_nVampires--;
        }
    }

    // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c, std::string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        std::cout << "***** " << "Invalid arena position (" << r << ","
            << c << ") in call to " << functionName << std::endl;
        std::exit(1);
    }
}