#include "Projectile.h"
#include "Room.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
const float Projectile::DEFAULT_PROJECTILE_FIRE_RATE = 0.2f; // Example value, adjust as needed
const float Projectile::DEFAULT_SIZE = 10.0f;               // Example size
const float Projectile::DEFAULT_SPEED = 1000.0f;             // Example speed
const float Projectile::DEFAULT_RANGE = 700.0f;            // Example range
const int Projectile::DEFAULT_DAMAGE = 5;                   // Example damage
const float Projectile::MISSILE_FIRE_RATE = 1.0f; // Adjust as needed
const int Projectile::MISSILE_DAMAGE = 10; // Higher damage than default projectile
const float Projectile::MISSILE_SPEED = 1000.0f; // Adjust as needed
const float Projectile::MISSILE_RANGE = 10000.0f; // Unlimited range
const float Projectile::MISSILE_SIZE = 15.0f; // Adjust as needed

Projectile::Projectile(float x, float y, float size, float speed, float range, int damage, int direction, bool isMissile)
    : x(x), y(y), size(size), speed(speed), range(range), damage(damage),
    direction(direction), distanceTravelled(0.0f), isMissile(isMissile) {
    // Constructor implementation
}
void Projectile::update(float deltaTime) {
    if (direction == 0) {  // Upward shooting
        y += speed * deltaTime;
    }
    else {  // Left or right shooting
        x += speed * direction * deltaTime;
    }
    distanceTravelled += speed * deltaTime;
}

void Projectile::render() const {
    const int numSegments = 20;  // Increase for a smoother circle
    const float radius = size / 2;  // Assuming 'size' is the diameter

    // Set color based on projectile type
    if (isMissile) {
        glColor3f(1.0f, 0.0f, 0.0f);  // Red color for missiles
    }
    else {
        glColor3f(1.0f, 1.0f, 0.0f);  // Yellow color for regular projectiles
    }

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Center of circle
    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float cx = radius * cosf(theta);
        float cy = radius * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}


bool Projectile::isExpired() const {
    return distanceTravelled >= range;
}
bool Projectile::isMarkedForRemoval() const {
    return markedForRemoval;
}
void Projectile::markForRemoval() {
    markedForRemoval = true;
}


float Projectile::getX() const {
    return x;
}

float Projectile::getY() const {
    return y;
}

int Projectile::getDamage() const {
    return damage;
}
float Projectile::getSize() const {
    return size; // Replace 'size' with the actual member variable that stores the size of the projectile
}

