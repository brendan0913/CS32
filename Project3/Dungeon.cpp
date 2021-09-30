#include "Dungeon.h"
#include "utilities.h"
#include "GameObject.h"
#include "GameConstants.h"
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

Dungeon::Dungeon(int level, Player* player) : m_level(level), m_player(player)
{
    createDungeon(); // creates dungeon, level 0
}

void Dungeon::createDungeon()
{
    int r, c;
    int max_r = 18;
    int max_c = 70;

    for (r = 0; r < max_r; r++)
        for (c = 0; c < max_c; c++)
            m_grid[r][c] = WALL; // sets every position to a wall at first

    vector<Room> rvector; // uses vector of Rooms

    while (percentEmpty() < 40 && rvector.size() < randInt(4,6)) // makes 4-6 rooms, the percent of the dungeon filled with EMPTY is less than 40
    {  // percentEmpty counts the number of EMPTY and divides it by the total number of positions, 18*70
        Room room;
        bool overlaps = false;
        do
        {
            room = Room(); // makes room
            overlaps = false;
            if (rvector.size() > 0) // if the room hasnt been created yet and pushed onto the vector, then it cant overlap
            {
                vector<Room>::iterator it;
                it = rvector.begin();
                while (it != rvector.end() && !overlaps)
                {
                    if (doesOverlap(room, *it)) // checks if rooms overlap, if they do, then the room shouldn't be made
                    {
                        overlaps = true;
                    }
                    it++;
                }
            }

        } while (!inBounds(room) || overlaps); // makes room if these are false (so the room is inBounds and does not overlap)

        rvector.push_back(room); 

        int currentrow = room.top_r;

        for (int i = 0; i < room.numrows; i++)
        {
            for (int j = 0; j < room.numcols; j++)
            {
                m_grid[currentrow][room.left_c + j] = EMPTY; // sets the entire interior of the room to empty, by each column at first
            }
            currentrow++;
        }
    }

    sort(rvector.begin(), rvector.end(), &roomCompare); // sorts the rooms to connect them, using a custom comparison operator

    vector<Room>::iterator it;

    it = rvector.begin();
    
    while (it != rvector.end() - 1) // cannot connect the last room to the void, so stops before the last room
    {
        roomConnect(*it, *(it + 1)); // connects the rooms that are next to each since they were sorted
        it++;
    }

    int numOfObjects = randInt(2, 3); // there can be 2-3 objects per level, this number is passed by reference
    int numOfMonsters = randInt(2, 5 * (m_level + 1) + 1); // total number of monsters per level, passed by reference to the add functions
    if (m_level == 0 || m_level == 1) 
    {
        clearLevel(); // clears the level (empties and deletes the monster and object vectors, if it is level 1, clears the level and sets the player to a new random empty position
        addGoblins(numOfMonsters); // only goblins and snakewomen on 0th and 1st floor
        addSnakewomen(numOfMonsters); // the amount of snakewomen added will be the remainder of the predetermined num of monsters after a random num of goblins is added
        m_player->setMonster(m_monsters); // so the player can know of the monsters and is able to fight and attack the correct current monster
        clearActorStatus();
        addWeapon(numOfObjects); // adds weapons randomly and to valid spots, finds the number of weapons to add using randInt from 0 to numofObjects, then the rest will be scrolls
        addScroll(numOfObjects); // adds scrolls, an amount which is the remainder of numOfObjects still left to be added
        addStairway(); // adds stair to valid position
    }
    if (m_level == 2)
    {
        clearLevel(); // deletes the monsters and objects if there are any remaining (havent been picked up or killed)
        addGoblins(numOfMonsters);
        addSnakewomen(numOfMonsters);
        addBogeymen(numOfMonsters); // level two has bogeymen
        m_player->setMonster(m_monsters);
        clearActorStatus();
        addWeapon(numOfObjects);
        addScroll(numOfObjects);
        addStairway();
    }

    if (m_level == 3)
    {
        clearLevel();
        addGoblins(numOfMonsters);
        addSnakewomen(numOfMonsters);
        addBogeymen(numOfMonsters);
        addDragons(numOfMonsters); // level 3 (and 4) has dragons
        m_player->setMonster(m_monsters);
        clearActorStatus();
        addWeapon(numOfObjects);
        addScroll(numOfObjects);
        addStairway();
    }

    if (m_level == 4)
    {
        clearLevel();
        addGoblins(numOfMonsters);
        addSnakewomen(numOfMonsters);
        addBogeymen(numOfMonsters);
        addDragons(numOfMonsters);
        m_player->setMonster(m_monsters);
        clearActorStatus();
        addWeapon(numOfObjects);
        addScroll(numOfObjects);
        addGoldenIdol(); // level 4 has golden idol, not staircase
    }
}

double Dungeon::percentEmpty()
{
    double filled = 0;
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            if (getCellStatus(i, j) == EMPTY) // calculates the number of empty spots in the dungeon
                filled++;
        }
    }
    //    18 * 70 = 1260
    return (filled / 1260.0) * 100;
}

bool doesOverlap(Room a, Room b)
{
    if ((a.left_c >= b.right_c || b.left_c >= a.right_c) && (a.top_r <= b.bottom_r || b.top_r <= a.bottom_r))
        return false; // if the left of one room is to the right of the other room, and if the top of one room is below the 
                    // other room, then the rooms dont overlap
    return true;
}

bool inBounds(Room c)
{
    if (c.top_r >=1 && c.left_c >= 1 && c.right_c <= 68 && c.bottom_r <= 16) // rooms dont go past the outer edge of walls
        return true;

    return false;
}

bool roomCompare(const Room& a, const Room b)
{
    if (a.left_c <= b.left_c) 
        return true;
    else
        return false;
}

void Dungeon::roomConnect(Room a, Room b)
{
    int rdistance = abs(a.midrow - b.midrow); // takes distance between each room's center, which is where the corridor starts
    int cdistance = abs(a.midcol - b.midcol);

    if (a.midrow < b.midrow)
    {
        for (int i = 0; i < rdistance; i++)
            m_grid[a.midrow + i][a.midcol] = EMPTY; // sets each spot empty, going right, towards the row equal to the other room's center

        for (int j = 0; j < cdistance; j++)
        {
            m_grid[a.midrow + rdistance][a.midcol + j] = EMPTY; // sets each spot empty, going towards the column equal to the other room's center
        }

    }
    else // same process, but now going the opposite way, left, since the center of a is greater than b
    {
        for (int i = 0; i < rdistance; i++)
            m_grid[a.midrow - i][a.midcol] = EMPTY;

        for (int j = 0; j < cdistance; j++)
        {
            m_grid[a.midrow - rdistance][a.midcol + j] = EMPTY;
        }

    }
}

void Dungeon::setPlayerPos()
{
    int rPlayer, cPlayer;
    do
    {
        rPlayer = randInt(1, 16);
        cPlayer = randInt(1, 68);
    } while (getCellStatus(rPlayer, cPlayer) == WALL); // find valid coordinate; if the coordinate has an actor or wall on
                                        // it, then it finds another coordinate until it doesnt
    m_player->setr(rPlayer);
    m_player->setc(cPlayer);
    if (getCellStatus(rPlayer, cPlayer) == EMPTY) // to not erase an object or stairway or idol
        setCellStatus(rPlayer, cPlayer, ACTOR);
}

void Dungeon::teleportPlayer()
{
    int rPlayer, cPlayer;
    do
    {
        rPlayer = randInt(1, 16);
        cPlayer = randInt(1, 68);
        checkIfMonsterOnSpot(rPlayer, cPlayer);
    } while (getCellStatus(rPlayer, cPlayer) == WALL || checkIfMonsterOnSpot(rPlayer, cPlayer)); // find valid coordinate; if the coordinate has an actor or wall on
                                        // it, then it finds another coordinate until it doesnt
    m_player->setr(rPlayer);
    m_player->setc(cPlayer);
}

bool Dungeon::checkIfMonsterOnSpot(int r, int c)
{
    for (Actor* a : m_monsters)
    {
        if (r == a->r() && c == a->c())
        {
            return true;
        }
    }
    return false;
}

void Dungeon::clearLevel()
{
    if (m_level>0) // only sets the player to a new position if its not the first level (since the first level already created the player at a position)
        setPlayerPos();
    if (m_monsters.size() != 0) // deletes monsters only if there are some remaining that werent killed and deleted
    {
        std::vector<Actor*>::iterator it = m_monsters.begin();
        while (m_monsters.size() != 0)
        {
            delete* it;
            it = m_monsters.erase(it);
        }
    }
    if (m_objects.size() != 0) // deletes objects only if there are some remaining that werent picked up 
    {
        std::vector<GameObject*>::iterator itr = m_objects.begin();
        while (m_objects.size() != 0)
        {
            delete* itr;
            itr = m_objects.erase(itr);
        }
    }
}

void Dungeon::clearActorStatus()
{
    for (int r = 0; r < 18; r++)
    {
        for (int c = 0; c < 70; c++)
        {
            if (getCellStatus(r, c) == ACTOR)
            {
                setCellStatus(r, c, EMPTY);
            }
        }
    }
}


int Dungeon::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r][c]; // EMPTY, SCROLL, GOBLIN, etc.
}

void Dungeon::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r][c] = status;
}

bool Dungeon::isPosInBounds(int r, int c) const
{
    return (r >= 0 && r <= 17 && c >= 0 && c <= 69);
}

void Dungeon::checkPos(int r, int c) const
{
    if (!isPosInBounds(r, c))
    {
        exit(-1);
    }
}

void Dungeon::addGoblins(int& num)
{
    int nGoblins = randInt(0, num); // takes in the number of monsters determined by the level and calculated in createDungeon
    if (nGoblins == 0) { return; } // makes no goblins, goes on to the next monster to make
    for (int i = 0; i < nGoblins; i++)
    {
        int rw, cw;
        do
        {
            rw = randInt(1, 16); // cannot be on the outside row and columns
            cw = randInt(1, 68);
        
        } while (getCellStatus(rw, cw) == WALL || getCellStatus(rw,cw) == ACTOR);

        Goblin* m = new Goblin(rw, cw, m_player, 15); // player is passed in so the goblin knows where the player is
        if (getCellStatus(rw, cw) == EMPTY) // to not erase an object if the goblin is placed there
            setCellStatus(rw, cw, ACTOR);
        m_monsters.push_back(m);
        m->setDungeon(this); // sets the goblins dungeon to m_dungeon
    }
    num -= nGoblins; // now the other monsters will be added from the remaining amount of monsters to be added
}

void Dungeon::addSnakewomen(int& num)
{
    if (m_level == 0 || m_level == 1) // on level 0 or 1 after adding the goblin, the snakewoman is the only monster
    {                           // left to be added, so the num of Snakewomen is determined by the numOfMonsters to be made minus the number of goblins added
        for (int i = 0; i < num; i++)
        {
            int rw, cw;
            do
            {
                rw = randInt(1, 16);
                cw = randInt(1, 68);
            } while (getCellStatus(rw, cw) == WALL || getCellStatus(rw,cw) == ACTOR);

            Snakewoman* s = new Snakewoman(rw, cw, m_player);
            if (getCellStatus(rw, cw) == EMPTY)
                setCellStatus(rw, cw, ACTOR);
            m_monsters.push_back(s);
            s->setDungeon(this);
        }
    }
    if (m_level == 2 || m_level == 3 || m_level == 4) // if there are more monsters to add after snakewoman
    {
        int nSnakewomen = randInt(0, num); // uses same technique as goblin
        if (nSnakewomen == 0) { return; }
        for (int i = 0; i < nSnakewomen; i++)
        {
            int rw, cw;
            do
            {
                rw = randInt(1, 16);
                cw = randInt(1, 68);
            } while (getCellStatus(rw, cw) == WALL || getCellStatus(rw, cw) == ACTOR);

            Snakewoman* s = new Snakewoman(rw, cw, m_player);
            if (getCellStatus(rw, cw) == EMPTY)
                setCellStatus(rw, cw, ACTOR);
            m_monsters.push_back(s);
            s->setDungeon(this);
        }
        num -= nSnakewomen; // the num of Monsters to be added after Snakewomen are added is determined by the 
        // difference between the number of monsters left to add and the snakewomen added
    }
}

void Dungeon::addBogeymen(int& num)
{
    if (m_level == 2) // if level 2, bogeymen are the last monster to be added
    {
        for (int i = 0; i < num; i++) // the number of them added is the remaining number of monsters to be added
        {           // after the goblin and snakewomen are added
            int rw, cw;
            do
            {
                rw = randInt(1, 16);
                cw = randInt(1, 68);
            } while (getCellStatus(rw, cw) == WALL || getCellStatus(rw, cw) == ACTOR);

            Bogeyman* b = new Bogeyman(rw, cw, m_player);
            if (getCellStatus(rw, cw) == EMPTY)
                setCellStatus(rw, cw, ACTOR);
            m_monsters.push_back(b);
            b->setDungeon(this);
        }
    }
    if (m_level == 3 || m_level == 4)
    {
        int nBogeymen = randInt(0, num); // on level 3 and 4, there are still dragons to be added
        if (nBogeymen == 0) { return; }
        for (int i = 0; i < nBogeymen; i++)
        {
            int rw, cw;
            do
            {
                rw = randInt(1, 16);
                cw = randInt(1, 68);
            } while (getCellStatus(rw, cw) == WALL || getCellStatus(rw, cw) == ACTOR);

            Bogeyman* b = new Bogeyman(rw, cw, m_player);
            if (getCellStatus(rw, cw) == EMPTY)
                setCellStatus(rw, cw, ACTOR);
            m_monsters.push_back(b);
            b->setDungeon(this);
        }
        num -= nBogeymen; // the number of dragons to be added is the remainder of the initial numOfmonsters and the number of monsters added
    }
}

void Dungeon::addDragons(int& num) // the number of dragons added is always the remainder of monsters left to be added
{                           // since there are never any monsters to add after the dragons
    for (int i = 0; i < num; i++)
    {
        int rw, cw;
        do
        {
            rw = randInt(1, 16);
            cw = randInt(1, 68);
        }
        while (getCellStatus(rw, cw) == WALL || getCellStatus(rw,cw) == ACTOR);

        Dragon* d = new Dragon(rw, cw, m_player, randInt(20, 25));
        if (getCellStatus(rw, cw) == EMPTY)
            setCellStatus(rw, cw, ACTOR);
        m_monsters.push_back(d);
        d->setDungeon(this);
    }
}

void Dungeon::addWeapon(int& num)
{
    int nWeapons = randInt(0, num); // same process as the monsters since there are scrolls to add after weapons
    if (nWeapons == 0) { return; }
    int rWeapon, cWeapon;
    for (int i = 0; i < nWeapons; i++)
    {
        do
        {
            rWeapon = randInt(1, 16);
            cWeapon = randInt(1, 68);
        } while (getCellStatus(rWeapon, cWeapon) != EMPTY); // objects can be added anywhere, as long as it isnt stairway or idol

        int weapo = randInt(MACE, LONGSWORD); // magic weapons are only added to the dungeon when dropped
        Weapon* weapon = new Weapon(rWeapon, cWeapon, weapo);
        m_grid[rWeapon][cWeapon] = weapo;
        m_objects.push_back(weapon); // the object vector for dungeon
    }
    num -= nWeapons; // the number of scrolls to be added is the numberOfObjects initially minus the number of Weapons added
}

void Dungeon::addScroll(int& num)
{
    int rScroll, cScroll;
    for (int i = 0; i < num; i++) // the num of scrolls to add is the number of objects left to add after weapons are added
    {
        do
        {
            rScroll = randInt(1, 16);
            cScroll = randInt(1, 68);
        } while (getCellStatus(rScroll, cScroll) != EMPTY);

        int sc = randInt(S_HEALTH, S_STRENGTH); // TP scroll is only added to the dungeon when dropped by a dragon
        Scroll* scptr = new Scroll(rScroll, cScroll, sc);
        m_grid[rScroll][cScroll] = sc;
        m_objects.push_back(scptr);
    }
}

void Dungeon::addStairway()
{
    int rStair, cStair;
    do
    {
        rStair = randInt(1, 16);
        cStair = randInt(1, 68);
    } while (getCellStatus(rStair, cStair) != EMPTY); // must be empty, no objects
    setCellStatus(rStair, cStair, STAIR);
    m_grid[rStair][cStair] = STAIR;
}

void Dungeon::addGoldenIdol()
{
    int ridol, cidol;
    do
    {
        ridol = randInt(1, 16);
        cidol = randInt(1, 68);
    } while (getCellStatus(ridol, cidol) != EMPTY); // must be empty, no objects
    setCellStatus(ridol, cidol, IDOL);
    m_grid[ridol][cidol] = IDOL;
}

void Dungeon::vibeCheck(Actor* a) // kill, delete the monsters
{
    vector<Actor*>::iterator it = m_monsters.begin();
    while (*it != a) { it++; } // goes through the monster to find the monster that was killed
    if (*it == a)
    {
        Bogeyman* b = dynamic_cast<Bogeyman*>(*it);
        if (b != nullptr)
        {
            int poss = randInt(1, 10); // 1 in 10 chance to drop a magic axe
            if (poss == 1 && getCellStatus(b->r(),b->c()) == EMPTY) // if there is an object or stair on the spot, nothing will be added
            {
                Weapon* ma = new Weapon(b->r(), b->c(), MAGICAXE);
                m_grid[b->r()][b->c()] = MAGICAXE;
                m_objects.push_back(ma);
            }
            delete* it;
            m_monsters.erase(it);
        }
        else // monster is not bogeyman
        {
            Snakewoman* s = dynamic_cast<Snakewoman*>(*it);
            if (s != nullptr)
            {
                int poss = randInt(1, 3); // 1 in 3 chance to drop magicfang if the spot has no object on it
                if (poss == 1 && getCellStatus(s->r(),s->c()) == EMPTY)
                {
                    Weapon* mf = new Weapon(s->r(), s->c(), MAGICFANG);
                    m_grid[s->r()][s->c()] = MAGICFANG;
                    m_objects.push_back(mf);
                }
                delete* it;
                m_monsters.erase(it);
            }
            else // killed monster is not bogeyman or snakewoman
            {
                Goblin* g = dynamic_cast<Goblin*>(*it);
                if (g != nullptr)
                {
                    int poss = randInt(1, 3); // 1 in 3 chance to drop weapon if there is nothing on the spot
                    if (poss == 1 && getCellStatus(g->r(),g->c()) == EMPTY)
                    {
                        int type = randInt(MAGICFANG, MAGICAXE); // the ints for these weapons are consecutive, so the randInt will always choose either one
                        Weapon* w = new Weapon(g->r(), g->c(), type);
                        m_grid[g->r()][g->c()] = type;
                        m_objects.push_back(w);
                    }
                    delete* it;
                    m_monsters.erase(it);
                }
                else // if monster is not bogeyman, snakewoman, or dragon
                {
                    Dragon* d = dynamic_cast<Dragon*>(*it);
                    if (d != nullptr)
                    {
                        if (getCellStatus(d->r(), d->c()) == EMPTY)
                        {
                            int type = randInt(S_HEALTH, S_TP); // 100% chance to drop scroll
                        // these ints are consecutive, nothing undefined will be added
                            Scroll* s = new Scroll(d->r(), d->c(), type);
                            m_grid[d->r()][d->c()] = type;
                            m_objects.push_back(s);
                        }
                        delete* it;
                        m_monsters.erase(it);
                    }
                }
            }
        }
    }
}

void Dungeon::takeMonsterTurns()
{
    //call attempt move on each monster if there are some alive
    if (m_monsters.size() != 0)
    {
        vector<Actor*>::iterator itr;

        itr = m_monsters.begin();
        while (itr != m_monsters.end())
        {
            Actor* aptr = *itr; // goes through each monster
            aptr->setMessage(""); // sets the attack message to empty before each turn to only display the attack message once per turn
            if (aptr->sleepTime() == 0)
            {
                aptr->attemptMove(); // can only move if they are not asleep
            }
            else
            {
                aptr->setSleepTime(aptr->sleepTime() - 1); // decreases sleeptime for monster
            }
            itr++;
        }
    }
}

int Dungeon::findObject(int i)
{
    vector<GameObject*>::iterator itr;

    itr = m_objects.begin();
    int j = 0;

    while (itr != m_objects.end())
    {
        if (m_objects[j]->getNameInt() == i) // finds the desired object from the dungeon object inventory
            return j;
        itr++;
        j++;
    }

    return 0; // if the size of the vector is 0, return 0
}

GameObject* Dungeon::getObject(int i)
{
    GameObject* o = m_objects[i];
    m_objects.erase(m_objects.begin() + i); // removes the desired object from the dungeon object inventory
    return o;
}

Dungeon::~Dungeon()
{ // deletes the objects and monsters that were not killed or picked up
    if (m_objects.size() != 0)
    {
        std::vector<GameObject*>::iterator it = m_objects.begin();
        while (m_objects.size() != 0)
        {
            delete* it;
            it = m_objects.erase(it);
        }
    }
    if (m_monsters.size() != 0)
    {
        std::vector<Actor*>::iterator it = m_monsters.begin();
        while (m_monsters.size() != 0)
        {
            delete* it;
            it = m_monsters.erase(it);
        }
    }

}