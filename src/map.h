#pragma once
#include <vector>
#include <cstdio>
#include <stdint.h>
#include <irrlicht.h>
#include <string>
#include <vector>
#include "globs.h"

#include <cstring>
#include <cassert>

using namespace irr;
using irr::core::vector3df;

#define get_index_for_tile(x, y, map) (y*map.width)+x
#define get_coords_at_index(index, map) {map.width%index, map.width/index}

extern std::vector<video::ITexture*> wallTextures;

extern scene::IMesh *wallMesh;
extern scene::IMesh *groundMesh;
extern std::vector<scene::IMeshSceneNode*> tileSceneNodes;

struct Tile{
	uint32_t type;
	uint32_t height;
	uint32_t point;
};

class Map {
public:
	void load_tiles(FILE*);
	void load_textures();
	void calculate_render();
	void load(FILE*);
	std::vector<Tile> tiles;
	uint32_t width, height;
};


const std::string texture_names[] = {
	"ground",
	"dirt",
	"loose",
	"hard",
	"solid",
	"eseam",
	"oseam",
	"rseam",
	"water",
	"lava"
};
const bool tile_is_wall[] = {
	false,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	false
};

struct mapCoords {int x,y;};

void load_textures();
void calculate_render();
