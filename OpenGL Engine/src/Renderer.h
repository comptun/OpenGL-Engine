#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x; \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearErrors();

bool glLogCall(const char* function, const char* file, unsigned int line);

#endif