#include "VertexBufferLayout.h"


unsigned int VertexBufferElement::getSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT: return 4;
	case GL_UNSIGNED_INT: return 4;
	case GL_UNSIGNED_BYTE: return 1;
	}
	return 0;
}

VertexBufferLayout::VertexBufferLayout()
	: m_Stride(0) {}

void VertexBufferLayout::pushFloat(unsigned int count)
{
	m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
	m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const { return m_Elements; }
unsigned int VertexBufferLayout::getStride() const { return m_Stride; }