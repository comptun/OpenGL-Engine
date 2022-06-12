#pragma once

#include <vector>
#include <GL/glew.h>

class VertexBufferElement
{
public:
	unsigned int type;
	unsigned int count;
	unsigned char normalised;

	static unsigned int getSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout();

	void pushFloat(unsigned int count);

	const std::vector<VertexBufferElement>& getElements() const;
	unsigned int getStride() const;
};