#include "Room.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <Windows.h>
#include <GL/gl.h>
#include <sstream>
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include <assert.h>
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "spriteDemoManager.h"
#include "random.h"

#include "soil.h"


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
    // Using fopen to check if the file can be opened
    FILE* file = fopen("asset/Brinstar.png", "rb"); // "rb" - read binary mode
    if (file == nullptr) {
        perror("Error opening file");
    }
    else {
        std::cout << "'asset/Brinstar.png' opened successfully" << std::endl;
        fclose(file); // Close the file after successful opening
    }
    //BrinstarAtlas = SOIL_load_OGL_texture("asset/Brinstar.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    BrinstarAtlas = SOIL_load_OGL_texture("asset/Brinstar.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void Room::render() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, BrinstarAtlas);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float left = x * tileSize;
            float bottom = (height - y - 1) * tileSize;
            float texLeft, texRight;

            switch (grid[y][x]) {
            case OPEN_SPOT:
                continue; // No need to draw anything for OPEN_SPOT
            case BASE:
                texLeft = 0.0f / 304.0f;
                texRight = 16.0f / 304.0f;
                break;
            case EGG_SHAPE:
                texLeft = 16.0f / 304.0f;
                texRight = 32.0f / 304.0f;
                break;
            case VERT_PIPE:
                texLeft = 32.0f / 304.0f;
                texRight = 48.0f / 304.0f;
                break;
            case FACE:
                texLeft = 48.0f / 304.0f;
                texRight = 64.0f / 304.0f;
                break;
            case WEB:
                texLeft = 64.0f / 304.0f;
                texRight = 80.0f / 304.0f;
                break;
            case TRIANGLE_RIGHT:
                texLeft = 80.0f / 304.0f;
                texRight = 96.0f / 304.0f;
                break;
            case TRIANGLE_LEFT:
                texLeft = 96.0f / 304.0f;
                texRight = 112.0f / 304.0f;
                break;
            case HORIZ_PIPE:
                texLeft = 112.0f / 304.0f;
                texRight = 128.0f / 304.0f;
                break;
            case SPIRAL:
                texLeft = 128.0f / 304.0f;
                texRight = 144.0f / 304.0f;
                break;
            case METAL_BLOCK:
                texLeft = 144.0f / 304.0f;
                texRight = 160.0f / 304.0f;
                break;
            case LAVA_TOP:
                texLeft = 160.0f / 304.0f;
                texRight = 176.0f / 304.0f;
                break;
            case LAVA:
                texLeft = 176.0f / 304.0f;
                texRight = 192.0f / 304.0f;
                break;
            case MARIO_TUBE_BLUE_LEFT:
                texLeft = 192.0f / 304.0f;
                texRight = 208.0f / 304.0f;
                break;
            case MARIO_TUBE_BLUE_RIGHT:
                texLeft = 208.0f / 304.0f;
                texRight = 224.0f / 304.0f;
                break;
            case BUBBLE:
                texLeft = 224.0f / 304.0f;
                texRight = 240.0f / 304.0f;
                break;
            case WHITE_PIPE_VERT:
                texLeft = 240.0f / 304.0f;
                texRight = 256.0f / 304.0f;
                break;
            case WHITE_PIPE_HORIZ:
                texLeft = 256.0f / 304.0f;
                texRight = 272.0f / 304.0f;
                break;
            case WHITE_DOUBLE_PIPE:
                // Coordinates originally meant for WHITE_METAL
                texLeft = 272.0f / 304.0f;
                texRight = 288.0f / 304.0f;
                break;
            case GREEN_BLOCK:
                // Last 16 pixels for GREEN_BLOCK
                texLeft = 288.0f / 304.0f;
                texRight = 304.0f / 304.0f;
                break;
            default:
                continue; // Skip drawing if the tile type is not recognized
            }

            drawQuad(left, bottom, texLeft, 0.0f, texRight, 1.0f);
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    //drawGrid();
}




void Room::drawQuad(float left, float bottom, float texLeft, float texBottom, float texRight, float texTop) const {
    float right = left + tileSize;
    float top = bottom + tileSize;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, BrinstarAtlas);

    glBegin(GL_QUADS);
    glColor4ub(0xff, 0xff, 0xff, 0xff);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(left, bottom);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(right, bottom);
    glTexCoord2f(texRight, texTop);
    glVertex2f(right, top);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(left, top);
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
    // Clear the existing grid before loading the new room
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = OPEN_SPOT;
        }
    }
    
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    // Define a mapping from characters to CellType
    std::unordered_map<char, CellType> tileMapping = {
        {'.', OPEN_SPOT},
        {'B', BASE},
        {'E', EGG_SHAPE},
        {'Z', VERT_PIPE},
        {'F', FACE},
        {'W', WEB},
        {'R', TRIANGLE_RIGHT},
        {'L', TRIANGLE_LEFT},
        {'P', HORIZ_PIPE}, // Using 'P' from the original PIPE_HORIZ
        {'S', SPIRAL},
        {'M', METAL_BLOCK},
        {'T', LAVA_TOP},
        {'A', LAVA},
        {'X', MARIO_TUBE_BLUE_LEFT},
        {'Y', MARIO_TUBE_BLUE_RIGHT},
        {'U', BUBBLE},
        {'I', WHITE_PIPE_VERT},
        {'Z', WHITE_PIPE_HORIZ},
        {'N', WHITE_METAL},
        {'C', WHITE_DOUBLE_PIPE},
        {'V', GREEN_BLOCK}
    };

    // Read the file and process each line
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines and comments
        }

        if (line[0] == 'D') { // Check if the line represents a door
            std::istringstream iss(line.substr(2)); // Skip 'D '
            Door door;
            if (!(iss >> door.gridX >> door.gridY >> door.destinationRoomFile)) {
                std::cerr << "Error parsing door data: " << line << std::endl;
                continue;
            }
            doors.push_back(door); // Add the door to the vector
            continue;
        }

        lines.push_back(line); // Add the line to the lines vector
    }

    file.close();

    // Check if the file has more lines than the height of the room
    if (lines.size() > static_cast<size_t>(height)) {
        std::cerr << "File contains more lines than expected. Only the first "
            << height << " lines will be used." << std::endl;
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
                std::cerr << "Unknown tile character: " << tileChar
                    << " at (" << x << ", " << i << ")" << std::endl;
            }
        }
    }
}



