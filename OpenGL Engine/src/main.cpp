#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr int WIDTH = 1440, HEIGHT = 720;

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

    {
        float positions[] = {
            -0.5, -0.5, -0.5,
            -0.5, -0.5,  0.5,
             0.5, -0.5,  0.5,
             0.5, -0.5, -0.5,
            -0.5,  0.5, -0.5,
            -0.5,  0.5,  0.5,
             0.5,  0.5,  0.5,
             0.5,  0.5, -0.5,
        };

        unsigned int indices[] = {
            0, 1, 5,
            0, 4, 5,
            1, 2, 6,
            1, 5, 6,
            2, 3, 7,
            2, 6, 7,
            3, 7, 4,
            3, 0, 4,
            4, 5, 6,
            4, 7, 6,
            0, 1, 2,
            0, 3, 2,
        };

        glCall(glEnable(GL_DEPTH_TEST));
        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;

        layout.push<float>(3);
        va.addBuffer(vb, layout);

        IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

        Shader shader("res/shaders/basic.shader");
        Texture texture(std::vector<std::string>{
            "res/textures/skybox/right.jpg",
            "res/textures/skybox/left.jpg",
            "res/textures/skybox/top.jpg",
            "res/textures/skybox/bottom.jpg",
            "res/textures/skybox/front.jpg",
            "res/textures/skybox/back.jpg"
        });

        texture.bind(0);
        shader.bind();
        shader.setUniform1i("u_Texture", 0);
        
        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 blockPos(0.0f, 0.0f, -10.0f);
        glm::vec3 blockPos2(1.0f, 0.0f, -10.0f);

        glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
        
        float increment = 0.1f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPos);
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH / HEIGHT), 0.1f, 1000.0f);


            /* Render here */
            renderer.clear();

            ImGui_ImplGlfwGL3_NewFrame();

            glCall(glDisable(GL_DEPTH_TEST));

            {
                static float rot = 0.0f;
                shader.bind();
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
                model = glm::rotate(model, glm::radians<float>(rot), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 mvp = proj * model;
                rot = rot + 0.01;
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }

            glCall(glEnable(GL_DEPTH_TEST));

            {
                static float rot = 0.0f;
                shader.bind();
                glm::mat4 model = glm::translate(glm::mat4(1.0f), blockPos);
                model = glm::rotate(model, glm::radians<float>(rot), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 mvp = proj * view * model;
                rot = rot + increment;
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }
            {
                static float rot = 0.0f;
                shader.bind();
                glm::mat4 model = glm::translate(glm::mat4(1.0f), blockPos2);
                model = glm::rotate(model, glm::radians<float>(rot), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 mvp = proj * view * model;
                rot = rot + increment;
                shader.setUniformMat4f("u_MVP", mvp);
                renderer.draw(va, ib, shader);
            }

            {
                ImGui::Begin("Debugger");
                ImGui::Text("Hello, world!");
                ImGui::SliderFloat3("TranslationA", &cameraPos.x, -10.0f, 10.0);

                 ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            glCall(glfwPollEvents());
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}