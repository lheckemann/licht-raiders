#pragma once
#include "globs.h"
// Do we really need a class?
void reconstruct_path(std::map<Tile*,Tile*> came_from, Tile* current_node, std::vector<Tile*>& path);
std::vector<mapCoords*> calculate_path(std::vector<Tile*> tiles);

