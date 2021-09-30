#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include "utilities.h"

const int NORTH = 0; // directions
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int SEEN = 69;

const int EMPTY = 0; // cell statuses
const int WALL = 1;
const int PLAYER = 2;
const int STAIR = 3;
const int WEAPON = 4;
const int SCROLL = 5;
const int IDOL = 6;
const int GOBLIN = 7;
const int BOGEYMAN = 8;
const int SNAKEWOMAN = 9;
const int DRAGON = 10;
const int ACTOR = 11;

const int MACE = 100; // weapon numbers
const int SHORTSWORD = 101;
const int LONGSWORD = 102;
const int MAGICFANG = 103;
const int MAGICAXE = 104;

const int S_HEALTH = 105; // scroll numbers
const int S_ARMOR = 106;
const int S_DEXTERITY = 107;
const int S_STRENGTH = 108;
const int S_TP = 109;

#endif // !GAMECONSTANTS_H
