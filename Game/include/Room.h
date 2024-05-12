#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>

enum CellType {
    BASE,
    EGG_SHAPE,
    VERT_PIPE,
    FACE,
    WEB,
    TRIANGLE_RIGHT,
    TRIANGLE_LEFT,
    HORIZ_PIPE, // Renamed from PIPE_HORIZ
    SPIRAL,
    METAL_BLOCK,
    DOOR_SQUARE,
    LAVA_TOP,
    LAVA,
    MARIO_TUBE_BLUE_LEFT,
    MARIO_TUBE_BLUE_RIGHT,
    BUBBLE,
    SMALL_PIPE,
    QUAD,
    WHITE_PIPE_VERT,
    WHITE_PIPE_HORIZ,
    WHITE_METAL,
    WHITE_DOUBLE_PIPE,
    OPEN_SPOT,
    GREEN_BLOCK
};
struct Door {
    int gridX, gridY; // Position of the door in the grid
    std::string destinationRoomFile; // Filename of the destination room
};

struct Position {
    float x;
    float y;
};

class Room {
public:
    Room(int width, int height, float tileSize);
    Position gridToWorld(int gridX, int gridY) const {
        return Position{ gridX * tileSize, gridY * tileSize };
    }
    void loadRoomFromFile(const std::string& filename);
    void render() const;
    bool isCellBlocked(int x, int y) const;
    void setTile(int x, int y, CellType type);
    float getTileSize() const { return tileSize; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::vector<Door>& getDoors() const { return doors; }


private:
    static const int MAX_WIDTH = 1000;
    static const int MAX_HEIGHT = 1000;
    int width, height;
    float tileSize;
    CellType grid[MAX_HEIGHT][MAX_WIDTH];
    void drawQuad(float left, float bottom, float texLeft, float texBottom, float texRight, float texTop) const;
    void drawGrid() const;
    std::vector<Door> doors; // Vector to store Door objects
    int BrinstarAtlas;

};

#endif // ROOM_H
