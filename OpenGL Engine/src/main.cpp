#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"



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
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
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

    {
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

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        va.addBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("basic.shader");
        shader.bind();
        shader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        
        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        float r = 0.0;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.bind();
            shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            va.bind();
            ib.bind();

            glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f || r < 0.0f)
                increment = -increment;
            r += increment;

            /* Swap front and back buffers */
            glCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            glCall(glfwPollEvents());
        }
    }

    glfwTerminate();
    return 0;
}