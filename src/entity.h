#pragma once
#include <irrlicht.h>
#include <string>

#define FALLBACK_MESH_NAME "data/models/can.dae"

using namespace irr;
using irr::core::vector3df;
using irr::core::vector2df;

class Entity {
public:
	Entity(Entity*);
	Entity(vector2df);
	scene::IAnimatedMeshSceneNode *node;
	void setTarget(vector2df);
private:
	scene::IAnimatedMesh *mesh;
	const static io::path meshName;
	vector2df map_position;
};
