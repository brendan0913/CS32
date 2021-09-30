#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>

class GameObject 
{
public:
	GameObject(int r, int c, int name) : m_r(r), m_c(c), m_name(name) {}

	std::string getName();
	int getNameInt() { return m_name; }
	void setr(int r) { m_r = r; }
	void setc(int c) { m_c = c; }
	virtual ~GameObject() {}
private:
	int m_r;
	int m_c;
	int m_name;
	std::string m_namestring;
};

class Weapon : public GameObject
{
public:
	Weapon(int r, int c, int name);
	//void setName(std::string name) { m_name = name; }

	int dexterityBonus() { return m_increaseDexterity; }
	int damageAmount() { return m_damageAmount; }
	std::string getAction() { return m_actionName; }
	void setACTION(std::string a) { m_actionName = a; }
	void setDMG(int d) { m_damageAmount = d; }
	void setDEX(int d) { m_increaseDexterity = d; }
	void setST(int s) { m_sleepTime = s; }
	virtual ~Weapon() {}
private:
	std::string m_actionName;
	std::string m_name;
	int m_increaseDexterity;
	int m_damageAmount;
	int m_sleepTime;
};

class Scroll : public GameObject
{
public:
	Scroll(int r, int c, int name);
	int increaseArmor() { return m_increaseArmor; }
	int increaseStrength() { return m_increaseStrength; }
	int increaseMaxHP() { return m_incMaxHP; }
	int increaseDexterity() { return m_increaseDexterity; }

	//int teleportToPlace() { return m_}

	std::string getAction() { return m_actionName; }

	void setACTION(std::string a) { m_actionName = a; }
	void setSTRENGTH(int d) { m_increaseStrength = d; }
	void setDEX(int d) { m_increaseDexterity = d; }
	void setARMOR(int s) { m_increaseArmor = s; }
	void setMAXHP(int h) { m_incMaxHP = h; }

	//void setTP(int r, int c) { }
	virtual ~Scroll() {}
private:
	std::string m_actionName;
	int m_increaseArmor;
	int m_incMaxHP;
	int m_increaseStrength;
	int m_increaseDexterity;
};

#endif // !GAMEOBJECT_H
