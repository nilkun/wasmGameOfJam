#include "./Pathfinding.hpp"
#include <iostream>

// temp fix
// #include <cstdlib> 
#include <cmath>



int Pathfinding::manhattan(nilkun::Vector pos1, nilkun::Vector pos2) {
    return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
};

void Pathfinding::sort(
    std::vector<nilkun::Coordinates> &queue, 
    std::vector<std::vector<Data> > &data
) {
    std::sort(queue.begin(), queue.end(), 
        // LAMBDA
        [data](nilkun::Coordinates a, nilkun::Coordinates b) { 
            return (data[a.x][a.y].distanceGoal + data[a.x][a.y].distanceStart) > (data[b.x][b.y].distanceGoal + data[b.x][b.y].distanceStart);
        }
    );
};  

// needs start and goal (static function?)
std::vector<nilkun::Vector> Pathfinding::astar(std::vector<std::vector<int> > map, nilkun::Vector start, nilkun::Vector goalVector) {

    std::vector<std::vector<Data> > data;
    std::vector<nilkun::Coordinates> queue;
    nilkun::Coordinates goal { static_cast<int>(goalVector.x), static_cast<int>(goalVector.y) };
    bool found = false;

    // std::cout << "astar" << std::endl;
    // Data tdata[]

    nilkun::Coordinates current;
    // queue.push_back(Data { true, manhattan(start, goal), 0 });
    queue.push_back(nilkun::Coordinates { static_cast<int>(start.x), static_cast<int>(start.y) });
    const int DISTANCEBETWEEN = 1;

    while(queue.size() > 0 && !found) {
        // std::cout << "queue sorting" << std::endl;
        sort(queue, data);
        // std::cout << "fixing queue" << std::endl;

        current = queue.back();
                // std::cout << "popping queue" << std::endl;
                // std::cout << "QUEUE: " << queue.size() << std::endl;

        queue.pop_back();
        // std::cout << "visited" << map.size() << std::endl;

        data.resize(map.size(), std::vector<Data>(map[0].size()));

        // std::cout << current.x << "x" << current.y << std::endl;


        data[current.x][current.y].visited = true;
        // std::cout << "entering loop" << std::endl;

        if(current.x == goal.x && current.y == goal.y) found = true;
        else {
            for(int x = -1; x <= 1; x++) {
                for(int y = -1; y <= 1; y++) {
                    // std::cout << "loop" << std::endl;
                    // Check if within bounds
                    if(current.x + x >= 0 && current.x + x < map.size()) {
                        if(current.y + y >= 0 && current.y + y < map[0].size()) {
                            // Check if visited
                            if(map[current.x + x][current.y + y] <= 0 && !data[current.x + x][current.y + y].visited) {
                                // Compare distance to stored distance
                                const int compare = data[current.x][current.y].distanceStart + DISTANCEBETWEEN;
                                if(compare < data[current.x + x][current.y + y].distanceStart) {
                                    // Add current to list if not already on
                                    if(std::numeric_limits<int>::max() == data[current.x + x][current.y + y].distanceGoal) {
                                        queue.push_back(nilkun::Coordinates { current.x + x, current.y + y });
                                        // Find distance to goal
                                        data[current.x + x][current.y + y].distanceGoal = manhattan(
                                            nilkun::Vector{static_cast<float>(current.x + x), static_cast<float>(current.y + y) }, 
                                            nilkun::Vector{static_cast<float>(goal.x), static_cast<float>(goal.y) }
                                        );
                                    }
                                    // Set distance to start
                                    data[current.x + x][current.y + y].distanceStart = compare;
                                    // Set current as parent
                                    data[current.x + x][current.y + y].parent = current;
                                }                                    
                            }
                        }
                    }
                }
            }
        }

    };
    // std::cout << "end of search" << std::endl;
    std::vector<nilkun::Vector> returnData;
    if(found) {
        returnData.push_back(nilkun::Vector{ static_cast<float>(goal.x), static_cast<float>(goal.y) });
        int x = goal.x;
        int y = goal.y;
        while(data[x][y].parent.x != -1) {
            int tempX = data[x][y].parent.x;
            y = data[x][y].parent.y;
            x = tempX;
            returnData.push_back(nilkun::Vector{ static_cast<float>(x), static_cast<float>(y) });
        };
    }
    else std::cout << "NO PATH FOUND!!!" << std::endl;   
    // if(returnData.size() > 100) std::cout << "OBJECT " << returnData.size() << std::endl;      
    return returnData;       
};