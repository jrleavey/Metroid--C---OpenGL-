#include "level_data.h"

// Initialize the static level data as a 10x10 grid
const char LevelData::level_data[LEVEL_HEIGHT][LEVEL_WIDTH + 1] = {
    "XXXXXXXXXX",
    "X--------X",
    "X--------X",
    "X--------X",
    "X--------X",
    "X--------X",
    "X--------X",
    "X--------X",
    "X--------X",
    "XXXXXXXXXX",
    // Null terminators are implicitly added to each string literal
};
