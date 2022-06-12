#include "Renderer.h"

#include <iostream>


#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x; \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

float positions[] = {
   -0.5, -0.5, -0.5,
   -0.5, -0.5,  0.5,
    0.5, -0.5,  0.5,
    0.5, -0.5, -0.5,
   -0.5,  0.5, -0.5,
   -0.5,  0.5,  0.5,
    0.5,  0.5,  0.5,
    0.5,  0.5, -0.5,
};

#define LEFT 0, 1, 5, 0, 4, 5
#define FRONT 1, 2, 6, 1, 5, 6
#define RIGHT 2, 3, 7, 2, 6, 7
#define BACK 3, 7, 4, 3, 0, 4
#define TOP 4, 5, 6, 4, 7, 6
#define BOTTOM 0, 1, 2, 0, 3, 2

unsigned int indices[] = {
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

void Block::setTexture(const int& tex)
{
    shader.setUniform1i("u_Texture", tex);
}


Block::Block()
    : vb(positions, sizeof(positions)), ib(indices, sizeof(indices) / sizeof(unsigned int)), shader("res/shaders/basic.shader")
{
    layout.pushFloat(3);
    va.addBuffer(vb, layout);
    shader.bind();
    shader.setUniform1i("u_Texture", 0);
}
Block::~Block()
{

}

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

void Renderer::drawScene(const glm::vec3& position, FastNoiseLite& noise, const glm::mat4& mvp)
{

}

void Renderer::clear() const
{
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const glm::mat4& mvp)
{
    shader.bind();
    va.bind();
    ib.bind();
    shader.setUniformMat4f("u_MVP", mvp);
    glCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(Block& block, const glm::mat4& mvp)
{
    block.shader.bind();
    block.va.bind();
    block.ib.bind();
    block.shader.setUniformMat4f("u_MVP", mvp);
    glCall(glDrawElements(GL_TRIANGLES, block.ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(Block& block, FastNoiseLite& noise, float x, float z, const glm::mat4& mvp)
{
    auto noi = [](FastNoiseLite& n, float a, float b) {
        return (int)(n.GetNoise(a, b) * 10);
    };
    block.shader.bind();
    block.va.bind();
    block.ib.bind();

    int n = noi(noise, x, z);

    std::vector<unsigned int> ind = {
            TOP,
            BOTTOM,
            LEFT,
            RIGHT,
            FRONT,
            BACK,
    };

    if (noi(noise, x - 1, z) == n) {
        ind = {
            TOP,
            BOTTOM,
            RIGHT,
            FRONT,
            BACK,
        };
    }
    IndexBuffer i(&ind[0], ind.size());
    i.bind();
    block.shader.setUniformMat4f("u_MVP", mvp);
    glCall(glDrawElements(GL_TRIANGLES, i.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawSkybox(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const glm::mat4& mvp)
{
    shader.bind();
    va.bind();
    ib.bind();

    glCall(glDisable(GL_DEPTH_TEST));

    shader.setUniformMat4f("u_MVP", mvp);
    glCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

    glCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::drawSkybox(Block& block, const glm::mat4& mvp)
{
    block.shader.bind();
    block.va.bind();
    block.ib.bind();

    glCall(glDisable(GL_DEPTH_TEST));

    block.shader.setUniformMat4f("u_MVP", mvp);
    glCall(glDrawElements(GL_TRIANGLES, block.ib.getCount(), GL_UNSIGNED_INT, nullptr));

    glCall(glEnable(GL_DEPTH_TEST));
}
