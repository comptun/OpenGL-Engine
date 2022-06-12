#include "Engine.h"

int Engine::setup()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
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

    glCall(glEnable(GL_DEPTH_TEST));
    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    return 0;
}

bool Engine::hasFailed() const
{
    return failed;
}
GLFWwindow* Engine::getWindow()
{
    return window;
}

Engine::Engine(int w, int h) : WIDTH(w), HEIGHT(h), failed(false)
{
    if (setup() == -1)
        failed = true;
}