#pragma once
#include <string>
#include "ConfigFile.h"
#include <irrlicht.h>

class EntityType {
public:
	EntityType(std::string, ConfigFile*);
	EntityType();
private:
	bool usable;
	int hp;
	int travelMask; // Allows for moddability of tile types; 1 = ground; 2 = water; 4 = lava; 8 = rock; 16 = something else...
	std::string typeName;
	std::string modelFilename;
	irr::scene::IMesh *mesh;
	int drillPower; // Which walls it can drill
	int drillSpeed; // How fast it can drill
	bool mobile;
	bool userControlled;
	enum AIType {MONSTER, RAIDER, VEHICLE, NONE} AIType;
};
