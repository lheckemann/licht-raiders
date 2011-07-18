#pragma once
#include <string>
#include "ConfigFile.h"

class EntityType {
public:
	EntityType(std::string, ConfigFile*);
	EntityType();
private:
	bool usable;
	int hp;
	int travelMask; // Allows for moddability of tile types; 1 = ground; 2 = water; 4 = lava; 8 = ???
	std::string typeName;
	std::string modelFilename;
	int drillPower; // Which walls it can drill
	int drillSpeed; // How fast it can drill
	bool mobile;
	bool userControlled;
	enum AIType {MONSTER, RAIDER, VEHICLE, NONE} AIType;
};
