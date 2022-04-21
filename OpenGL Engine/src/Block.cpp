#include "Block.h"

static float positions[] = {
    -0.5, -0.5, -0.5,
    -0.5, -0.5,  0.5,
     0.5, -0.5,  0.5,
     0.5, -0.5, -0.5,
    -0.5,  0.5, -0.5,
    -0.5,  0.5,  0.5,
     0.5,  0.5,  0.5,
     0.5,  0.5, -0.5,
};

static unsigned int indices[] = {
    0, 1, 5,
    0, 4, 5,
    1, 2, 6,
    1, 5, 6,
    2, 3, 7,
    2, 6, 7,
    3, 7, 4,
    3, 0, 4,
    4, 5, 6,
    4, 7, 6,
    0, 1, 2,
    0, 3, 2,
};

Block::Block(const Texture& texture)
    : vb(positions, sizeof(positions)), ib(indices, sizeof(indices) / sizeof(unsigned int)), shader("res/shaders/basic.shader"), texture(texture)
{
    layout.push<float>(3);
    va.addBuffer(vb, layout);
}

Block::~Block() {}