#include "Engine.h"

int WIDTH = 1600, HEIGHT = 800;

Camera camera(glm::vec3(2,2,20));

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static float yaw = -90.0f, pitch = 0.0f;
    static bool firstMouse = true;
    static float lastX = (float)(WIDTH / 2), lastY = (float)(HEIGHT / 2);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.9f)
        pitch = 89.9f;
    if (pitch < -89.9f)
        pitch = -89.9f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera.setFront(glm::normalize(direction));
}

int main(void)
{
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    Engine engine(WIDTH, HEIGHT);

    if (engine.hasFailed())
        return -1;

    {

        Block block;

        Texture skybox(std::vector<std::string>{
            "res/textures/skybox/right.jpg",
                "res/textures/skybox/left.jpg",
                "res/textures/skybox/top.jpg",
                "res/textures/skybox/bottom.jpg",
                "res/textures/skybox/front.jpg",
                "res/textures/skybox/back.jpg"
        });

        Texture dirt(std::vector<std::string>{
            "res/textures/dirt2.png",
                "res/textures/dirt2.png",
                "res/textures/dirt.png",
                "res/textures/dirt2.png",
                "res/textures/dirt2.png",
                "res/textures/dirt2.png"});
        
        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfwGL3_Init(engine.getWindow(), true);
        ImGui::StyleColorsDark();

        glfwSetKeyCallback(engine.getWindow(), keyCallback);
        glfwSetCursorPosCallback(engine.getWindow(), mouseCallback);

        glfwSetInputMode(engine.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(engine.getWindow()))
        {
            glfwGetFramebufferSize(engine.getWindow(), &WIDTH, &HEIGHT);
            glViewport(0, 0, WIDTH, HEIGHT);
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH / HEIGHT), 0.1f, 1000.0f);
            glm::mat4 view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getFront(), camera.getUp());
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            /* Render here */
            renderer.clear();

            ImGui_ImplGlfwGL3_NewFrame();
            
            skybox.bind(0);
            block.setTexture(0);
            renderer.drawSkybox(block.va, block.ib, block.shader, proj * glm::lookAt(glm::vec3(0, 0, 0), camera.getFront(), camera.getUp()));

            const int distance = 50;

            dirt.bind(1);
            block.setTexture(1);
            for (int x = camera.getPos().x - distance; x < camera.getPos().x + distance; ++x) {
                for (int z = camera.getPos().z - distance; z < camera.getPos().z + distance; ++z) {
                    const double n = noise.GetNoise((float)x, (float)z);
                    renderer.draw(block.va, block.ib, block.shader, proj * view * glm::translate(glm::mat4(1.0f), glm::vec3(x, (int)(n*10), z)));
                }
            }

            {
                ImGui::Begin("Debugger");
                ImGui::Text("Hello, world!");
                //ImGui::SliderFloat3("TranslationA", &cameraPos.x, -30.0f, 30.0);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            processInput(camera, deltaTime);

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glCall(glfwSwapBuffers(engine.getWindow()));

            /* Poll for and process events */
            glCall(glfwPollEvents());
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}