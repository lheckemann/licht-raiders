#pragma once
#include "globs.h"
#include <irrlicht.h>
// Do we really need a class?
void reconstruct_path(std::map<Tile*,Tile*> came_from, Tile* current_node, std::vector<Tile*>& path);
std::deque<Tile*> calculate_path(Tile*, Tile*);

