class Camera {
public:
    Camera(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}

    void update(float playerX, float playerY, const Room& room) {
        // Center the camera on the player, but don't let it go outside the room
        x = std::max(0.0f, std::min(playerX - width / 2, room.getWidth() - width));
        y = std::max(0.0f, std::min(playerY - height / 2, room.getHeight() - height));
    }

    void applyViewTransform() {
        // Apply a transformation to the OpenGL view matrix
        glTranslatef(-x, -y, 0);
    }

private:
    float x, y;     // Camera position
    float width, height; // Camera size (usually the size of the window)
};
