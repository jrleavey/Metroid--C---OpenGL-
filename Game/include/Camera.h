#ifndef CAMERA_H
#define CAMERA_H

#include "windows.h"
#include "Player.h"
#include "Room.h"

class Camera {
public:
    Camera(float width, float height) : x(0.0f), y(0.0f), width(width), height(height) {}
    // Constructor: Initializes the camera with a specific width and height.
    // x and y are set to 0.0f, indicating the initial position of the camera.

    void update(const Player& player, const Room& room);
    // update: Adjusts the camera's position based on the player's position and the room's boundaries.

    void applyViewTransform() const;
    // applyViewTransform: Applies a transformation to the OpenGL view matrix, shifting the view to the camera's position.
    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x, y;     // Camera position
    float width, height; // Camera dimensions
};

#endif // CAMERA_H
