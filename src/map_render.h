#pragma once
#include <irrlicht.h>
#include <string>
#include <vector>
#include "map.h"

using namespace irr;

#define get_index_for_tile(x, y, map) (y*map.width)+x
#define get_coords_at_index(index, map) {map.width%index, map.width/index}

video::ITexture *wallTextures;

const std::string texture_names[] = {
	"ground",
	"dirt",
	"loose",
	"hard",
	"solid",
	"eseam",
	"oseam",
	"water",
	"lava"
}
const bool tile_is_wall[] = {
	false,
	true,
	true,
	true,
	true,
	true, 
	true,
	false,
	false
}

typedef int[2] mapCoords;

void load_textures();
void calculate_render(Map*);
