// Game.cpp

#include "Game.h"
#include <iostream>
#include "utilities.h"
#include "Dungeon.h"
#include "Actor.h"
#include "GameConstants.h"
using namespace std;

Game::Game(int goblinSmellDistance) : m_pickedUp(false), m_victory(false), m_full(false)
{ // init list: all bools are false ,so nothing about picking up an object or having a full inventory is displayed
    //also, victory has not been achieved
    createPlayer(); // creates player to set it in the dungeon constructor so the dungeon knows what the player is
    m_dungeon = new Dungeon(0, m_player); // 0 is the level
    addPlayer();
    m_player->setWeapon(0); // sets the shortsword to position 0 of the player inventory, so it can be pushed
    m_player->setDungeon(m_dungeon);
    m_dungeon->setPlayer(m_player);
}

void Game::createPlayer()
{
    m_player = new Player();
}

void Game::takePlayerTurn(char c)
{
    int dir;
    m_player->gainHP(); // player gains hp, 1/10 chance
    m_player->checkHP(); // if hp 0 or less, homie is dead, end the game
    setPickedUp(false); // at the start of each turn, these are set false so nothing about picking up an object or having full inventory is displayed
    setFull(false);
    m_player->setMessage(""); // sets the message displayed to empty at start of turn, so nothing about an attack is displayed for more than one turn
    m_player->setWielding(false); // displaying the read or wield string for wielding or reading an object will not be displayed for more than one turn
    m_player->setReading(false);
    m_player->setActionString("");
    m_player->setWieldString(""); // all empty, it is start of new turn
    m_player->setReadString("");
    if (c == 'q') // quits
        exit(1);
    if (m_player->sleepTime() == 0) // can only do stuff if not asleep
    {
        if (decodeDirection(c, dir)) // takes ARROW_LEFT, etc., turns it into a direction WEST, etc.
        {
            m_player->attemptMove(dir); // moves in direction, attacks if necessary
        }
        else if (c == 'g')
        {
            int row = m_player->r();
            int col = m_player->c();
            if (m_dungeon->getCellStatus(row, col) >= MACE && m_dungeon->getCellStatus(row, col) <= S_TP && m_player->getInventory().size()<=25)
            { // MACE AND TP SCROLL are consecutive with all other objects in between 
                int current = m_dungeon->findObject(m_dungeon->getCellStatus(row, col));
                GameObject* o = m_dungeon->getObject(current); // erases object out of dungeon vector
                m_player->pickUp(o);
                m_dungeon->setCellStatus(row, col, EMPTY); // sets the spot to empty (there is nothing there now)
                setPickedUp(true); // so the display knows to display the string when you pick up something
            }
            if (m_dungeon->getCellStatus(row, col) == IDOL)
            {
                setVictory(true); // if the idol is picked up, game is over, the "Congratulations" thing is displayed
                displayVictory();
            }
            if (m_dungeon->getCellStatus(row, col) >= MACE && m_dungeon->getCellStatus(row, col) <= S_TP && m_player->getInventory().size() > 25)
            { // if try to pick up object and size if 25, you cant pick it up
                setFull(true);
            }
        }
        else if (c == 'i')
        {
            displayInventory();
            if (getCharacter() == 'i')
                return;
        }
        else if (c == '>' && m_dungeon->getCellStatus(m_player->r(), m_player->c()) == STAIR) // if stair button is pressed on stair
        {
            if (m_dungeon->level() == 0)
            {
                m_dungeon->setLevel(1); // set level to new level
                m_dungeon->clearLevel();
                m_dungeon->createDungeon(); // create dungeon knows which level it is, what monsters and weapons to add
            }
            else if (m_dungeon->level() == 1)
            {
                m_dungeon->setLevel(2);
                m_dungeon->clearLevel();
                m_dungeon->createDungeon();
            }
            else if (m_dungeon->level() == 2)
            {
                m_dungeon->setLevel(3);
                m_dungeon->clearLevel();
                m_dungeon->createDungeon();
            }
            else if (m_dungeon->level() == 3)
            {
                m_dungeon->setLevel(4);
                m_dungeon->clearLevel();
                m_dungeon->createDungeon();
            }
        }
        else if (c == 'w')
        {
            displayInventory();
            m_player->accessWield(); // access wield has wield() function, wield takes in the correct GameObject ptr
        }
        else if (c == 'r')
        {
            displayInventory();
            m_player->accessScroll(); // same thing as accessWield
        }
        else if (c == 'c')
        {
            m_player->setStrength(9); // sets stats to OP levels
            m_player->setMaxHp(50);
            m_player->setHP(50);
        }
    }
    else // if the player is asleep
    {
        m_player->setSleepTime(m_player->sleepTime() - 1); // decreases sleep time by 1
    }

}

void Game::displayPickUp()
{
    if (m_pickedUp) // if the player picked up something on the previous turn
    {
        cout << endl;
        GameObject* object = m_player->getInventory().back(); // the object that is in the back of the inventory is the object that was just picked up
        Scroll* s = dynamic_cast<Scroll*>(object);
        string str = "";
        if (s == nullptr) // if the object is a weapon
        {
            str = "You pick up " + object->getName();
        }
        else // if the object is a scroll
        {
            str = "You pick up a scroll called " + object->getName();
        }
        cout << str << endl;
    }
}

void Game::displayWielding()
{
    if (m_player->getWielding())
    {
        cout << m_player->getWieldString() << endl; // displays the "You wield 'weapon'" string for one turn after the previous
    }
}

void Game::displayReading()
{
    if (m_player->getReading())
    {
        cout << m_player->getReadString() << endl; // displays the "You read the scroll called..." for one turn after the item was picked up
        cout << m_player->getActionString(); // also displays the description of what the scroll does
    }
}

void Game::displayVictory()
{
    if (m_victory)
    {
        cout << "You pick up the golden idol" << endl;
        cout << "Congratulations, you won!" << endl;
        cout << "Press q to exit game." << endl;
        char c = getCharacter();
        while (c != 'q')
        {
            if (c == 'q') // the button you press must be q to quit or else youre stuck foreverrrrrrrrrrrrr
                exit(1);
            else
                c = getCharacter();
        }
    }
}

void Game::displayDeath()
{
    if (m_player->isDead())
    {
        cout << "Press q to exit game." << endl;
        char c = getCharacter();
        while (c != 'q') 
        {
            if (c == 'q') // the button you press must be q to quit or else youre stuck foreverrrrrrrrrrrrr
                exit(1);
            else
                c = getCharacter();
        }
    }
}

void Game::displayFull()
{
    if (m_full)
    {
        cout << "Your knapsack is full; you can't pick that up." << endl;
        return;
    }
}


void Game::play()
{
    display(); // displays the grid before taking any turns
    char ch = getCharacter();
    while (!gameOver()) // game ends when victory is set true (g is pressed on idol) or player dies OMEGALUL
    {
        takePlayerTurn(ch); // take player turn with getCharacter
        m_dungeon->takeMonsterTurns(); // take monster turn (they move or attack)
        if (gameOver() && m_victory) {
            break;
            display(); // displays the victory message and forces you to press q when you win
        }
        if (m_player->isDead())
        {
            display(); break; // displays "Press q to quit..." when you die
        }
        display(); // displays the dungeon after turns are taken
        displayPickUp();
        displayReading(); // displays these things if something was read, wielded, or picked up on the previous turn
        displayWielding();
        displayFull(); // displays the string "Your knapsack is full..." if the inventory is full
        if (m_player->hp() > 0)
            ch = getCharacter(); // can only go on to the next turn if you alive
        else display();
    }
}

bool Game::gameOver()
{
    if (m_player->isDead() || m_victory)
        return true;
    else
        return false;
}

bool Game::decodeDirection(char ch, int& dir)
{
    switch (ch)
    {
    default:  return false;
    case ARROW_UP: dir = NORTH; break;
    case ARROW_RIGHT: dir = EAST;  break;
    case ARROW_DOWN: dir = SOUTH; break;
    case ARROW_LEFT: dir = WEST;  break;
    }
    return true;
}

void Game::display()
{
    char displayGrid[18][70];
    int r = 0;
    int c = 0;

    for (r = 0; r <= 17; r++)
    {
        for (c = 0; c <= 69; c++)
        {
            switch (m_dungeon->getCellStatus(r, c)) // the statuses of cells are set in the creation of the dungeon
            {           // this just displays the correct icon for them
            case EMPTY:
                displayGrid[r][c] = ' ';
                break;
            case WALL:
                displayGrid[r][c] = '#';
                break;
            //case PLAYER:
            //    displayGrid[r][c] = '@';
            //    break;
            case STAIR:
                displayGrid[r][c] = '>';
                break;
            case MACE:
                displayGrid[r][c] = ')';
                break;
            case SHORTSWORD:
                displayGrid[r][c] = ')';
                break;
            case LONGSWORD:
                displayGrid[r][c] = ')';
                break;
            case MAGICAXE:
                displayGrid[r][c] = ')';
                break;
            case MAGICFANG:
                displayGrid[r][c] = ')';
                break;
            case S_HEALTH:
                displayGrid[r][c] = '?';
                break;
            case S_ARMOR:
                displayGrid[r][c] = '?';
                break;
            case S_STRENGTH:
                displayGrid[r][c] = '?';
                break;
            case S_DEXTERITY:
                displayGrid[r][c] = '?';
                break;
            case S_TP:
                displayGrid[r][c] = '?';
                break;
            case IDOL:
                displayGrid[r][c] = '&';
                break;
            default:
                displayGrid[r][c] = ' ';
                break;
            }
        }
    }

    if (m_player != nullptr)
    {
        displayGrid[m_player->r()][m_player->c()] = '@'; // follows the player's symbol as it moves over objects and stuff
    }

    vector<Actor*> monsterinventory = m_dungeon->getMonsters();

    if (monsterinventory.size() != 0)
    {
        vector<Actor*>::iterator itr;

        itr = monsterinventory.begin();
        while (itr != monsterinventory.end())
        {
            Actor* aptr = *itr;
            displayGrid[aptr->r()][aptr->c()] = aptr->getSymbol(); // follows the monsters' symbol as they move over objects and stuff
            itr++;
        }
    }

    clearScreen();
    for (r = 0; r <= 17; r++)
    {
        for (c = 0; c <= 69; c++)
            cout << displayGrid[r][c];
        cout << endl;
    }
    cout << endl;

    cout << "Dungeon Level : " << m_dungeon->level() << ", Hit points : " << m_player->hp() <<
        ", Armor : " << m_player->armor() << ", Strength : " << m_player->strength() << ", Dexterity : " <<
        m_player->dexterity(); // the stats
    cout << endl;

    cout << '\n' << m_player->getMessage(); // the message for attack, sleep, stuff

    if (m_player->getMessage() != "")
        cout << endl;

    // print monster action messagesj 
    if (monsterinventory.size() != 0)
    {
        vector<Actor*>::iterator itr;

        itr = monsterinventory.begin();
        while (itr != monsterinventory.end())
        {
            Actor* aptr = *itr;
            cout << aptr->getMessage(); // each monsters attack message
            if (aptr->getMessage() != "")
                cout << endl;
            itr++;
        }
    }

    cout << endl;
    if (m_victory)
        displayVictory();
    if (m_player->isDead())
        displayDeath();
}

void Game::displayInventory()
{
    clearScreen();
    cout << "Inventory" << endl;
    std::vector<GameObject*> temp = m_player->getInventory();
    std::vector<GameObject*>::const_iterator it = temp.begin();
    int i = 0;
    char l = 'a'; // starts at a., then goes to b
    while (it != temp.end())
    {
        cout << l << '.' << ' ';
        GameObject* o = m_player->getObject(i);
        Scroll* s = dynamic_cast<Scroll*>(o);
        if (s == nullptr)
        {
            cout << o->getName() << endl; // if weapon, then it is just name
        }
        else
        {
            cout << "A scroll called " << o->getName() << endl; // if scroll then it has to be the scroll called scroll of something
        }
        it++; // moves down the vector, new letter, one more down the player inventory
        i++;
        l++;
    }
}

void Game::addPlayer()
{
    int rPlayer, cPlayer;
    do
    {
        rPlayer = randInt(1, 16);
        cPlayer = randInt(1, 68);
    } while (m_dungeon->getCellStatus(rPlayer, cPlayer) == WALL || m_dungeon->getCellStatus(rPlayer,cPlayer) == ACTOR); // player is added first, it doesnt have to check for monsters

    m_player->setr(rPlayer);
    m_player->setc(cPlayer);
    m_player->makeWeapon(); // makes the short sword for the player
}

Game::~Game() 
{   
    delete m_dungeon;
    delete m_player;
}