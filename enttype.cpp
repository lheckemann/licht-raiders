#include "enttype.h"
#include "ConfigFile.h"
#include <string>
#include <iostream>

EntityType::EntityType() {
	usable = false;
	hp = 0;
	typeName = "";
	travelMask = 0;
	drillPower = -1;
	drillSpeed = -1;
	mobile = false;
	userControlled = false;
	AIType = NONE;
}

EntityType::EntityType(std::string name, ConfigFile *cfg) {
	typeName = name;
	usable = true;
	hp = cfg->read<int>(name + ".hp", 100);
	travelMask = cfg->read<int>(name + ".travelMask", 0);
	drillPower = cfg->read<int>(name + ".drillPower", -1);
	drillSpeed = cfg->read<int>(name + ".drillSpeed", -1);
	mobile = cfg->read<bool>(name + ".mobile", false);
	userControlled = cfg->read<bool>(name + ".isUnit", false);
	modelFilename = cfg->read<std::string>(name + ".model", "");

	std::string _AIType;
	_AIType = cfg->read<std::string>(name + ".AIType", "NONE");
	if (_AIType == "MONSTER") {
		AIType = MONSTER;
	}
	else if (_AIType == "RAIDER") {
		AIType = RAIDER;
	}
	else if (_AIType == "VEHICLE") {
		AIType = VEHICLE;
	}
	else if (_AIType == "NONE") {
		AIType = NONE;
	}
	else {
		std::cout << "Unsupported AI Type: " << _AIType << "\nFalling back to NONE.\n";
		AIType = NONE;
	}
}
