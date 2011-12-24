#pragma once
#include <irrlicht.h>
#include <string>
#include <deque>
#define FALLBACK_MESH_NAME "data/models/can.dae"

using namespace irr;
using irr::core::vector3df;
using irr::core::vector2df;
class Entity {
public:
	Entity(Entity*);
	Entity(vector2df);
	scene::IAnimatedMeshSceneNode *node;
	vector2df target;
	std::deque<vector2df> path;
	bool setTarget(vector2df targ);
	void update();
private:
	scene::IAnimatedMesh *mesh;
	const static io::path meshName;
	vector2df map_position;
};
