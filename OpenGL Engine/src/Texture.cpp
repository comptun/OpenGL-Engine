#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

#include <iostream>
#include <vector>
#include <string>

Texture::Texture(const std::string path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Type(GL_TEXTURE_2D)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glCall(glGenTextures(1, &m_RendererID));
	glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); 

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::Texture() : m_RendererID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Type(0) {}

Texture::Texture(const std::vector<std::string>& textures)
	: m_RendererID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Type(GL_TEXTURE_CUBE_MAP)
{
	createCubemap(textures);
}

Texture::~Texture()
{
	glCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::createCubemap(const std::vector<std::string>& textures)
{
	glCall(glGenTextures(1, &m_RendererID));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	for (unsigned int i = 0; i < textures.size(); ++i) {
		m_LocalBuffer = stbi_load(textures[i].c_str(), &m_Width, &m_Height, &m_BPP, 4);
		if (m_LocalBuffer) {
			glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
			stbi_image_free(m_LocalBuffer);
		}
		else {
			std::cout << "Error: Failed to load cubemap texture '" + textures[i] << "'\n";
		}
	}
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Texture::bind(unsigned int slot) const
{
	glCall(glActiveTexture(GL_TEXTURE0 + slot))
	glCall(glBindTexture(m_Type, m_RendererID));
}
void Texture::unbind() const
{
	glCall(glBindTexture(m_Type, 0));
}