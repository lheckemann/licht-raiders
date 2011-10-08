#include "map.h"

#include <cassert>

void Map::load(FILE* map) {
	Tile t;
	uint32_t type, tileheight, point;
	uint8_t tilesize, field;
	uint8_t check[4] = "  ";
	uint16_t version;
	fread(&check, 1, 2, map);
	fread(&version, 2, 1, map);
//	assert(check == "RR"); // TODO check magic and version!
//	assert(version == 1);
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
