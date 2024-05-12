#ifndef RIPPER_H
#define RIPPER_H

#include "Room.h"
#include "Enemy.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include "openglframework.h"  
#include "gamedefs.h"
#include "openGLStuff.h"
#include <iostream>


class Ripper : public Enemy {
public:
    Ripper(float x, float y, float size, float speed, Room& room);

    void update(float deltaTime);
    void render() const;
    float getX() const;
    float getY() const;
    float getSize() const { return size; }
    void decreaseHealth(int damage) override;
    bool isDead() const override;


private:
    float x, y;
    float size;
    float speed;
    Room& room;
    int direction; // 1 for right, -1 for left

    bool isNextCellBlocked() const;
    void turnAround();

    GLuint RipperTexture; // Texture ID for the Ripper sprite

};

#endif // RIPPER_H