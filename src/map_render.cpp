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
		
		
/* DO NOT LOOK AT THE FOLLOWING CODE!!!! */
		//	calculate surroundings
		surround_coords = {
			{current[0]-1, current[1]-1}, {current[0]  , current[1]-1}, {current[0]+1, current[1]-1},
			{current[0]-1, current[1]  }, {current[0]  , current[1]  }, {current[0]+1, current[1]  },
			{current[0]-1, current[1]+1}, {current[0]  , current[1]+1}, {current[0]+1, current[1]+1}
		}
		// calculate surrounding indexes
		surround_indexes[0] = get_index_for_tile(surround_coords[0][0], surround_coords[0][1]);
		surround_indexes[1] = get_index_for_tile(surround_coords[1][0], surround_coords[1][1]);
		surround_indexes[2] = get_index_for_tile(surround_coords[2][0], surround_coords[2][1]);
		surround_indexes[3] = get_index_for_tile(surround_coords[3][0], surround_coords[3][1]);
		surround_indexes[4] = get_index_for_tile(surround_coords[4][0], surround_coords[4][1]);
		surround_indexes[5] = get_index_for_tile(surround_coords[5][0], surround_coords[5][1]);
		surround_indexes[6] = get_index_for_tile(surround_coords[6][0], surround_coords[6][1]);
		surround_indexes[7] = get_index_for_tile(surround_coords[7][0], surround_coords[7][1]);
		surround_indexes[8] = get_index_for_tile(surround_coords[8][0], surround_coords[8][1]);
		// get wall bools
		for (int x = 0; x < 9 ; ++x) {
			surround_types[x] = tile_is_wall[map->tiles[surround_indexes[x]].type];
		}
/* It's okay, you can look again. Wait, no.*/
		
	}
}
