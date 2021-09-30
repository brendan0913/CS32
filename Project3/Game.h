// Game.h
#ifndef GAME_INCLUDED
#define GAME_INCLUDED
#include "Actor.h"

class Dungeon;
// You may add data members and other member functions to this class.

class Game
{
public:
    //constructor
    Game(int goblinSmellDistance);

    //destructor
    ~Game();

    //member functions
    void play();
    void createPlayer();
    void addPlayer();
    void takePlayerTurn(char character);
    void display();
    void displayInventory();
    void displayPickUp();
    void displayWielding();
    void displayReading();
    void displayVictory();
    void displayDeath();
    void displayFull();
    void setPickedUp(bool p) { m_pickedUp = p; }
    void setVictory(bool v) { m_victory = v; }
    void setFull(bool f) { m_full = f; }
    bool gameOver();
    //getter functions
    Player* player() const { return m_player; }
    int getLevel() { return m_level; }

private:
    Dungeon* m_dungeon;
    Player* m_player;
    bool m_pickedUp;
    bool m_victory;
    int m_level;
    bool m_full;
    bool decodeDirection(char ch, int& dir);

};

#endif // GAME_INCLUDED
