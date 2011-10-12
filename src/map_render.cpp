#include "map_render.h"

#define index (i - map->tiles.begin())

std::vector<video::ITexture*> wallTextures;

scene::IMesh *wallMesh;
scene::IMesh *groundMesh;

void load_textures() {
	io::path path;
	for (unsigned i = 0; i < sizeof(texture_names)/sizeof(std::string); i++) {
		path = ("data/textures/tile/" + texture_names[i] + ".png").c_str();
		wallTextures.push_back(driver->getTexture(path));
	}
	wallMesh = smgr->getMesh("data/models/wall.obj");
	groundMesh = smgr->getMesh("data/models/ground.obj");
}

void calculate_render(Map* map) {
	std::vector<Tile>::iterator i;
	mapCoords current;
	current.x = 0;
	current.y = 0;
/*	mapCoords surround_coords[9];
	int surround_indexes[9];
	bool surround_walls[9];*/
	scene::IMeshSceneNode *sceneNode;

	for (i = map->tiles.begin(); i != map->tiles.end(); i++) {
//		current = get_coords_at_index(i - map->tiles.begin(), *&map); // TODO fix get_coords_at_index macro
		current.x = map->width % index;
		current.y = map->width / index;

		if (tile_is_wall[i - map->tiles.begin()]) {
			sceneNode = smgr->addMeshSceneNode(wallMesh);
		}
		else {
			sceneNode = smgr->addMeshSceneNode(groundMesh);
		}
		sceneNode->setMaterialTexture(0, wallTextures[i - map->tiles.begin()]);
		sceneNode->setMaterialType(video::EMT_SOLID);
		//	calculate surrounding coordinates
/*		surround_coords = {
			{current[0]-1, current[1]-1}, {current[0]  , current[1]-1}, {current[0]+1, current[1]-1},
			{current[0]-1, current[1]  }, {current[0]  , current[1]  }, {current[0]+1, current[1]  },
			{current[0]-1, current[1]+1}, {current[0]  , current[1]+1}, {current[0]+1, current[1]+1}
		}
		// get wall bools
		for (int x = 0; x < 9 ; ++x) {
			surround_indexes[x] = get_index_for_tile(surround_coords[x][0], surround_coords[x][1]);
			surround_types[x] = tile_is_wall[map->tiles[surround_indexes[x]].type];
		}*/
	}
}
