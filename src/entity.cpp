#include <irrlicht.h>
#include "entity.h"
#include "globs.h"
#include "map.h"
#include "astar.h"
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

bool Entity::setTarget(vector2df targ) {
    // TODO pathfinding code
    // convert the path into coordinates
    // mapposition converted to tile coords, skip first tile -> next tile ... -> skip last tile, directly go to targ
    // optional: bezier curves :)
    target = targ;
    
    Tile* starttile = map->tiles[map_position.Y/TILE_SIZE+map_position.X/TILE_SIZE];
    Tile* endtile = map->tiles[targ.X/TILE_SIZE+targ.X/TILE_SIZE];
    std::deque<Tile*> tilepath = calculate_path(starttile, endtile);
    std::deque<Tile*>::iterator itr;
    vector2df coords;
    for (itr = tilepath.begin(); itr != tilepath.end(); itr++) {
        coords.X = (*itr)->x*TILE_SIZE;
        coords.Y = (*itr)->y*TILE_SIZE;
        path.push_back(coords);
    }
}
