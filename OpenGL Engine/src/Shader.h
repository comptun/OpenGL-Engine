#pragma once

#include <string>
#include <unordered_map>

struct ShaderSource
{
	std::string vertex;
	std::string fragment;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string filePath);
	~Shader();

	void bind() const;
	void unbind() const;

	// set uniforms
	void setUniform1i(const std::string& name, int v0);
	void setUniform1f(const std::string& name, float v0);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	ShaderSource parseShaders(const std::string filePath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	int getUniformLocation(const std::string& name);
};