#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include "ArtifactRoomScene.cpp"
#include "BlackHoleRoom.cpp"

// Function prototypes
void CameraController(GLFWwindow* window, Scene* currentScene, float dt);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos);


/* Globals */
/* --------*/
float window_width = 1024.0f;
float window_height = 768.0f;

// Camera/Mouse Controls
bool bLockCursor = true;
bool bFirstMouse = true;
float fCameraSensitivity = 0.05;
float fCameraMovementSpeed = 10.0f;
float fCameraZoomSpeed = 0.1f;

Scene* currentScene;

int main()
{
    srand(static_cast <unsigned int> (time(0)));

    /* Initializing Window
	--------------------------------*/
    glfwInit();

    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "COMP 371 - Interactive Museum - Team 4", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialize GLFW Input
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_movement_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Set background colour
    glClearColor(0.4f, 0.8f, 0.8f, 1.0f);

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Entering Main Loop
    float dt;
    int width, height;
    
    // Frame Timing
    float lastFrameTime = 0.0f;
    float lastButtonTime = 0.0f;
    
    /* Scene Initialization */
    ArtifactRoomScene* ArtifactRoom = new ArtifactRoomScene(window_width, window_height);
    BlackHoleRoomScene* BlackHoleRoom = new BlackHoleRoomScene(window_width, window_height);
    ArtifactRoom->Initialize();
    BlackHoleRoom->Initialize();
    currentScene = ArtifactRoom;
    currentScene->SetKeyCallback(window);

    /* Initialize Cursor */
    glfwSetCursorPos(window, window_width/2, window_height/2);

    while(!glfwWindowShouldClose(window))
    {
		// Frame time calculation
		dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

        currentScene->InputController(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentScene->Update(dt);

        /* Input */
        CameraController(window, currentScene, dt);
        currentScene->UpdateCameraView();

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}

void CameraController(GLFWwindow* window, Scene* currentScene, float dt)
{
    glm::mat4 transform;
    // Move Forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
        currentScene->TranslateCameraForward(dt * fCameraMovementSpeed);
    }
    // Move Backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
        currentScene->TranslateCameraBackward(dt * fCameraMovementSpeed);
    }
    // Strafe Left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    {
        currentScene->TranslateCameraLeft(dt * fCameraMovementSpeed);
    }
    // Strafe Right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
    {
        currentScene->TranslateCameraRight(dt * fCameraMovementSpeed);
    }
    /*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if(bLockCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        bLockCursor = !bLockCursor;
    }*/
}
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos)
{
    /* Mouse Controls */
    if(bLockCursor)
    {
        float x_diff, y_diff;
        float yaw = currentScene->GetCameraYaw();
        float pitch = currentScene->GetCameraPitch();

        x_diff = (window_width/2 - (float)xpos) * fCameraSensitivity;
        y_diff = (window_height/2 - (float)ypos) * fCameraSensitivity;

        // Because.
        if (bFirstMouse)
        {
            x_diff = 90.0f;
            y_diff = 0;
            bFirstMouse = false;
        }

        yaw += -x_diff;
        pitch += y_diff;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if( pitch < -70.0f)
            pitch = -70.0f;

        currentScene->RotateCameraEuler(yaw, pitch);

        glfwSetCursorPos(window, window_width/2, window_height/2);
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}