#include "map.h"
#include "globs.h"

std::vector<video::ITexture*> tileTextures;
video::ITexture *selected_tex;
video::ITexture *unselected_tex;

scene::IMesh *wallMesh;
scene::IMesh *groundMesh;
std::vector<scene::IMeshSceneNode*> tileSceneNodes;


void Map::load_tiles(FILE* map) {
	Tiledata t;
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
		tiledatas.push_back(t);
	}
}

void Map::load(FILE* map) {
	load_tiles(map);
	calculate_render();
}

void Map::load_textures() {
	io::path path;
	video::ITexture* active;
	unsigned char* texData;
	unsigned int pix;
	for (unsigned i = 0; i < sizeof(texture_names)/sizeof(std::string); i++) {
		path = ("data/textures/tile/" + texture_names[i] + ".png").c_str();
		active = driver->getTexture(path);
		if (active == NULL) {
			bork((path + " could not be loaded").c_str());
		}
		tileTextures.push_back(active);
	}

	selected_tex = driver->getTexture("data/textures/tile/selected.png");
	unselected_tex = driver->getTexture("data/textures/tile/normal.png");

	wallMesh = smgr->getMesh("data/models/wall.dae");
	groundMesh = smgr->getMesh("data/models/ground.dae");
}


#define getindex ((int)(i - tiledatas.begin()))
void Map::calculate_render() {
	load_textures();
	std::vector<Tiledata>::iterator i;
	core::vector2di current;
	current.X = 0;
	current.Y = 0;
	int index;
	Tile* tile;
/*	mapCoords surround_coords[9];
	int surround_indexes[9];
	bool surround_walls[9];*/
	for (i = tiledatas.begin(); i != tiledatas.end(); i++) {
		index = getindex;
//		current = get_coords_at_index(i - tiledatas.begin(), *&this);
		current.X = index % width;
		current.Y = index / width;

		tile = new Tile(tiledatas[index], current.X, current.Y);

		tiles.push_back(tile);

		//	calculate surrounding coordinates
/*		surround_coords = {
			{current[0]-1, current[1]-1}, {current[0]  , current[1]-1}, {current[0]+1, current[1]-1},
			{current[0]-1, current[1]  }, {current[0]  , current[1]  }, {current[0]+1, current[1]  },
			{current[0]-1, current[1]+1}, {current[0]  , current[1]+1}, {current[0]+1, current[1]+1}
		}
		// get wall bools
		for (int x = 0; x < 9 ; ++x) {
			surround_indexes[x] = get_index_for_tile(surround_coords[x][0], surround_coords[x][1]);
			surround_types[x] = tile_is_wall[tiledatas[surround_indexes[x]].type];
		}*/
	}
}

Tile::Tile(Tiledata _data, int _x, int _y) {
	data = _data;
	x = _x;
	y = _y;
	vector3df tilePos(y*TILE_SIZE, 0, x*TILE_SIZE);
	if (tile_is_wall[data.type]) {
		scn = smgr->addMeshSceneNode(wallMesh, 0, ID_SELECTABLE, tilePos);
	}
	else {
		scn = smgr->addMeshSceneNode(groundMesh, 0, ID_SELECTABLE, tilePos);
	}
	scn->setMaterialType(video::EMT_LIGHTMAP_ADD);
	scn->setMaterialFlag(video::EMF_LIGHTING, UserConfig.read<bool>("display.lighting", true));
	scn->setMaterialFlag(video::EMF_BILINEAR_FILTER, not UserConfig.read<bool>("display.minecraftmode", false)); // Minecraft!
	scn->setMaterialTexture(0, tileTextures[data.type]);
	NodeOwner *own = new NodeOwner;
	own->ownerType = NodeOwner::TYPE_TILE;
	own->tileOwner = this;
	scn->setUserData(own);
	tileSceneNodes.push_back(scn);
}


#if UNIT_TEST
#include <iostream>
int main () {
	Map map;
	FILE* f;
	f = fopen("test.map", "rb");
	map.load(f);
	std::vector<Tiledata>::iterator it = map.tiledatas.begin();
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
