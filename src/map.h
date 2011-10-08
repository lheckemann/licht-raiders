#pragma once
#include <vector>
#include <cstdio>
#include <stdint.h>

typedef struct {
	uint32_t type;
	uint32_t height;
	uint32_t point;
} Tile;

class Map {
public:
	void load(FILE*);
	std::vector<Tile> tiles;
	uint32_t width, height;
};
