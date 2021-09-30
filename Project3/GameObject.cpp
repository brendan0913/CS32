#include "GameObject.h"
#include "GameConstants.h"
#include "utilities.h"

Weapon::Weapon(int r, int c, int name) : GameObject(r,c,name), m_actionName(""),
m_increaseDexterity(0), m_damageAmount(0), m_sleepTime(0) // all these bonuses are 0 at first
{
	switch (name)
	{
	case SHORTSWORD:
		setACTION("slashes short sword at");
		setDMG(2); // damage bonus
		break;
	case LONGSWORD:
		setACTION("swings long sword at");
		setDMG(4);
		setDEX(2);
		break;
	case MACE:
		setACTION("swings mace at");
		setDMG(2); // same as shortsword
		break;
	case MAGICAXE: // magic axe damage and dexterity bonus OP
		setACTION("chops magic axe at");
		setDMG(5);
		setDEX(5);
		break;
	case MAGICFANG:
		setACTION("strikes magic fangs at");
		setDMG(2);
		setDEX(3);
		setST(0); // sleep time is determined at time of hit, rather than at creation of the weapon
		break;
	default:
		break;
	}
}

std::string GameObject::getName()
{
	switch (m_name) // turns the int name to a string
	{
		case MACE:
			return "mace";
			break;
		case SHORTSWORD:
			return "short sword";
			break;
		case LONGSWORD:
			return "long sword";
			break;
		case MAGICAXE:
			return "magic axe";
			break;
		case MAGICFANG:
			return "magic fangs of sleep";
			break;
		case S_HEALTH:
			return "scroll of enhance health";
			break;
		case S_ARMOR:
			return "scroll of enhance armor";
			break;
		case S_STRENGTH:
			return "scroll of strength";
			break;
		case S_DEXTERITY:
			return "scroll of enhance dexterity";
			break;
		case S_TP:
			return "scroll of teleportation";
			break;
		default:
			return "";
			break;
	}
}

Scroll::Scroll(int r, int c, int name) : GameObject(r, c, name), m_actionName(""), 
m_increaseDexterity(0), m_incMaxHP(0), m_increaseArmor(0), m_increaseStrength(0)
{
	switch (name)
	{
	case S_HEALTH:
		setACTION("You feel your heart beating stronger.");
		setMAXHP(randInt(3,8));
		break;
	case S_DEXTERITY:
		setACTION("You feel like less of a klutz.");
		setDEX(1);
		break;
	case S_ARMOR:
		setACTION("Your armor glows blue.");
		setARMOR(randInt(1,3));
		break;
	case S_STRENGTH:
		setACTION("Your muscles bulge.");
		setSTRENGTH(randInt(1,3));
		break;
	case S_TP:
		setACTION("You feel your body wrenched in space and time.");
		break;
	default:
		break;
	}
}