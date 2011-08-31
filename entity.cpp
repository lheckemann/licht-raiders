#include <irrlicht.h>
#include "enttype.h"
#include "entity.h"

using irr::core::vector3df;

Entity::Entity(EntityType *type, irr::scene::ISceneManager *smgr, vector3df pos) {
	if (!type->usable) {return NULL;}
	hp = type->hp;
	travelMask = type->travelMask;
	typeName = type->typeName;
	drillPower = type->drillPower;
	drillSpeed = type->drillSpeed;
	mobile = type->mobile;
	userControlled = type->userControlled;
	AIType = type->AIType;
	model = smgr->addAnimatedMeshSceneNode(type->mesh, 0, -1, pos);
}
