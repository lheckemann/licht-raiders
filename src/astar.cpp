#include "astar.h"
#include <deque>
#include <algorithm>

// we don't need this in the headerfile
bool comp(Tile* t1, Tile* t2) {
    return t1->data.height < t1->data.height;
}

void reconstruct_path(std::map<Tile*,Tile*> came_from, Tile* current_node, std::deque<Tile*>& path) {
    if (came_from.count(current_node)) {
        reconstruct_path(came_from, current_node, path);
    }
    path.push_back(current_node);
    return;
}

std::deque<Tile*> calculate_path(Tile* start, Tile* goal) {
    #define getindex(x,y) tiles[y*map->width+x]
    // wikipedia ftw
    std::deque<Tile*> openset;
    std::deque<Tile*> path;
    std::vector<Tile*> closedset;
    std::map<Tile*,Tile*> came_from;
    std::map<Tile*,unsigned int> f_cost,h_cost,g_cost;

    #define h(x) x->data.height
    // init
    openset.push_back(start);
    assert(!tile_is_wall[start->data.type]);
    g_cost[start] = 0;
    h_cost[start] = h(start);
    f_cost[start] = g_cost[start] + h_cost[start];

    while (!openset.empty()) {
        if (openset[0]==goal) {

            reconstruct_path(came_from, came_from[goal], path);
            // TODO cleanup
            return path;
        }
        closedset.push_back(openset.front());
        openset.pop_front();
        // neighbours
        // eight or four?
        // TODO

        sort(openset.begin(),openset.end(),comp); // TODO horribly inefficient
    }
    return path; // lol nullpointer

}
