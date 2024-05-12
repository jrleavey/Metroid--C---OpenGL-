#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

class LevelData {
public:
    static const int TILE_SIZE = 16;    // Each tile is 16x16 pixels
    static const int LEVEL_WIDTH = 10;  // The level is 10 tiles wide
    static const int LEVEL_HEIGHT = 10; // The level is 10 tiles high

    // Define the level layout with 'X' for solid blocks and '-' for empty spaces
    static const char level_data[LEVEL_HEIGHT][LEVEL_WIDTH + 1]; // +1 for null terminator
};

#endif // LEVEL_DATA_H
