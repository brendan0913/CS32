#include <queue>
#include <iostream>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    queue<Coord> coordQueue;
    coordQueue.push(Coord(sr, sc));
    maze[sr][sc] = '#';
    while (!coordQueue.empty())
    {
        int r = coordQueue.front().r();
        int c = coordQueue.front().c();
        coordQueue.pop();
        if (r == er && c == ec)
        {
            return true;
        }
        if (maze[r + 1][c] == '.') // south
        {
            coordQueue.push(Coord(r + 1, c));
            maze[r + 1][c] = '#';
        }
        if (maze[r][c - 1] == '.') // west
        {
            coordQueue.push(Coord(r, c - 1));
            maze[r][c - 1] = '#';
        }
        if (maze[r - 1][c] == '.') // north
        {
            coordQueue.push(Coord(r - 1, c));
            maze[r - 1][c] = '#';
        }
        if (maze[r][c + 1] == '.') // east
        {
            coordQueue.push(Coord(r, c + 1));
            maze[r][c + 1] = '#';
        }
    }
    return false;
}
