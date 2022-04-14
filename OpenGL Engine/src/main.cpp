#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x; \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

static void glClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool glLogCall(const char* function, const char* file, unsigned int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (0x0" << std::hex << error << std::dec << "): "
                  << function << " " << file << ":" << line << "\n";
        return false;
    }
    return true;
}

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};

static ShaderSource parseShaders(const std::string filePath)
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

static unsigned int compileShader(unsigned int type, const std::string& source)
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

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glCall(glfwTerminate());
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error! GLEW could not be initialised.\n";

    std::cout << glGetString(GL_VERSION) << "\n";

    float positions[] = {
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
       -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    glCall(glGenVertexArrays(1, &vao));
    glCall(glBindVertexArray(vao));

    unsigned int buffer;
    glCall(glGenBuffers(1, &buffer));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    glCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    glCall(glEnableVertexAttribArray(0));
    glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));

    unsigned int ibo;
    glCall(glGenBuffers(1, &ibo));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderSource source = parseShaders("basic.shader");
    unsigned int shader = createShader(source.vertex, source.fragment);
    glCall(glUseProgram(shader));

    glCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    glCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    glCall(glBindVertexArray(0));
    glCall(glUseProgram(0));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    float r = 0.0;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glCall(glClear(GL_COLOR_BUFFER_BIT));
        
        glCall(glUseProgram(shader));
        glCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

        glCall(glBindVertexArray(vao));
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f || r < 0.0f)
            increment = -increment;
        r += increment;

        /* Swap front and back buffers */
        glCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        glCall(glfwPollEvents());
    }

    glCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}