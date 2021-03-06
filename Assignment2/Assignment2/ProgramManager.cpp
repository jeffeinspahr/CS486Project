//
//  ProgramManager.cpp
//  Assignment1
//
//  Created by Jeff Einspahr on 11/23/13.
//  Copyright (c) 2013 Jeff Einspahr. All rights reserved.
//

#define Update_Per_Second 60.0f

#include "ProgramManager.h"

ProgramManager::ProgramManager(bool running):
_running(running),
_window(glfwGetCurrentContext()),
_renderSystem(&RenderSystem::getRenderSystem()),
_resourceManager(&ResourceManager::getResourceManager()),
_movementSystem(&MovementSystem::getMovementSystem()),
_cameraSystem(&CameraSystem::getCameraSystem()),
scene(new Scene)
//_userInputSystem(&UserInputSystem::getUserInputSystem())
{
}

ProgramManager::~ProgramManager()
{
    ResourceManager::destroyResourceManager();
    CameraSystem::destroyCameraSystem();
    RenderSystem::destroyRenderSystem();
}

void ProgramManager::runProgramLoop()
{
    double lastTime = glfwGetTime();
    double deltaTime = 0.0f;
    while (_running)
    {
        double currentTime = glfwGetTime();
        deltaTime += (currentTime - lastTime) * Update_Per_Second;
        lastTime = currentTime;
        
        while (deltaTime >= 1.0f) {
            
            _running = !glfwWindowShouldClose(_window);
            if (glfwGetKey(_window, GLFW_KEY_Q)) {
                _running = false;
            }
            
            /*if (entity->getPosition().x == 0.9f && entity->getVelocity().x > 0.0f) {
                std::cout << "Hello" << std::endl;
                entity->setVelocity(makeVec3((-1 * entity->getVelocity().x), entity->getVelocity().y, entity->getVelocity().z));
            }*/
            
            _movementSystem->update(scene->getChildren());
            
            --deltaTime;
        }
        _renderSystem->render(scene->getChildren());
        
    }
}

ProgramManager& ProgramManager::getProgramManager()
{
    static ProgramManager *programManager = NULL;
    if(programManager == NULL)
    {
        glfwInit();
        
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        
        //samples of rendering to smooth edges
        glfwWindowHint(GLFW_SAMPLES, 16);
        
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
        GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL Program", NULL, NULL);
        glfwMakeContextCurrent(window);
        programManager = new ProgramManager(true);
        std::cout << "Program Created\n";
    }
    return *programManager;
}
void ProgramManager::destroyProgramManager()
{
    ProgramManager *ProgramManager = &getProgramManager();
    delete ProgramManager;
    std::cout <<"Program Destroyed\n";
    GLFWwindow *window = glfwGetCurrentContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}