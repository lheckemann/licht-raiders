#pragma once
#include "enttype.h"
#include <irrlicht.h>
#include <string>

using irr::core::vector3df;

class Entity {
private:
	int id;
public:
	virtual void Entity() = 0;
};
