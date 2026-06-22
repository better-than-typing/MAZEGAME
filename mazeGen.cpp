//
// Created by EyesightsX on 2026-06-21.
//

#include <random>

#include "Headers/mazegen.h"

namespace Maze {
    std::vector<arrowIndex> initMaze() {
        std::vector<arrowIndex> arrowIndicesVector;

        int i = 0;
        for (int z = 0; z < numDotsOnSide; z++) {
            for (int x = 0; x < Maze::numDotsOnSide; x++) {
                //arrowIndex arrow_index{x, x+1, y, y};
                //arrowIndicesVector.push_back(arrow_index);

                if (x == numDotsOnSide - 1 && z == 0) {
                    arrowIndex noArrow{numDotsOnSide - 1, numDotsOnSide - 1, 0, 0};
                    arrowIndicesVector.push_back(noArrow);
                    //std::cout << i << ": " << noArrow.xI << ", " << noArrow.yI << " -> " << noArrow.xF << ", " << noArrow.yF << std::endl;
                    break;
                }

                if (x <= numDotsOnSide - 2) {
                    arrowIndex arrow_index{x, x+1, z, z};
                    arrowIndicesVector.push_back(arrow_index);
                    //std::cout << i << ": " << arrow_index.xI << ", " << arrow_index.yI << " -> " << arrow_index.xF << ", " << arrow_index.yF << std::endl;
                } else {
                    //std::cout << "Insert Down Arrow\n";
                    arrowIndex down_index{x, x, z, z - 1};
                    arrowIndicesVector.push_back(down_index);
                    //std::cout << i << ": " << down_index.xI << ", " << down_index.yI << " -> " << down_index.xF << ", " << down_index.yF << std::endl;
                }
            }

            //std::cout << "Next\n";
        }

        return arrowIndicesVector;
    }

    std::vector<arrowIndex> shiftedMazeIndices(origin nextOrigin, std::vector<arrowIndex> prevMazeIndices) {

        int index = currentOrigin.x + currentOrigin.z * numDotsOnSide;
        int removedIndex = nextOrigin.x + nextOrigin.z * numDotsOnSide;

        //std::cout << "Index: " << index << std::endl;

        /**
        for (int i = 0; i < prevMazeIndices.size(); i++) {
            std::cout << "vector[" << i << "]: ";

            std::string s1 = std::format("({0}, {1})", prevMazeIndices[i].xI,  prevMazeIndices[i].yI);
            std::string s2 = std::format("({0}, {1})", prevMazeIndices[i].xF,  prevMazeIndices[i].yF);

            std::cout << s1 << " -> " << s2 << std::endl;
        }
        **/

        //std::string origin1 = std::format("{0} , {1}", currOrigin.x, currOrigin)

        prevMazeIndices[index].xF = nextOrigin.x;
        prevMazeIndices[index].zF = nextOrigin.z;

        currentOrigin.x = nextOrigin.x;
        currentOrigin.z = nextOrigin.z;

        originIndex = currentOrigin.x + currentOrigin.z * numDotsOnSide;
        //std::cout << "Origin Index: " << originIndex << "\n";

        prevMazeIndices[originIndex].xF = prevMazeIndices[originIndex].xI;
        prevMazeIndices[originIndex].zF = prevMazeIndices[originIndex].zI;

        return prevMazeIndices;
    }

    std::vector<Wall> generateWalls(const std::vector<arrowIndex>& mazeIndicesVector, const std::vector<glm::vec3>& worldPosDots) {
        std::vector<Wall> wallsVector;


        int i = 0;
        for (arrowIndex arrowIndex : mazeIndicesVector) {
            if (arrowIndex.xF != arrowIndex.xI || !arrowIndex.beingPointedAtX) {

                glm::vec3 position = worldPosDots[i];
                position.x *= 0.1f;
                position.z = (position.z + dotPosHalfInterval) * 0.1f;

                glm::vec3 size = glm::vec3(dotPosHalfInterval * 2 * 0.1f, 3.0f, 1.0f);


                Wall wall{worldPosDots[i].x - dotPosHalfInterval,
                    worldPosDots[i].x + dotPosHalfInterval,
                    worldPosDots[i].z - dotPosHalfInterval,
                    worldPosDots[i].z - dotPosHalfInterval,
                    position, size, glm::vec3(0.0f, 0.0f, 0.0f)};

                wallsVector.push_back(wall);
            }

            i++;
        }

        // TODO FIX ROTATION OR BAD POSITION?
        i = 0;
        for (arrowIndex arrowIndex : mazeIndicesVector) {
            if (arrowIndex.zF != arrowIndex.zI || !arrowIndex.beingPointedAtZ) {

                glm::vec3 position = worldPosDots[i];
                position.z *= 0.1f;
                position.x = (position.x + dotPosHalfInterval) * 0.1f;

                glm::vec3 size = glm::vec3(dotPosHalfInterval * 2 * 0.1f, 3.0f, 1.0f);

                Wall wall{worldPosDots[i].x - dotPosHalfInterval,
                    worldPosDots[i].x + dotPosHalfInterval,
                    worldPosDots[i].z - dotPosHalfInterval,
                    worldPosDots[i].z - dotPosHalfInterval,
                    position, size, glm::vec3(0.0f, 1.0f, 0.0f)};

                wallsVector.push_back(wall);
            }

            i++;
        }


        return wallsVector;
    }

    origin getRandomOrigin() {
        origin randomOrigin{currentOrigin.x, currentOrigin.z};

        std::random_device rd;
        std::mt19937 gen(rd());

        bool isValidRandomOrigin = false;
        do {
            randomOrigin.x = currentOrigin.x;
            randomOrigin.z = currentOrigin.z;

            std::uniform_int_distribution<int> axisDistribution(0, 1);
            std::uniform_int_distribution<int> scalarDistribution(0, 1);

            bool xAxis = axisDistribution(gen);
            int directionScalar = scalarDistribution(gen) ? 1 : -1;

            if (xAxis) {
                randomOrigin.x = currentOrigin.x + directionScalar;
            } else {
                randomOrigin.z = currentOrigin.z + directionScalar;
            }

            if ((randomOrigin.x < numDotsOnSide && randomOrigin.x >= 0) && ((randomOrigin.z < numDotsOnSide && randomOrigin.z >= 0))) {
                isValidRandomOrigin = true;
            }

        } while (!isValidRandomOrigin);

        return randomOrigin;
    }

    //TODO Implement Random Walls
    void generateMaze(int iterations) {

        std::vector<arrowIndex> initArrowIndices = initMaze();
        //std::cout << "Origin: " << currentOrigin.x << ", " << currentOrigin.y << std::endl;

        std::vector<arrowIndex> currArrowIndices = initArrowIndices;

        //std::cout << "Current Origin: " << currentOrigin.x << ", " << currentOrigin.y << std::endl;


        //TODO Generate Later!!!
        /*
        for (int i = 0; i < iterations; i++) {
            origin nextOrigin = getRandomOrigin();

            std::vector<arrowIndex> nextArrowIndices = shiftedMazeIndices(nextOrigin, currArrowIndices);
            currArrowIndices = nextArrowIndices;

            //std::cout << "Current Origin: " << currentOrigin.x << ", " << currentOrigin.y << std::endl;
        }
        */
    }

    void markNodes(std::vector<arrowIndex>& mazeIndices) {
        int nextVectorIndex = 0;
        for (arrowIndex index : mazeIndices) {

            if (index.xF != index.xI) {
                nextVectorIndex = index.xF + index.zF * numDotsOnSide;
                mazeIndices[nextVectorIndex].beingPointedAtX = true;
            }

            if (index.zF != index.zI) {
                nextVectorIndex = index.xF + index.zF * numDotsOnSide;
                mazeIndices[nextVectorIndex].beingPointedAtZ = true;
            }
        }
    }
}

