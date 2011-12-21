#include "map.h"
#include "globs.h"

std::vector<video::ITexture*> tileTextures;
std::vector<video::ITexture*> tileTextures_sel;

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
	char* selname = new char[10];
	video::ITexture* active;
	video::ITexture* active_selected;
	unsigned char* texData;
	unsigned char* texData_sel;
	unsigned int pix;
	for (unsigned i = 0; i < sizeof(texture_names)/sizeof(std::string); i++) {
		path = ("data/textures/tile/" + texture_names[i] + ".png").c_str();
		active = driver->getTexture(path);
		if (active == NULL) {
			bork((path + " could not be loaded").c_str());
		}
		sprintf(selname, "SEL%i", i);
		active_selected = driver->addTexture(active->getOriginalSize(), selname, active->getColorFormat());
		switch(active->getColorFormat()) { // TODO: Implement other colour formats / fix this bloody mess
			case video::ECF_A8R8G8B8:
				texData = (unsigned char*) active->lock();
				texData_sel = (unsigned char*) active_selected->lock();
				for (pix = 0; pix < active->getOriginalSize().Width * active->getOriginalSize().Height; pix++) {
					texData_sel[pix*4+0] = ((texData[pix*4+0] + 32) <= 32) ? 255 : texData[pix*4+0] + 32; // B
					texData_sel[pix*4+1] = texData[pix*4+1]; // G
					texData_sel[pix*4+2] = texData[pix*4+2]; // R
					texData_sel[pix*4+3] = texData[pix*4+3]; // A stays what it is
				}
				active->unlock();
				active_selected->unlock();
				break;
			default:
				bork("Unsupported pixel format in last mentioned texture");
		}
		tileTextures.push_back(active);
		tileTextures_sel.push_back(active_selected);
	}

	wallMesh = smgr->getMesh("data/models/wall.dae");
	groundMesh = smgr->getMesh("data/models/ground.dae");
}


#define getindex ((int)(i - tiledatas.begin()))
void Map::calculate_render() {
	load_textures();
	std::vector<Tiledata>::iterator i;
	mapCoords current;
	current.x = 0;
	current.y = 0;
	int index;
	Tile* tile;
/*	mapCoords surround_coords[9];
	int surround_indexes[9];
	bool surround_walls[9];*/
	for (i = tiledatas.begin(); i != tiledatas.end(); i++) {
		index = getindex;
//		current = get_coords_at_index(i - tiledatas.begin(), *&this); // TODO fix get_coords_at_index macro
		current.x = index % width;
		current.y = index / width;

		tile = new Tile(tiledatas[index], current.x, current.y);

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

Tile::Tile(Tiledata _data, int x, int y) {
	data = _data;
	if (tile_is_wall[data.type]) {
		scn = smgr->addMeshSceneNode(wallMesh, 0, ID_SELECTABLE, vector3df(y*-2, 0, x*-2));
	}
	else {
		scn = smgr->addMeshSceneNode(groundMesh, 0, ID_SELECTABLE, vector3df(y*-2, 0, x*-2));
	}
	scn->setMaterialType(video::EMT_SOLID);
	scn->setMaterialFlag(video::EMF_LIGHTING, UserConfig.read<bool>("display.lighting", true));
	scn->setMaterialFlag(video::EMF_BILINEAR_FILTER, not UserConfig.read<bool>("display.minecraftmode", false)); // Minecraft!
	scn->setMaterialTexture(0, tileTextures[data.type]);
	scn->setUserData(this);
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
