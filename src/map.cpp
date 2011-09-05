#include "map.h"

Tile::Tile(uint8_t _type, uint8_t _height) {
	type = _type;
	height = _height;
}

void Map::load(FILE* map) {
	int width, height;
	uint32_t type, height, point;
	uint8_t tilesize, field;
	fread(&width, 4, 1, map);
	fread(&height, 4, 1, map);
	for (int i = 0; i < width*height; i++;) { // For every tile
		fread(&tilesize, 1, 1, map); // Read number of fields in tile
		for (int j = 0; j < tilesize; j++) { // For every field in the current tile
			fread(&field, 1, 1, map); // Read field type
			switch(field) { // Decide on field type and read into appropriate variable
				case 0: fread(&type, 4, 1, map); break; // Field type 0 = tile type
				case 1: fread(&height, 4, 1, map); break; // Field type 1 = tile height
				case 2: fread(&point, 4, 1, map); break; // Field type 2 = tile number for scripting
//				case 3: fread(&asdf, 1, 1, map); break; // Field type 3 = ????
				default: fseek(map, 4, SEEK_CUR); // Skip the value of the field if its type is unknown
			}
		}
	}
}

