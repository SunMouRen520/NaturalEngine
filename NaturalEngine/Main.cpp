#ifndef GLAD_H
#define GLAD_H
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Engine/Window.h"
#include <iostream>
#include "Camera.h"

#include "DrawableObjects/drawableObjects.h"
#include "DrawableObjects/sceneElements.h"
#include "DrawableObjects/SkyBox.h"


int main()
{
	// 相机的设置
	glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	Camera camera(startPosition);

	// 开窗
	int sucess;
	Window window(sucess, 1600, 900);
	if (!sucess) return -1;

	window.camera = &camera;

	
	glm::vec3 fogColor(0.5, 0.6, 0.7);
	glm::vec3 lightColor(255, 255, 230);
	lightColor /= 255.0;

	FrameBufferObject SceneFBO(Window::SCR_WIDTH, Window::SCR_HEIGHT);
	glm::vec3 lightPosition, seed;
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom),(float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
	glm::vec3 lightDir = glm::vec3(-0.5, 0.5, 1.0);


	// 每个场景对象都需要渲染这些东西
	sceneElements scene;

	scene.sceneFBO = &SceneFBO;


	drawableObject::scene = &scene;

	SkyBox skybox;

	
	// 主循环
	while (window.continueLoop())
	{		
		scene.lightDir = glm::normalize(scene.lightDir);
		scene.lightPos = scene.lightDir * 1e6f + camera.Position;

		skybox.update();

		skybox.draw();

		window.swapBuffersAndPollEvents();
	}


	return 0;
}