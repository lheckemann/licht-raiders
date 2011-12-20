#include "map.h"

std::vector<video::ITexture*> wallTextures;

scene::IMesh *wallMesh;
scene::IMesh *groundMesh;
std::vector<scene::IMeshSceneNode*> tileSceneNodes;


void Map::load_tiles(FILE* map) {
	Tile t;
	uint32_t type, tileheight, point;
	uint8_t tilesize, field;
	char* check = new char[3];
	check[2] = 0;
	uint16_t version;
	fread(check, 1, 2, map);
	fread(&version, 2, 1, map);
	assert(strcmp(check, "RR") == 0); // TODO: Replace with something that actually complains about an error rather than just aborting :)
	assert(version == 1);
	fread(&width, 4, 1, map);
	fread(&height, 4, 1, map);
	for (unsigned int i = 0; i < width*height; i++) { // For every tile
		fread(&tilesize, 1, 1, map); // Read number of fields in tile
		for (unsigned int j = 0; j < tilesize; j++) { // For every field in the current tile
			fread(&field, 1, 1, map); // Read field type
			t.type = 0;
			t.height = 0;
			t.point = 0;
			switch(field) { // Decide on field type and read into appropriate variable
				case 0: fread(&type, 4, 1, map); break; // Field type 0 = tile type
				case 1: fread(&tileheight, 4, 1, map); break; // Field type 1 = tile height
				case 2: fread(&point, 4, 1, map); break; // Field type 2 = tile number for scripting
//				case 3: fread(&asdf, 1, 1, map); break; // Field type 3 = ????
				default: fseek(map, 4, SEEK_CUR); // Skip the value of the field if its type is unknown
			}
			t.type = type;
			t.height = tileheight;
			t.point = point;
		}
		tiles.push_back(t);
	}
}

void Map::load(FILE* map) {
	load_tiles(map);
	calculate_render();
}

void Map::load_textures() {
	io::path path;
	for (unsigned i = 0; i < sizeof(texture_names)/sizeof(std::string); i++) {
		path = ("data/textures/tile/" + texture_names[i] + ".png").c_str();
		wallTextures.push_back(driver->getTexture(path));
		if (*(wallTextures.end()-1) == NULL) {
			bork((path + " could not be loaded").c_str());
		}
	}
	wallMesh = smgr->getMesh("data/models/wall.dae");
	groundMesh = smgr->getMesh("data/models/ground.dae");
}


#define index ((int)(i - tiles.begin()))
void Map::calculate_render() {
	load_textures();
	std::vector<Tile>::iterator i;
	mapCoords current;
	current.x = 0;
	current.y = 0;
/*	mapCoords surround_coords[9];
	int surround_indexes[9];
	bool surround_walls[9];*/
	scene::IMeshSceneNode *sceneNode;

	for (i = tiles.begin(); i != tiles.end(); i++) {
//		current = get_coords_at_index(i - tiles.begin(), *&this); // TODO fix get_coords_at_index macro
		current.x = index % width;
		current.y = index / width;

		if (tile_is_wall[tiles[index].type]) {
			sceneNode = smgr->addMeshSceneNode(wallMesh, 0, MAP_SCN_ID, vector3df(current.y*-2, 0, current.x*-2));
		}
		else {
			sceneNode = smgr->addMeshSceneNode(groundMesh, 0, MAP_SCN_ID, vector3df(current.y*-2, 0, current.x*-2));
		}
		sceneNode->setMaterialTexture(0, wallTextures[tiles[index].type]);
		sceneNode->setMaterialType(video::EMT_SOLID);
		sceneNode->setMaterialFlag(video::EMF_LIGHTING, UserConfig.read<bool>("display.lighting", true));
		sceneNode->setMaterialFlag(video::EMF_BILINEAR_FILTER, not UserConfig.read<bool>("display.minecraftmode", false)); // Minecraft!
		tileSceneNodes.push_back(sceneNode);
		//	calculate surrounding coordinates
/*		surround_coords = {
			{current[0]-1, current[1]-1}, {current[0]  , current[1]-1}, {current[0]+1, current[1]-1},
			{current[0]-1, current[1]  }, {current[0]  , current[1]  }, {current[0]+1, current[1]  },
			{current[0]-1, current[1]+1}, {current[0]  , current[1]+1}, {current[0]+1, current[1]+1}
		}
		// get wall bools
		for (int x = 0; x < 9 ; ++x) {
			surround_indexes[x] = get_index_for_tile(surround_coords[x][0], surround_coords[x][1]);
			surround_types[x] = tile_is_wall[tiles[surround_indexes[x]].type];
		}*/
	}
}



#if UNIT_TEST
#include <iostream>
int main () {
	Map map;
	FILE* f;
	f = fopen("test.map", "rb");
	map.load(f);
	std::vector<Tile>::iterator it = map.tiles.begin();
	int x, y;
	for(x = 0; x<map.width ; x++) {
		for (y = 0; y<map.height; y++) {
			std::cout << it->type << "\t";
			it++;
		}
		std::cout << "\n";
	}
}
#endif
