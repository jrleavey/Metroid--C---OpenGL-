#ifndef GRID_H
#define GRID_H

class Grid {
public:
    Grid(float tileSize);

    float getTileSize() const;
    // Add other utility methods as needed

private:
    float tileSize;
};
#endif // GRID_H
