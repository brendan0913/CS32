#include "Actor.h"
#include "utilities.h"
#include "GameConstants.h"
#include "Dungeon.h"
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

void Actor::move(int direction) // move is only ever called after attempt move returns true
{
	int crow = r();
	int ccol = c();

	switch (direction)
	{
	case WEST:
		setc(ccol - 1); // simply sets the actor's position to the new position after receiving a direction from attempt move
		break;

	case EAST:
		setc(ccol + 1);
		break;

	case NORTH:
		setr(crow - 1);
		break;

	case SOUTH:
		setr(crow + 1);
		break;

	default:
		break;
	}
}

void Player::attemptMove(int dir) // takes in a direction in the form of any integer from a function called decodeDirection
{ // which uses getCharacter and returns a direction based on the input
	int row = r();
	int col = c();
	if (sleepTime() == 0) // only allowed to move if the player is not asleep
	{
		switch (dir)
		{
		case WEST:
			if (isMonster(row, col - 1)) // if there is a monster to the west and the player attempts to move west
			{
				setCurrentMonster(row, col - 1); // gets the current monster from the vector of monsters
				setMessage(attack(m_currentmonster)); // attacks the monster with the correct message ("Player slashes..."
			}
			else if (d()->getCellStatus(row, col - 1) != WALL) // now the only object that can block the player's movement
			{												// is a wall, since player will attack monsters or walk over objects and the stairs
				move(dir); // moves in that direction
				setMessage(""); // there is no displayed message on a move
			}
			break;

		case EAST: // same prcoess as WEST
			if (isMonster(row, col + 1))
			{
				setCurrentMonster(row, col + 1);
				setMessage(attack(m_currentmonster));
			}
			else if (d()->getCellStatus(row, col + 1) != WALL)
			{
				move(dir);
				setMessage("");
			}
			break;

		case NORTH: // same prcoess as WEST
			if (isMonster(row - 1, col))
			{
				setCurrentMonster(row - 1, col);
				setMessage(attack(m_currentmonster));
			}
			else if (d()->getCellStatus(row - 1, col) != WALL)
			{
				move(dir);
				setMessage("");
			}
			break;

		case SOUTH: // same prcoess as WEST
			if (isMonster(row + 1, col))
			{
				setCurrentMonster(row + 1, col);
				setMessage(attack(m_currentmonster));
			}
			else if (d()->getCellStatus(row + 1, col) != WALL)
			{
				move(dir);
				setMessage("");
			}
			break;
		default:
			break;

		}
	}
}

void Player::makeWeapon() // makes the short sword for player and puts it as the first object in the player's inventory
{
	Weapon* wptr = new Weapon(r(), c(), SHORTSWORD);
	
	m_inventory.push_back(wptr);
}

void Player::setWeapon(int i) // when player wields a weapon, it is set as the current weapon
{
	GameObject* optr = m_inventory[i];
	Weapon* wptr = dynamic_cast<Weapon*>(optr); // looks through the inventory, turns the GameObject pointer to a weapon pointer
	m_currentweapon = wptr;
}

bool Actor::isMonster(int row, int col)
{
	if (d()->getMonsters().size() != 0) // there are no monsters if the size of the monster vector in Dungeon is 0
	{
		std::vector<Actor*> temp = d()->getMonsters();

		vector<Actor*>::const_iterator itr = temp.begin();

		while (itr != temp.end())
		{
			Actor* a = *itr;
			if (row == a->r() && col == a->c()) // if the parameters for the row and col match the current monster's position
			{
				return true;
			}
			itr++;
		}
		return false; // returns false if the position is not the current monster's position
	}
	return false; // return false if there are no monsters
}

void Player::setCurrentMonster(int row, int col)
{
	std::vector<Actor*> temp = d()->getMonsters(); // gets the monster vector from Dungeon

	vector<Actor*>::const_iterator itr = temp.begin();
	while (itr != temp.end())
	{
		Actor* a = *itr;
		if (row == a->r() && col == a->c())
		{
			m_currentmonster = a; // same process as IsMonster, but this function sets the current monster so the Player is fighting the correct monster
		}
		itr++;
	}
}

void Player::setMonster(std::vector<Actor*>& monsterlist)
{
	m_monsters = monsterlist;
}

std::string Player::attack(Actor* mptr)
{
	string message = "Player " + m_currentweapon->getAction() + " "; // gets the action string of the current weapon, like "slashes short sword at"
	int attackerPoints = dexterity() + m_currentweapon->dexterityBonus(); // combines the player's dexterity and the weapon's bonus
	int defenderPoints = mptr->dexterity() + mptr->armor(); // combines the current monster's dexterity and armor points
	message += mptr->getName();
	if (randInt(1, attackerPoints) >= randInt(1,defenderPoints)) // if the attack is a hit
	{
		int damagePoints = randInt(0, strength() + m_currentweapon->damageAmount() - 1); // using formula of calculating damage points
		mptr->decreaseHP(damagePoints); // does the damage to the HP
		if (getWeapon()->getNameInt() != MAGICFANG) // if the weapon cannot sleep someone
		{
			if (mptr->hp() <= 0)
			{
				message = message + " dealing a final blow.";
				d()->vibeCheck(mptr);
			}
			else
				message = message + " and hits.";
		}
		else // if it is a magic fang
		{
			if (mptr->hp() <= 0) // if the hit kills
			{
				message = message + " dealing a final blow.";
				d()->vibeCheck(mptr); // deletes the current monster from the monster vector
				return message;
			}
			int sleepPoss = randInt(1, 5); // 1 in 5 chance
			if (sleepPoss == 1) // if the hit puts the monster to sleep
			{
				int sleepTime = randInt(2, 6); // gets the sleep time
				if (mptr->sleepTime() > 0) // if already asleep
				{
					if (sleepTime > mptr->sleepTime()) // if the new sleeptime is greater, it is the sleep time
					{
						mptr->setSleepTime(sleepTime);
					} // if it is not greater, then the previous sleep time is the sleeptime
				}
				else // if not asleep
				{
					mptr->setSleepTime(sleepTime);
				}
				message += " putting the " + mptr->getName() + " to sleep."; // adds to the message
			}
			else // if the hit does not put to sleep
			{
				if (mptr->hp() <= 0) // if te monster dies
				{
					message = message + " dealing a final blow.";
					d()->vibeCheck(mptr);
					return message;
				}
				else // if the monster is hit but does not die
				{
					message = message + " and hits.";
				}
			}
		}
	}
	else // if the hit misses
	{
		message = message + " and misses.";
	}
	return message;
}

void Bogeyman::attemptMove()
{
	int prow = m_playertracker->r(); // m_playertracker is the player pointer set when the Bogeyman was added (to know where the player is)
	int pcol = m_playertracker->c();
	int rdistance = abs(prow - r());
	int cdistance = abs(pcol - c());

	int distancetoplayer = rdistance + cdistance; // adds the distance of how far away the mosnter and player is

	int row = r();
	int col = c();

	if (distancetoplayer == 1) // if the monster and player are 1 apart, that means they are next to each other
	{
		//west
		if (isPlayer(row, col - 1)) // if the player is next to the monster, the monster attacks
			setMessage(attack());
		//east
		if (isPlayer(row, col + 1))
			setMessage(attack());
		//north
		if (isPlayer(row - 1, col))
			setMessage(attack());
		//south
		if (isPlayer(row + 1, col))
			setMessage(attack());
	}
	else
	{

		if (distancetoplayer < 6) // Bogeymen have smell range of 5
		{
			// north
			int north_r = row - 1;
			int new_rdistance = abs(prow - north_r);
			int newdistancetoplayer = new_rdistance + cdistance;
			if (newdistancetoplayer < distancetoplayer 
				&& d()->getCellStatus(north_r, col) != WALL && !isMonster(north_r, col))
			{ // if the theoretical distance after moving one to the north, if the spot is open (not occupied by an actor,
				// not including the player since the player is already not 1 north of the monster), shortens the distance
				// from monster to player, then the bogeyman will move in that direction
				move(NORTH); // moves north
				setMessage(""); // the message is empty since there is no displayed message on a simple move
			}

			else
			{
				// south
				int south_r = row + 1;
				new_rdistance = abs(prow - south_r);
				newdistancetoplayer = new_rdistance + cdistance;
				if (newdistancetoplayer < distancetoplayer && d()->getCellStatus(south_r, col) != WALL
					&& !isMonster(south_r, col))
				{ // same process as with moving to the north
					move(SOUTH);
					setMessage("");
				}

				else
				{
					// west
					int west_c = col - 1;
					int new_cdistance = abs(pcol - west_c);
					newdistancetoplayer = rdistance + new_cdistance;
					if (newdistancetoplayer < distancetoplayer &&
						d()->getCellStatus(row, west_c) != WALL && !isMonster(row, west_c))
					{ // same process as with moving to the north
						move(WEST);
						setMessage("");
					}
					else
					{ // same process as with moving to the north
						// east
						int east_c = col + 1;
						new_cdistance = abs(pcol - east_c);
						newdistancetoplayer = rdistance + new_cdistance;
						if (newdistancetoplayer < distancetoplayer &&
							d()->getCellStatus(row, east_c) != WALL && !isMonster(row, east_c))
						{
							move(EAST);
							setMessage("");
						}
					}
				}
			}
		}
	}
}

bool Bogeyman::isPlayer(int r, int c)
{
	if (m_playertracker->r() == r && m_playertracker->c() == c) // if the player is at the positon indicated by the passed in row and col
		return true;
	else
		return false;
}

string Bogeyman::attack()
{
	string message = "the Bogeyman slashes short sword at Player "; // Bogeyman will always have the same message, as he only has a short sword

	int attackerPoints = dexterity(); // Bogeyman's dexterity (could be 2 or 3)
	int defenderPoints = m_playertracker->dexterity() +m_playertracker->armor(); // player's armor and dexterity
	if (randInt(1,attackerPoints) >= randInt(1,defenderPoints)) // if the hit lands
	{
		int damagePoints = randInt(0, strength() + 1); // strength is 2 or 3
		m_playertracker->decreaseHP(damagePoints); // attacks and does damage

		if (m_playertracker->hp() <= 0) // if the player dies
			message = message + "dealing a final blow.";
		else
			message = message + "and hits.";
	}
	else // if the hit does not land
		message = message + "and misses."; 

	return message;
}


void Actor::checkHP()  // if the player has HP less than or equal to 0, he is dead
{ if (m_hp <= 0) setDead(); }

void Actor::gainHP() // player and dragon and regain health, only if the health is less than the max health
{ if (randInt(1,10) == 1 && m_hp < m_maxHP) m_hp++; }

void Player::pickUp(GameObject* object)
{
	if (m_inventory.size() < 26) // can only hold 25 items or less
	{
		m_inventory.push_back(object);
	}
	else
	{
		return;
	}
}

GameObject* Player::getObject(int i)
{
	return m_inventory[i];
}

std::vector<GameObject*> Player::getInventory()
{
	return m_inventory;
}

bool Player::wield(GameObject* go)
{
	Weapon* w = dynamic_cast<Weapon*>(go);  // try to convert to weapon
	if (w == nullptr) // if the pointer is a scroll, then you cannot wield a scroll
	{
		return false;
	}
	else  // if result is not null, go really points to a weapon
	{
		if (w->getNameInt() == SHORTSWORD || w->getNameInt() == MACE)
		{
			setDamageBonus(2); // these are the bonuses for each weapon
			setDexBonus(0);
		}
		if (w->getNameInt() == LONGSWORD)
		{
			setDamageBonus(4);
			setDexBonus(2);
		}
		if (w->getNameInt() == MAGICAXE)
		{
			setDamageBonus(5);
			setDexBonus(5);
		}
		if (w->getNameInt() == MAGICFANG) // has a sleep time but the sleep time is determined at the time of hit
		{								// if it lands, not when the weapon is initially wielded
			setDamageBonus(2);
			setDexBonus(3);
		}
		return true;
	}
}

string Player::accessWield()
{
	string str = "abcdefghijklmnopqrstuvwxyz"; // string of valid characters in the inventory
	char input = getCharacter();
	if (input - 97 >= m_inventory.size() || input - 97 < 0) // if the inputted character is not a-z
	{
		return "";
	}
	else
	{
		for (size_t i = 0; i < m_inventory.size(); i++)
		{
			if (input == str.at(i)) // takes the input that is equal to the str's ith position
			{
				string s = "";
				if (!wield(m_inventory[i])) // if wield returns false, then it is a scroll
				{
					GameObject* o = getObject(i);
					s = "You can't wield a " + o->getName();

				}
				else // if wield returns true, then it is a weapon
				{
					GameObject* o = getObject(i);
					setWeapon(i);
					s = "You wield " + o->getName();
				}
				setWieldString(s); // sets the player's wielding string to created message so it can be displayed
				setWielding(true); // this is set to true when something is wielded so it can be displayed on the next turn
									// after one turn of being displayed, this is set to false in Game so it is only displayed once for each wield
				return s;
			}
			else continue; // g32 kept giving stupid warnings and errors about not all paths returning a value
		}
		return "";
	}
}

bool Player::read(GameObject* go)
{
	Scroll* s = dynamic_cast<Scroll*>(go);  // try to convert to scroll
	if (s == nullptr) // if the pointer is a weapon, you cannot wield a weapon
	{
		return false;
	}
	else  // if result is not null, go points to a scroll
	{
		if (s->getNameInt() == S_HEALTH) // if health scroll 
		{
			setMaxHp(getMaxHp() + s->increaseMaxHP()); // adds to the max hp, 3-8 points
		}
		if (s->getNameInt() == S_ARMOR)
		{
			setArmor(armor() + s->increaseArmor()); // armor scroll adds armor, 1-3 points
		}
		if (s->getNameInt() == S_DEXTERITY)
		{
			setDexterity(dexterity() + s->increaseDexterity()); // dexterity adds 1 point
		}
		if (s->getNameInt() == S_STRENGTH)
		{
			setStrength(strength() + s->increaseStrength()); //  strength adds 1-3 points
		}
		if (s->getNameInt() == S_TP)
		{
			d()->teleportPlayer(); // tps to random position, checks if the new position is not occupied by an actor or wall
		}
		return true;
	}
}

string Player::accessScroll()
{
	string str = "abcdefghijklmnopqrstuvwxyz"; // same process as wield
	char input = getCharacter();
	if (input - 97 >= m_inventory.size() || input - 97 < 0)
	{
		return "";
	}
	else
	{
		for (size_t i = 0; i < m_inventory.size(); i++)
		{
			if (input == str.at(i)) // if the input is matched to the object's position in the inventory
			{
				string s = "";
				if (!read(m_inventory[i]))
				{
					GameObject* o = getObject(i);
					s = "You can't read a " + o->getName();
				}
				else
				{
					GameObject* o = getObject(i);
					s = "You read the scroll called " + o->getName();
					Scroll* scroll = dynamic_cast<Scroll*>(o);  // try to convert to scroll
					std::string scrollstr = scroll->getAction();
					setActionString(scrollstr);
					std::vector<GameObject*>::iterator it = m_inventory.begin(); // only with reading, scrolls are deleted
					delete m_inventory[i];
					m_inventory.erase(it + i); // deletes the scroll at the inputted position
				}
				setReadString(s); // sets the reading string and the bool to true so it can be displayed for one turn after it is read
				setReading(true);
				return s;
			}
			else continue;
		}
		return "";
	}
}

Player::~Player()
{
	std::vector<GameObject*>::iterator it = m_inventory.begin();
	int i = 0;
	while (m_inventory.size() != 0) // deletes the entire inventory since all objects were dynamically allocated, even the initial weapon
	{
		delete* it;
		it = m_inventory.erase(it);
		i++;
	}
}


void Snakewoman::attemptMove()
{
	int prow = m_playertracker->r(); // same as Bogeyman, knows where the player is because the player is passed into the snakewoman constructor when she is created
	int pcol = m_playertracker->c();
	int rdistance = abs(prow - r());
	int cdistance = abs(pcol - c());

	int distancetoplayer = rdistance + cdistance;

	int row = r();
	int col = c();

	if (distancetoplayer == 1) // if the player is 1 away from the monster, they are next to eachother, Snakewoman attacks
	{
		//west
		if (isPlayer(row, col - 1))
			setMessage(attack());
		//east
		if (isPlayer(row, col + 1))
			setMessage(attack());
		//north
		if (isPlayer(row - 1, col))
			setMessage(attack());
		//south
		if (isPlayer(row + 1, col))
			setMessage(attack());
	}
	else
	{
		if (distancetoplayer < 4) // same process as Bogeyman, but the smell distance is 3
		{
			// north
			int north_r = row - 1;
			int new_rdistance = abs(prow - north_r);
			int newdistancetoplayer = new_rdistance + cdistance;
			if (newdistancetoplayer < distancetoplayer && d()->getCellStatus(north_r, col) != WALL 
				&& !isMonster(north_r, col))
			{
				move(NORTH);
				setMessage("");
			}

			else
			{
				// south
				int south_r = row + 1;
				new_rdistance = abs(prow - south_r);
				newdistancetoplayer = new_rdistance + cdistance;
				if (newdistancetoplayer < distancetoplayer && d()->getCellStatus(south_r, col) != WALL 
					&& !isMonster(south_r, col))
				{
					move(SOUTH);
					setMessage("");
				}

				else
				{
					// west
					int west_c = col - 1;
					int new_cdistance = abs(pcol - west_c);
					newdistancetoplayer = rdistance + new_cdistance;
					if (newdistancetoplayer < distancetoplayer && d()->getCellStatus(row, west_c) != WALL
						&& !isMonster(row, west_c))
					{
						move(WEST);
						setMessage("");
					}
					else
					{
						// east
						int east_c = col + 1;
						new_cdistance = abs(pcol - east_c);
						newdistancetoplayer = rdistance + new_cdistance;
						if (newdistancetoplayer < distancetoplayer && d()->getCellStatus(row, east_c) != WALL
							&& !isMonster(row, east_c))
						{
							move(EAST);
							setMessage("");
						}
					}
				}
			}
		}
	}
}

bool Snakewoman::isPlayer(int r, int c)
{
	if (m_playertracker->r() == r && m_playertracker->c() == c)
		return true;
	else
		return false;
}

string Snakewoman::attack()
{
	string message = "the Snakewoman strikes magic fangs at Player ";

	int attackerPoints = 6; // Snakewoman dexterity and weapon dex bonus is 3 + 3
	int defenderPoints = m_playertracker->dexterity() + m_playertracker->armor();
	if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))
	{
		// weapon is MAGICFANG, which has stength of 2 in addition to Snakewoman strength which is 2 and then minus 1
		int damagePoints = randInt(0, 3); 
		int sleepPoss = randInt(1, 5); // have 1 in 5 chance of hitting a sleep
		m_playertracker->decreaseHP(damagePoints); // always damages the player if it hits

		if (m_playertracker->hp() <= 0)
			message = message + "dealing a final blow.";
		else
		{
			if (sleepPoss == 1) // if the sleep hits
			{
				int sleepTime = randInt(2, 6); // sleepTime for randInt amount of turns
				if (m_playertracker->sleepTime() > 0) // if already asleep
				{
					if (sleepTime > m_playertracker->sleepTime())
					{
						m_playertracker->setSleepTime(sleepTime); // the sleepTime is the maximum of the initial time and new time
					} // if the new sleep time is equal or less, then the sleepTime stays as the original time
				}
				else // if the player is not asleep
				{
					m_playertracker->setSleepTime(sleepTime);
				}
				message += "putting the Player to sleep.";
			}
			else
			{
				message += "and hits.";
			}
		}
	}
	else
	{
		message += "and misses.";
	}
	return message;
}

void Goblin::tryMove(int dir) // same process as the past monster moves
{
	int prow = m_playertracker->r(); // same as Bogeyman, knows where the player is because the player is passed into the snakewoman constructor when she is created
	int pcol = m_playertracker->c();
	int rdistance = abs(prow - r());
	int cdistance = abs(pcol - c());

	int distancetoplayer = rdistance + cdistance;

	int row = r();
	int col = c();

	if (sleepTime() == 0) // can only move if not asleep
	{
		if (distancetoplayer == 1) // if the player is 1 away from the monster, they are next to eachother, Snakewoman attacks
		{
			//west
			if (isPlayer(row, col - 1))
				setMessage(attack());
			//east
			if (isPlayer(row, col + 1))
				setMessage(attack());
			//north
			if (isPlayer(row - 1, col))
				setMessage(attack());
			//south
			if (isPlayer(row + 1, col))
				setMessage(attack());
		}
		else
		{
			switch (dir)
			{
			case WEST:
				if (d()->getCellStatus(row, col - 1) != WALL && !isMonster(row, col - 1))
				{
					Actor::move(dir);
					setMessage("");
				}
				break;

			case EAST:
				if (d()->getCellStatus(row, col + 1) != WALL && !isMonster(row, col + 1))
				{
					Actor::move(dir);
					setMessage("");
				}
				break;

			case NORTH:
				if (d()->getCellStatus(row - 1, col) != WALL && !isMonster(row -1, col))
				{
					Actor::move(dir);
					setMessage("");
				}
				break;

			case SOUTH:
				if (d()->getCellStatus(row + 1, col) != WALL && !isMonster(row + 1, col))
				{

					Actor::move(dir);
					setMessage("");
				}
				break;
			default:
				break;
			}
		}
	}
}

void Goblin::attemptMove()
{
	int prow = m_playertracker->r();
	int pcol = m_playertracker->c();

	int grow = r();
	int gcol = c();

	int goblinGrid[18][70];

	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 70; j++)
		{
			if (d()->getCellStatus(i, j) == WALL)
			{
				goblinGrid[i][j] = 100; // any int that stays consistent so the goblin knows what is seen or unreachable
			}
			else
			{
				goblinGrid[i][j] = 20; // if it is reachable
			}
		}
	}

	for (Actor* aptr : d()->getMonsters())
	{
		goblinGrid[aptr->r()][aptr->c()] = 100; // the monsters are basically walls, which are unreachable, since the goblin cannot go through them
	}

	int min_dist = 100;

	int direction = findBestDirection(goblinGrid, grow, gcol, prow, pcol, min_dist, 0);

	switch (direction) {
	case 0:
		tryMove(SOUTH);
		break;
	case 1:
		tryMove(NORTH);
		break;
	case 2:
		tryMove(EAST);
		break;
	case 3:
		tryMove(WEST);
		break;
	default:
		break;
	}
}

bool Goblin::openSpot(int grid[18][70], int x, int y, int dist)
{
	if (grid[x][y] == 100) // unreachable, like wall or monster
		return false;
	else if (dist < grid[x][y])
		return true;
	else
		return false;
}

void Goblin::findPath(int grid[18][70], int sr, int sc, int er, int ec, int& min_dist, int dist)
{
	if (sr == er && sc == ec)
	{
		min_dist = min(dist, min_dist);
		return;
	}
	if (dist > 15)
		return;

	grid[sr][sc] = dist;
	if (openSpot(grid, sr + 1, sc, dist))
		findPath(grid, sr + 1, sc, er, ec, min_dist, dist + 1);

	if (openSpot(grid, sr - 1, sc, dist))
		findPath(grid, sr - 1, sc, er, ec, min_dist, dist + 1);

	if (openSpot(grid, sr, sc + 1, dist))
		findPath(grid, sr, sc + 1, er, ec, min_dist, dist + 1);

	if (openSpot(grid, sr, sc - 1, dist))
		findPath(grid, sr, sc - 1, er, ec, min_dist, dist + 1);
}

int Goblin::findBestDirection(int grid[18][70], int sr, int sc, int er, int ec, int& min_dist, int dist)
{
	int directions[4] = { 100,100,100,100 };

	if (openSpot(grid, sr + 1, sc, dist)) // south
	{
		findPath(grid, sr + 1, sc, er, ec, min_dist, 0);
		directions[0] = min_dist;
	}

	if (openSpot(grid, sr - 1, sc, dist)) // north
	{
		min_dist = 100;
		findPath(grid, sr - 1, sc, er, ec, min_dist, 0);
		directions[1] = min_dist;
	}

	if (openSpot(grid, sr, sc + 1, dist)) // east 
	{
		min_dist = 100;
		findPath(grid, sr, sc + 1, er, ec, min_dist, 0);
		if (min_dist < 15)
			directions[2] = min_dist;
	}
	if (openSpot(grid, sr, sc - 1, dist)) // west
	{
		min_dist = 100;
		findPath(grid, sr, sc - 1, er, ec, min_dist, 0);
		if (min_dist < 15)
			directions[3] = min_dist;
	}

	int minimum = 100;
	int minindex = 0;
	for (int i = 0; i < 4; i++)
	{
		if (directions[i] < minimum)
		{
			minimum = directions[i];
			minindex = i;
		}
	}
	if (directions[minindex] < 15) // if is it less than 15, then the player is reachable
		return minindex;
	else // else the goblin shouldnt move, the direction -1 is not a movable direction
		return -1;
}

string Goblin::attack()
{
	string message = "the Goblin slashes short sword at Player "; // goblin always has same weapon

	int attackerPoints = 1; // short sword has 0 dexterity bonus, goblin has 1 dex
	int defenderPoints = m_playertracker->dexterity() + m_playertracker->armor();
	if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))
	{
		int damagePoints = randInt(0, 4); // goblin has 3 strength, shortsword has 2 extra strength, minus 1
		m_playertracker->decreaseHP(damagePoints); // always does damage if a hit

		if (m_playertracker->hp() <= 0)
			message += "dealing a final blow.";
		else
			message += "and hits.";
	}
	else
	{
		message += "and misses.";
	}
	return message;
}

void Dragon::attemptMove() // Dragon doesnt actually move, his "move" is just his turn where he attacks if the player is next to him
{
	int prow = m_playertracker->r(); // knows where player is
	int pcol = m_playertracker->c();
	int rdistance = abs(prow - r());
	int cdistance = abs(pcol - c());

	int distancetoplayer = rdistance + cdistance;

	int row = r();
	int col = c();
	gainHP(); // gains hp, 1 in 10 chance each turn
	if (distancetoplayer == 1)
	{
		//west
		if (isPlayer(row, col - 1))
			setMessage(attack());
		//east
		if (isPlayer(row, col + 1))
			setMessage(attack());
		//north
		if (isPlayer(row - 1, col))
			setMessage(attack());
		//south
		if (isPlayer(row + 1, col))
			setMessage(attack());
	}
}

string Dragon::attack()
{
	string message = "the Dragon swings long sword at Player "; // always has long sword

	int attackerPoints = 6; // dragon dexterity is 4, long sword dex is 2
	int defenderPoints = m_playertracker->dexterity() + m_playertracker->armor();
	if (randInt(1, attackerPoints) >= randInt(1, defenderPoints))
	{
		int damagePoints = randInt(0, 7); // dragon strength 4, long sword damage 4, minus 1
		m_playertracker->decreaseHP(damagePoints); // damages player

		if (m_playertracker->hp() <= 0)
			message += "dealing a final blow.";
		else
			message += "and hits.";
	}
	else
	{
		message += "and misses.";
	}
	return message;
}

bool Goblin::isPlayer(int r, int c)
{
	if (m_playertracker->r() == r && m_playertracker->c() == c) 
		return true;
	else
		return false;
}

bool Dragon::isPlayer(int r, int c)
{
	if (m_playertracker->r() == r && m_playertracker->c() == c)
		return true;
	else
		return false;
}
