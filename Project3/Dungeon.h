#ifndef DUNGEON_H
#define DUNGEON_H

#include <string>
#include <vector>
#include "Actor.h"
#include "GameObject.h"

struct Room;

class Dungeon
{
public:
    Dungeon(int level, Player* m_player);
    void createDungeon();
    void clearLevel();
    void setPlayerPos();

    void addGoblins(int& num);
    void addSnakewomen(int& num);
    void addBogeymen(int& num);
    void addDragons(int& num);
    void addWeapon(int& num);
    void addScroll(int& num);
    void addStairway();
    void addGoldenIdol();

    int getCellStatus(int r, int c) const;
    void setCellStatus(int r, int c, int status);
    void takeMonsterTurns();
    GameObject* getObject(int i);
    int findObject(int i);

    void setPlayer(Player* player) { m_player = player; }
    Player* getPlayer() { return m_player; }

    int level() { return m_level; }
    void setLevel(int l) { m_level = l; }
    std::vector<Actor*> getMonsters() { return m_monsters; }
    std::vector<GameObject*> getObjects() { return m_objects; }
    void vibeCheck(Actor* a);

    double percentEmpty();
    void roomConnect(Room a, Room b);

    void clearActorStatus();
    void teleportPlayer();
    bool checkIfMonsterOnSpot(int r, int c);

    ~Dungeon();
private:
    int m_level;
    int m_grid[18][70];
    int m_goblinGrid[18][70];
    bool isPosInBounds(int r, int c) const;
    void checkPos(int r, int c) const;
    std::vector<GameObject*> m_objects;
    std::vector<Actor*> m_monsters;
    Player* m_player;
};

struct Room
{
public:
    Room()
    {
        top_r = randInt(3, 10); // position of top row of the room (open area)
        left_c = randInt(3, 63); // position of first column of the room (open area)
        numrows = randInt(3, 5); // height of the room
        numcols = randInt(5, 12); // width of the room

        midrow = top_r + numrows / 2;
        midcol = left_c + numcols / 2;

        bottom_r = top_r + numrows;
        right_c = left_c + numcols;
    };
    int top_r;
    int left_c;
    int bottom_r;
    int right_c;
    int midrow;
    int midcol;
    int numrows;
    int numcols;
};

bool doesOverlap(Room a, Room b);
bool inBounds(Room c);
bool roomCompare(const Room& a, const Room b);

#endif // !DUNGEON_H
