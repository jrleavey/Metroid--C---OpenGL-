#include "Room.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <Windows.h>
#include <GL/gl.h>



Room::Room(int width, int height, float tileSize) : width(width), height(height), tileSize(tileSize) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                grid[y][x] = BASE;
            }
            else {
                grid[y][x] = OPEN_SPOT;
            }
        }
    }
}

void Room::render() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float left = x * tileSize;
            // Adjust the 'bottom' so that the drawing starts from the top left corner
            float bottom = (height - y - 1) * tileSize;

            switch (grid[y][x]) {
            case BASE:
                glColor3f(0.0f, 0.0f, 0.5f); // Navy blue for BASE
                drawQuad(left, bottom);
                break;
            case TRIANGLE_LEFT:
                glColor3f(0.0f, 1.0f, 1.0f); // Cyan for TRIANGLE_LEFT
                drawQuad(left, bottom);
                break;
            case TRIANGLE_RIGHT:
                glColor3f(0.0f, 0.5f, 0.5f); // Darker cyan for TRIANGLE_RIGHT
                drawQuad(left, bottom);
                break;
            case FACE:
                glColor3f(0.0f, 0.0f, 0.75f); // Medium blue for FACE
                drawQuad(left, bottom);
                break;
            case SPIRAL_BLOCK:
                glColor3f(0.0f, 0.4f, 0.4f); // Teal for SPIRAL_BLOCK
                drawQuad(left, bottom);
                break;
            case METAL_BLOCK:
                glColor3f(0.0f, 0.2f, 0.2f); // Dark teal for METAL_BLOCK
                drawQuad(left, bottom);
                break;
            case WEB:
                glColor3f(0.0f, 0.3f, 0.3f); // Dark cyan for WEB
                drawQuad(left, bottom);
                break;
            case EGG_SHAPE:
                glColor3f(0.0f, 0.6f, 0.6f); // Light teal for EGG_SHAPE
                drawQuad(left, bottom);
                break;
            case DOOR_TOP:
                glColor3f(0.0f, 0.0f, 0.3f); // Dark blue for DOOR_TOP
                drawQuad(left, bottom);
                break;
            case DOOR_MIDDLE:
                glColor3f(0.0f, 0.0f, 0.2f); // Slightly darker blue for DOOR_MIDDLE
                drawQuad(left, bottom);
                break;
            case DOOR_BOTTOM:
                glColor3f(0.0f, 0.0f, 0.3f); // Dark blue for DOOR_BOTTOM
                drawQuad(left, bottom);
                break;
            case DOOR_CUBES:
                glColor3f(0.0f, 0.0f, 0.1f); // Very dark blue for DOOR_CUBES
                drawQuad(left, bottom);
                break;
            case PIPE_HORIZ:
                glColor3f(0.5f, 0.5f, 0.5f); // Grey color for PIPE_HORIZ
                drawQuad(left, bottom);
                // Additionally, you can draw lines or other details to represent a pipe
                break;
            case PIPE_VERT:
                glColor3f(1.0f, 0.5f, 0.5f); // Grey color for PIPE_VERT
                drawQuad(left, bottom);
                // Additionally, you can draw lines or other details to represent a pipe
                break;
            case OPEN_SPOT:
            default:
                // No need to draw anything for OPEN_SPOT
                break;
            }
        }
    }
    drawGrid();  // Draw grid lines for testing
}


void Room::drawQuad(float left, float bottom) const {
    float right = left + tileSize;  // Right boundary
    float top = bottom + tileSize;  // Top boundary

    glBegin(GL_QUADS);
    glVertex2f(left, bottom);  // Bottom left corner
    glVertex2f(right, bottom);  // Bottom right corner
    glVertex2f(right, top);    // Top right corner
    glVertex2f(left, top);     // Top left corner
    glEnd();
}

void Room::drawGrid() const {
    glColor3f(0.1f, 0.1f, 0.1f);  // Dark color for grid lines
    glBegin(GL_LINES);
    for (int y = 0; y <= height; ++y) {
        float posY = y * tileSize;
        glVertex2f(0, posY);
        glVertex2f(width * tileSize, posY);
    }
    for (int x = 0; x <= width; ++x) {
        float posX = x * tileSize;
        glVertex2f(posX, 0);
        glVertex2f(posX, height * tileSize);
    }
    glEnd();
}

bool Room::isCellBlocked(int x, int y) const {
    // Flip the y coordinate to match the rendering system
    int invertedY = height - y - 1;

    if (x < 0 || x >= width || invertedY < 0 || invertedY >= height) {
        return true; // Out of bounds implies a blocked cell
    }

    // Only OPEN_SPOT is non-collidable; all other tiles are considered blocked.
    return grid[invertedY][x] != OPEN_SPOT;
}



void Room::setTile(int x, int y, CellType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = type;
    }
    else {
        // Optionally handle the error, such as logging it or throwing an exception
    }
}

void Room::loadRoomFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    // Define a mapping from characters to CellType
    std::unordered_map<char, CellType> tileMapping = {
        {'.', OPEN_SPOT},
        {'B', BASE},
        {'L', TRIANGLE_LEFT},
        {'R', TRIANGLE_RIGHT},
        {'F', FACE},
        {'S', SPIRAL_BLOCK},
        {'M', METAL_BLOCK},
        {'W', WEB},
        {'E', EGG_SHAPE},
        {'T', DOOR_TOP},
        {'Y', DOOR_MIDDLE},
        {'D', DOOR_BOTTOM},
        {'X', DOOR_CUBES},
        {'P', PIPE_HORIZ},
        {'Z', PIPE_VERT}
    };

    // Skip the legend part of the file
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines and comments
        }
        lines.push_back(line);
    }

    file.close();

    // Check if the file has more lines than the height of the room
    if (lines.size() > static_cast<size_t>(height)) {
        std::cerr << "File contains more lines than expected. Only the first " << height << " lines will be used." << std::endl;
        lines.resize(height); // Resize the vector to match the height of the room
    }

    // Process the lines in order, starting from the top (flip the Y-axis)
    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& currentLine = lines[i];
        for (int x = 0; x < static_cast<int>(currentLine.length()) && x < width; ++x) {
            char tileChar = currentLine[x];
            if (tileMapping.find(tileChar) != tileMapping.end()) {
                setTile(x, i, tileMapping[tileChar]);
            }
            else {
                std::cerr << "Unknown tile character: " << tileChar << " at (" << x << ", " << i << ")" << std::endl;
            }
        }
    }
}


