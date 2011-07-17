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
	hp = cfg->read<int>(name + ".hp");
	try {
		travelMask = cfg->read<int>(name + ".travelMask");
	}
	catch (ConfigFile::key_not_found) {
		travelMask = 0;
	}
	try {
		drillPower = cfg->read<int>(name + ".drillPower");
	}
	catch (ConfigFile::key_not_found) {
		drillPower = -1; // Cannot drill
	}
	try {
		drillSpeed = cfg->read<int>(name + ".drillSpeed");
	}
	catch (ConfigFile::key_not_found) {
		drillSpeed = -1; // Cannot drill
	}
	try {
		mobile = cfg->read<bool>(name + ".mobile");
	}
	catch (ConfigFile::key_not_found) {
		mobile = 0;
	}
	try {
		userControlled = cfg->read<bool>(name + ".isUnit");
	}
	catch (ConfigFile::key_not_found) {
		userControlled = 0;
	}
	std::string _AIType;
	try {
		_AIType = cfg->read<std::string>(name + ".AIType");
	}
	catch (ConfigFile::key_not_found) {
		_AIType = "NONE";
	}
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
	usable = true;
}
