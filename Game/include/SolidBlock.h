#ifndef SOLID_BLOCK_H
#define SOLID_BLOCK_H

#include "object.h"

class SolidBlockC : public ObjectC {
public:
    SolidBlockC(float initPosX, float initPosY);
    virtual void render() override;

    // Additional functions and members as needed
};

#endif // SOLID_BLOCK_H
