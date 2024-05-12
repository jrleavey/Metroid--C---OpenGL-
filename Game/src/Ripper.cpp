#include "Ripper.h"
#include <iostream>
#include "Room.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include "openglframework.h"  
#include "gamedefs.h"
#include "openGLStuff.h"
#include "SOIL.h"

Ripper::Ripper(float x, float y, float size, float speed, Room& room)
    : Enemy(), x(x), y(y), size(size), speed(speed), room(room), direction(1)
{
    health = 99; // Initialize health for Ripper
    RipperTexture = SOIL_load_OGL_texture("asset/Ripper Sprite.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

}


void Ripper::update(float deltaTime) {
    x += speed * direction * deltaTime;  // Move in the current direction

    if (isNextCellBlocked()) {
        turnAround();
    }

}

void Ripper::render() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, RipperTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Texture coordinates for the Ripper sprite
    float texLeft = direction == 1 ? 0.0f : 1.0f;   // Left edge of the sprite
    float texRight = direction == 1 ? 1.0f : 0.0f;  // Right edge of the sprite

    // Flipped texture coordinates for vertical flip, similar to the Geemer
    float texTop = 1.0f;    // Bottom edge of the texture (flipped)
    float texBottom = 0.0f; // Top edge of the texture (flipped)

    float left = x - size / 2;
    float right = x + size / 2;
    float bottom = y - size / 2;
    float top = y + size / 2;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);    // Top left corner of the quad
    glVertex2f(left, bottom);
    glTexCoord2f(texRight, texTop);   // Top right corner of the quad
    glVertex2f(right, bottom);
    glTexCoord2f(texRight, texBottom); // Bottom right corner of the quad
    glVertex2f(right, top);
    glTexCoord2f(texLeft, texBottom);  // Bottom left corner of the quad
    glVertex2f(left, top);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}





float Ripper::getX() const {
    return x;
}

float Ripper::getY() const {
    return y;
}

bool Ripper::isNextCellBlocked() const {
    // Predict the next cell based on current direction and position
    int nextX = static_cast<int>((x + (direction * size / 2)) / room.getTileSize());
    int currentY = static_cast<int>(y / room.getTileSize());

    // Check if the next cell in the direction of movement is blocked
    return room.isCellBlocked(nextX, currentY);
}

void Ripper::turnAround() {
    // Invert the direction
    direction *= -1;
}

void Ripper::decreaseHealth(int damage)
{
    //Rippers do not take damage
    gSoundManager.PlaySoundW(RIPPER_IMMUNE);

}

bool Ripper::isDead() const {
    return health <= 0;
}