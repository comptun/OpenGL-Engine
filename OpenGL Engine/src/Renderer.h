#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x; \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearErrors();

bool glLogCall(const char* function, const char* file, unsigned int line);

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};