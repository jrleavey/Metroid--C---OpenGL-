#include "Pickup.h"

// Constructor
Pickup::Pickup(Type type, float x, float y, float size, float duration)
    : type(type), x(x), y(y), size(size), lifespan(duration), timer(0.0f), markedForRemoval(false) {}

// Update method for the pickup
void Pickup::update(float deltaTime) {
    timer += deltaTime;
    if (timer >= lifespan) {
        markForRemoval(); // Mark for removal if lifespan is exceeded
    }
}

// Render method for the pickup
void Pickup::render() const {
    // Rendering logic (adjust according to your graphics library)
    glColor3f(type == Energy ? 1.0f : 0.0f, type == Missile ? 1.0f : 0.0f, 0.0f); // Energy: Red, Missile: Green
    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glVertex2f(x + size / 2, y + size / 2);
    glVertex2f(x - size / 2, y + size / 2);
    glEnd();
}

// Check if the pickup is expired
bool Pickup::isExpired() const {
    return timer >= lifespan;
}

// Get the type of the pickup
Pickup::Type Pickup::getType() const {
    return type;
}

// Mark the pickup for removal
void Pickup::markForRemoval() {
    markedForRemoval = true;
}

// Check if the pickup is marked for removal
bool Pickup::isMarkedForRemoval() const {
    return markedForRemoval;
}
float Pickup::getX() const {
    return x;
}

float Pickup::getY() const {
    return y;
}

float Pickup::getSize() const {
    return size;
}