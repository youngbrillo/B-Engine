#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "App.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback_cursor(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ResizeWindowCallback(GLFWwindow* window, int newWidth, int newHeight);

// The Width of the screen

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 500;

void cus_glfw_initialization();
void cus_glfw_callback_initialization(GLFWwindow* window);
void cus_imgui_init_config(GLFWwindow* win);


CSApp* application;
int main(char* argc, int argv[])
{
    //INITIALIZATION
    cus_glfw_initialization();

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Code Switcher", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  {  printf("Failded to Initialize GLAD\n"); return -1;  }


    // Load OpenGL functions using glad
    //int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
   // printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));


    // SET UP CALLBACKS
    cus_glfw_callback_initialization(window);
    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // ImGui configuration
    // -------------------
    //cus_imgui_init_config(window);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //INITIALIZE THE APP


    //final setup
    application = new App(SCREEN_WIDTH, SCREEN_HEIGHT);

    App::mainWindow = window;
    Game::mainWindow = window;

    application->Init();


    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float accumulator = 0.0f;
    float timeStep = 0.01f; // ~1/60
    float t = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        accumulator += deltaTime;
        glClearColor(Game::windowColorBackGround.x, Game::windowColorBackGround.y, Game::windowColorBackGround.z, Game::windowColorBackGround.w);
        glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        application->ProcessInput(window, deltaTime);
        application->Update(deltaTime);
        application->Render();




        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    delete application;
    // Deletes all ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

	return 0;
}
void cus_glfw_initialization()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, true);

}
void cus_glfw_callback_initialization(GLFWwindow* window)
{
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, mouse_callback_cursor);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, ResizeWindowCallback);


}
void cus_imgui_init_config(GLFWwindow* win)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

//	UTILITIES -----------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    application->KeyCallBack(window, key, scancode, action, mode);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) {
            application->setKey(key, true);
        }
        else if (action == GLFW_RELEASE) {
            application->setKey(key, false);

        }
    }
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    application->mouseCallback(window, button, action, mods);

}

void mouse_callback_cursor(GLFWwindow* window, double xpos, double ypos)
{
    application->mouseCallback_Cursor(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    application->mouseScroll_callback(window, xoffset, yoffset);
}

void ResizeWindowCallback(GLFWwindow* window, int newWidth, int newHeight)
{
    if(newWidth > 0 && newHeight > 0)
        application->ResizeWindowCallback(window, newWidth, newHeight);
}
