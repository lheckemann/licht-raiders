#pragma once
#include <vector>
#include <cstdio>

typedef class {
public:
	uint32_t type;
	uint32_t height;
	uint32_t point;
	Tile();
} Tile;

class Map {
public:
	void load(FILE*);
private:
	vector<Tile> tiles;
};
