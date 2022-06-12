#pragma once

#include <GL/glew.h>
#include <memory>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "PerlinNoise.hpp"

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x; \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

class Block
{
public:
    VertexArray va;
    VertexBuffer vb;
    VertexBufferLayout layout;
    IndexBuffer ib;
    Shader shader;
    void setTexture(const int& tex);
    Block();
    ~Block();
};

void glClearErrors();

bool glLogCall(const char* function, const char* file, unsigned int line);

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const glm::mat4& mvp);
    void draw(Block& block, FastNoiseLite& noise, float x, float z, const glm::mat4& mvp);
    void draw(Block& block, const glm::mat4& mvp);
    void drawSkybox(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const glm::mat4& mvp);
    void drawSkybox(Block& block, const glm::mat4& mvp);
    void drawScene(const glm::vec3& position, FastNoiseLite& noise, const glm::mat4& mvp);

};
