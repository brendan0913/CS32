#ifndef ACTOR_H
#define ACTOR_H

#include "GameObject.h"
#include "GameConstants.h"
#include "utilities.h"
#include <vector>
#include <string>

class Dungeon;

class Actor
{
public:
	Actor(int hp, int armor, int strength, int dexterity)
		: m_hp(hp), m_armor(armor), m_strength(strength), m_dexterity(dexterity), m_maxHP(hp), m_sleepTime(0), m_dead(false) {}
	virtual ~Actor() {}

	//getter functions
	virtual int r() const { return m_r; }
	virtual int c() const { return m_c; }
	int hp() const { return m_hp; }
	int armor() const { return m_armor; }
	int strength() const { return m_strength; }
	int dexterity() const { return m_dexterity; }
	int sleepTime() const { return m_sleepTime; }

	virtual std::string getName() { return ""; }

	void setHP(int hp) 
	{ 
		if (hp <= 99) m_hp = hp;
		else m_hp = 99;
	}
	void setArmor(int a) 
	{
		if (m_armor <= 99) m_armor = a;
		else m_armor = 99;
	}
	void setStrength(int s) 
	{
		if (m_strength <= 99) m_strength = s;
		else m_strength = 99;
	}
	void setDexterity(int d)
	{
		if (m_dexterity <= 99) m_dexterity = d;
		else m_dexterity = 99;
	}
	void setSleepTime(int st) { m_sleepTime = st; }
	Dungeon* d() const { return m_dungeon; }
	void setDungeon(Dungeon* dp) { m_dungeon = dp; }
	virtual void move(int dir);
	virtual void attemptMove() {}

	virtual bool isDead() const { return m_dead; }
	void setDead() { m_dead = true; }
	void setr(int r) { m_r = r; }
	void setc(int c) { m_c = c; }

	void checkHP();
	virtual void gainHP();
	void decreaseHP(int damage) { m_hp = m_hp - damage; }

	void setMessage(std::string m) { m_message = m; }
	std::string getMessage() { return m_message; }
	char getSymbol() { return m_symbol; }
	void setSymbol(char s) {  m_symbol = s; }
	int getMaxHp() { return m_maxHP; }
	void setMaxHp(int max) { m_maxHP = max; }
	bool isMonster(int row, int col);

private:
	int m_r;
	int m_c;
	int m_hp;
	int m_maxHP;
	int m_armor;
	int m_strength;
	int m_dexterity;
	int m_sleepTime;
	bool m_dead;
	char m_symbol;

	Dungeon* m_dungeon;
	std::string m_message;
};

class Player : public Actor
{
public:
	Player() : Actor(20, 2, 2, 2), m_damageBonus(2),
		m_dexBonus(0)
	{
		setSymbol('@');
	}
	std::string attack(Actor* aptr);
	virtual void attemptMove(int dir);
	void pickUp(GameObject* object);

	int getDamageBonus() { return m_damageBonus; }
	void setDamageBonus(int db) { m_damageBonus = db; }
	int getDexBonus() { return m_dexBonus; }
	void setDexBonus(int db) { m_dexBonus = db; }
	bool wield(GameObject* go);
	std::string accessWield(); 

	void setWieldString(std::string w) { wieldString = w; }
	void setReadString(std::string r) { readString = r; }
	std::string getWieldString() { return wieldString; }
	std::string getReadString() { return readString; }


	void setWielding(bool w) { wielding = w; }
	bool getWielding() { return wielding; }
	bool getReading() { return reading; }
	void setReading(bool r) { reading = r; }
	void setActionString(std::string as) { scrollActionString = as; }
	std::string getActionString() { return scrollActionString; }
	bool read(GameObject* go);
	std::string accessScroll();
	std::vector<GameObject*> getInventory();
	GameObject* getObject(int i);

	void setCurrentMonster(int row, int col);
	void setMonster(std::vector<Actor*>& monsterlist);
	Weapon* getWeapon() const { return m_currentweapon; }
	void makeWeapon();
	void setWeapon(int i);

	virtual ~Player();

private:
	std::vector<GameObject*> m_inventory;
	std::vector<Actor*> m_monsters;
	Weapon* m_currentweapon;
	Actor* m_currentmonster;
	int m_damageBonus;
	int m_dexBonus;
	bool wielding;
	bool reading;
	std::string wieldString;
	std::string readString;
	std::string scrollActionString;
};

class Bogeyman : public Actor
{
public:
	Bogeyman(int r, int c, Player* p) : Actor(randInt(5, 10), 2, randInt(2, 3), randInt(2, 3)), m_playertracker(p)
	{
		setr(r);
		setc(c);
		setSymbol('B');
	}

	virtual std::string getName() { return "Bogeyman"; }
	void attemptMove();
	std::string attack();
	bool isPlayer(int r, int c);
	virtual ~Bogeyman() {}

private:
	Player* m_playertracker;
};

class Snakewoman : public Actor
{
public:
	Snakewoman(int r, int c, Player* p) : Actor(randInt(3, 6), 2, 3, 3), m_playertracker(p)
	{
		setr(r);
		setc(c);
		setSymbol('S');
	}
	virtual std::string getName() { return "Snakewoman"; }
	void attemptMove();
	std::string attack();
	bool isPlayer(int r, int c);
	virtual ~Snakewoman() {}
private:
	Player* m_playertracker;
};

class Goblin : public Actor
{
public:
	Goblin(int r, int c, Player* p, int smellDistance) : Actor(randInt(15, 20), 2, randInt(2, 3), randInt(2, 3)), m_playertracker(p), m_maxdistance(smellDistance)
	{
		setr(r);
		setc(c);
		setSymbol('G');
	}

	virtual std::string getName() { return "Goblin"; }
	void attemptMove();
	void tryMove(int dir);
	std::string attack();
	bool isPlayer(int r, int c);
	bool openSpot(int grid[18][70], int x, int y, int dist);
	void findPath(int grid[18][70], int sr, int sc, int er, int ec, int& min_dist, int dist);
	int findBestDirection(int grid[18][70], int sr, int sc, int er, int ec, int& min_dist, int dist);
	void setCellStatus(int r, int c, int status) { goblinGrid[r][c] = status; }
	int getCellStatus(int r, int c) const { return goblinGrid[r][c]; }
	virtual ~Goblin() {}
private:
	Player* m_playertracker;
	int goblinGrid[18][70];
	int m_direction;
	int m_maxdistance;

};

class Dragon : public Actor
{
public:
	Dragon(int r, int c, Player* p, int hp) : Actor(hp, 4, 4, 4), m_playertracker(p)
	{
		setr(r);
		setc(c);
		setSymbol('D');
	}
	std::string attack();
	void attemptMove(); // doesnt actually move, in this case "move" is the dragon's turn, where it regains hp and attacks
	virtual std::string getName() { return "Dragon"; }
	bool isPlayer(int r, int c);
	virtual ~Dragon() {}
private:
	Player* m_playertracker;
};

#endif // !ACTOR_H

