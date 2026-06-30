//
// Created by EyesightsX on 2026-06-20.
//

#ifndef MAZEGAME_MAZEGEN_H
#define MAZEGAME_MAZEGEN_H

#include <vector>
#include <glm/vec3.hpp>

#include "objects.h"

struct origin {
    int x, z;
};

struct arrowIndex {
    int xI, xF, zI, zF;

    bool beingPointedFromUp = false;
    bool beingPointedFromDown = false;
    bool beingPointedFromLeft = false;
    bool beingPointedFromRight = false;
    /**
     *  ^
     *  |
     *  |
     *  |  * * *
     *  |  * * *
     *  |  * * *
     *  +z * <- ex(0, 0)
     *  +x - - - - - >
     *  INDICES X GO FIRST
     */
};

namespace Maze {
    // Recommended Odd Number
    inline int numDotsOnSide = 31;

    inline origin currentOrigin{numDotsOnSide - 1, numDotsOnSide - 1};
    inline int originIndex = numDotsOnSide - 1;

    std::vector<arrowIndex> initMaze();
    std::vector<arrowIndex> shiftedMazeIndices(origin nextOrigin, std::vector<arrowIndex> prevMazeIndices);
    std::vector<Wall> generateWalls(const std::vector<arrowIndex>& mazeIndicesVector, const std::vector<glm::vec3>& worldPosDots);
    origin getRandomOrigin();

    void generateMaze();
    void markNodes(std::vector<arrowIndex>& mazeIndices);
}

#endif //MAZEGAME_MAZEGEN_H