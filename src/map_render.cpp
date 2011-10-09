#include "map_render.h"

void load_textures() {
	wallTextures = malloc(sizeof(video::ITexture) * (sizeof(texture_names)/sizeof(std::string)));
	for (int i = 0; i < sizeof(texture_names)/sizeof(std::string); i++) {
		wallTextures[i] = driver->getTexture("data/textures/tile/" + texture_names[i] + ".png");
	}
}

void calculate_render(Map* map) {
	std::vector<Tile>::iterator i;
	int index;
	mapCoords current = {0, 0}
	mapCoords surround_coords[9];
	int surround_indexes[9];
	bool surround_walls[9];
	
	
	for (i = map->tiles.begin(); i != map->tiles.end(); i++) {
		coords = get_coords_at_index(i - map->tiles.begin());
		
		//	calculate surrounding coordinates
		surround_coords = {
			{current[0]-1, current[1]-1}, {current[0]  , current[1]-1}, {current[0]+1, current[1]-1},
			{current[0]-1, current[1]  }, {current[0]  , current[1]  }, {current[0]+1, current[1]  },
			{current[0]-1, current[1]+1}, {current[0]  , current[1]+1}, {current[0]+1, current[1]+1}
		}
		// get wall bools
		for (int x = 0; x < 9 ; ++x) {
			surround_indexes[x] = get_index_for_tile(surround_coords[x][0], surround_coords[x][1]);
			surround_types[x] = tile_is_wall[map->tiles[surround_indexes[x]].type];
		}
	}
}
