#include "Shader.h"
#include "Renderer.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>

Shader::Shader(const std::string filePath)
	: m_FilePath(filePath), m_RendererID(0)
{
    ShaderSource source = parseShaders(filePath);
    m_RendererID = createShader(source.vertex, source.fragment);
}
Shader::~Shader()
{
    glCall(glDeleteProgram(m_RendererID));
}

ShaderSource Shader::parseShaders(const std::string filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    ShaderSource contents;
    std::string currentShader;
    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                currentShader = "vertex";
            }
            else if (line.find("fragment") != std::string::npos) {
                currentShader = "fragment";
            }
            continue;
        }
        if (currentShader == "vertex")
            contents.vertex += line + "\n";
        else if (currentShader == "fragment")
            contents.fragment += line + "\n";
    }
    return contents;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    glCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    glCall(glShaderSource(id, 1, &src, nullptr));
    glCall(glCompileShader(id));

    int result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        glCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!\n";
        std::cout << message << "\n";
        glCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    glCall(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));

    return program;
}

void Shader::bind() const
{
    glCall(glUseProgram(m_RendererID));
}
void Shader::unbind() const
{
    glCall(glUseProgram(0));
}

// set uniforms
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    unsigned int location = getUniformLocation(name);
    glCall(glUniform4f(location, v0, v1, v2, v3));
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    glCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";

    m_UniformLocationCache[name] = location;
    return (unsigned)location;
}