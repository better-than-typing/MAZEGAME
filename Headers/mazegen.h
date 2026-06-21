//
// Created by EyesightsX on 2026-06-20.
//

#ifndef MAZEGAME_MAZEGEN_H
#define MAZEGAME_MAZEGEN_H

struct origin {
    int x, y;
};

struct arrowIndex {
    int xI, xF, yI, yF;

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

#endif //MAZEGAME_MAZEGEN_H