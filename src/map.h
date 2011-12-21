#pragma once
#include <vector>
#include <cstdio>
#include <stdint.h>
#include <irrlicht.h>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>

#include "object.h"

using namespace irr;
using irr::core::vector3df;

#define get_index_for_tile(x, y, map) (y*map.width)+x
#define get_coords_at_index(index, map) {map.width%index, map.width/index}

extern std::vector<video::ITexture*> tileTextures;
extern std::vector<video::ITexture*> tileTextures_sel;

extern scene::IMesh *wallMesh;
extern scene::IMesh *groundMesh;
extern std::vector<scene::IMeshSceneNode*> tileSceneNodes;

struct Tiledata {
	uint32_t type;
	uint32_t height;
	uint32_t point;
};

struct Pixel {
	char B, G, R, A;
};

class Tile : public Object {
public:
	Tiledata data;
	scene::IMeshSceneNode *scn;
	Tile(Tiledata, int, int);
};


class Map {
public:
	void load_tiles(FILE*);
	void load_textures();
	void calculate_render();
	void load(FILE*);
	std::vector<Tiledata> tiledatas;
	std::vector<Tile*> tiles;
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

// I made this for an experiment with addWaterSurfaceSceneNode; It kind of failed :D
const bool tile_is_liquid[] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
	true
};

struct mapCoords {int x,y;};
