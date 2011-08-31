#pragma once
#include "enttype.h"
#include <irrlicht.h>
#include <string>

using irr::core::vector3df;

class Entity {
private:
	int hp;
	int travelMask; // Allows for moddability of tile types; 1 = ground; 2 = water; 4 = lava; 8 = ???
	std::string typeName;
	int drillPower; // Which walls it can drill
	int drillSpeed; // How fast it can drill
	bool mobile;
	bool userControlled;
	enum AIType {MONSTER, RAIDER, VEHICLE, NONE} AIType;
	irr::scene::IAnimatedMeshSceneNode *model;
public:
	Entity(EntityType*, irr::scene::ISceneManager*, vector3df);
};
