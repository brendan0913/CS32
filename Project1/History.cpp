#include "History.h"
#include "globals.h"

#include <iostream>

History::History(int nRows, int nCols)
{
	m_Rows = nRows;
	m_Cols = nCols;
	for (int r = 1; r <= m_Rows; r++) // constructs the initial grid of all dots, since no posion has been dropped yet
	{
		for (int c = 1; c <= m_Cols; c++)
		{
			m_Grid[r - 1][c - 1] = '.';
		}
	}
}

bool History::record(int r, int c)
{
	if (r <= 0 || c <= 0 || r > m_Rows || c > m_Cols) // if the rows or columns are out of bounds or out of the restraints made by the History constructor, returns false
	{
		return false;
	}
	else if (m_Grid[r - 1][c - 1] == '.') // if it is a dot, manually changes it to an 'A'
	{
		m_Grid[r - 1][c - 1] = 'A';
	}
	else if (m_Grid[r - 1][c - 1] == 'Z') // if it is a 'Z', keeps it as a 'Z'
	{
		m_Grid[r - 1][c - 1] = 'Z';
	}
	else
	{
		(m_Grid[r - 1][c - 1])++; // otherwise, changes the ASCII value of the character by one to show the next letter
	}
	return true;
}

void History::display() const
{
	clearScreen();
	int r, c;

	for (r = 1; r <= m_Rows; r++) // displays the new history grid
	{
		for (c = 1; c <= m_Cols; c++)
		{
			std::cout << m_Grid[r - 1][c - 1];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
