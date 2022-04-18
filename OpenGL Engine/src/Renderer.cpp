#include "Renderer.h"

#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x; \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, unsigned int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (0x0" << std::hex << error << std::dec << "): "
            << function << " " << file << ":" << line << "\n";
        return false;
    }
    return true;
}