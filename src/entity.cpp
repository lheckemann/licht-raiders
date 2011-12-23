#include <irrlicht.h>
#include "entity.h"
#include "globs.h"
#include "map.h"

using irr::core::vector3df;

const io::path Entity::meshName("data/models/can.dae");

Entity::Entity(vector2df origin) {
	mesh = smgr->getMesh(meshName);
	if (!mesh) mesh = smgr->getMesh(FALLBACK_MESH_NAME);
	if (!mesh) bork("Couldn't load fallback mesh");
	map_position = origin;
	vector3df globalPos(origin.X*TILE_SIZE, 0, origin.Y*TILE_SIZE);
	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setPosition(globalPos);
	NodeOwner *x = new NodeOwner;
	x->ownerType = NodeOwner::TYPE_ENTITY;
	x->entityOwner = this;
	node->setUserData(x);
}

Entity::Entity(Entity* origin) {
	Entity(origin->map_position);
}
