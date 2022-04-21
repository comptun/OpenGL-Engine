#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	GLenum m_Type;
public:
	Texture(const std::string path);
	Texture(const std::vector<std::string>& textures);
	Texture();
	~Texture();

	void createCubemap(const std::vector<std::string>& textures);

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
};

