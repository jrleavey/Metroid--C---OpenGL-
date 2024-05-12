// Grid.cpp
#include "Grid.h"

Grid::Grid(float tileSize) : tileSize(tileSize) {}

float Grid::getTileSize() const {
    return tileSize;
}

// Add definitions of other utility methods