#include "SolidBlock.h"
#include "Object.h"  // Include the header file where ObjectC and Coord2D are defined
#include "ShapeDraw.h"

SolidBlockC::SolidBlockC(float initPosX, float initPosY) : ObjectC(initPosX, initPosY, 0.0f, 0.0f) {
    // Initialize block specifics
}

void SolidBlockC::render() {
    // Use getPosition() to access the position
    Coord2D* pos = getPosition();
    float left = pos->x;
    float right = pos->x + 16.0f;
    float top = pos->y;
    float bottom = pos->y + 16.0f;

    unsigned char r = 255; // Example color - adjust as needed
    unsigned char g = 255;
    unsigned char b = 255;

    DrawLine(left, top, right, top, r, g, b);
    DrawLine(right, top, right, bottom, r, g, b);
    DrawLine(right, bottom, left, bottom, r, g, b);
    DrawLine(left, bottom, left, top, r, g, b);
}
