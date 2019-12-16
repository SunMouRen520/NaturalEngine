#ifndef GLAD_H
#define GLAD_H
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Engine/Window.h"
#include <iostream>
#include "Camera.h"

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

	

	
	// 主循环
	while (window.continueLoop())
	{


		window.swapBuffersAndPollEvents();
	}


	return 0;
}