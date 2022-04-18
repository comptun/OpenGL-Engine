#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glCall(glGenBuffers(1, &this->m_RendererID));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer()
{
    glCall(glDeleteBuffers(1, &this->m_RendererID));
}

void VertexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
}
void VertexBuffer::unbind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}