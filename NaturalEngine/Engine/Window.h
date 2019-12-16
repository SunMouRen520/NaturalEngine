#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"



class Window
{
public:
	//初始化
	Window(int &success, unsigned int SCR_W, unsigned int SCR_H, std::string name = "NaturalEngine OpenGL");
	~Window();
	GLFWwindow *window;
	GLFWwindow *getwindow() const { return window; }

	// 输入处理
	void processInput(float frameTime);

	void terminate()
	{
		glfwTerminate();
	}


	// 把这个放在主循环的最后
	void swapBuffersAndPollEvents()
	{
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	// 如果不关闭窗口，就一直循环
	bool continueLoop()
	{
		return !glfwWindowShouldClose(this->window);
	}

public:
	static Camera *camera;
	// 屏幕设置
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

private:
	// 回调
	static void framebuffer_size_callbacke(GLFWwindow* window, int width, int heigth);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// 设置鼠标并加载OpenGL函数
	int gladLoader();

private:
	std::string name;
	

	

	// 避免无限按键
	static bool keyBools[10];

	static bool mouseCursorDisable;
	
	// 线框模式
	static bool wireframe;

	int oldState, newState;

	// 避免在程序开始时鼠标就跳跃
	static bool firstMouse;	// = true
	static float lastX;
	static float lastY;

};
