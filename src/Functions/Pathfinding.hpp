#pragma once

#include <vector>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include "../Functions/Nilkun.hpp"

// the map is just 0 and 1s
namespace Pathfinding {
    struct Data {
        bool visited = false;
        int distanceGoal = std::numeric_limits<int>::max();
        int distanceStart = std::numeric_limits<int>::max();
        nilkun::Coordinates parent = { -1, -1 };
    };        
    int manhattan(nilkun::Vector pos1, nilkun::Vector pos2);
    void sort(std::vector<nilkun::Coordinates> &queue, std::vector<std::vector<Data> > &data);
    std::vector<nilkun::Vector> astar(std::vector<std::vector<int> > map, nilkun::Vector start, nilkun::Vector goalVector);
};