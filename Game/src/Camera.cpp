#define NOMINMAX
#include "Camera.h"
#include <GL/gl.h>
#include <iostream>
void Camera::update(const Player& player, const Room& room) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Center on player, but don't go outside room boundaries
    x = fmaxf(0.0f, fminf(player.getX() - halfWidth, room.getWidth() * room.getTileSize() - width));
    y = fmaxf(0.0f, fminf(player.getY() - halfHeight, room.getHeight() * room.getTileSize() - height));
}




void Camera::applyViewTransform() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-x, -y, 0); // Move the world opposite to the camera's position
}
