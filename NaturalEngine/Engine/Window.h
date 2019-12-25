#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>
#include <iostream>



class Window
{
public:
	//initializer
	//初始化
	Window(int& success, unsigned int SCR_WIDTH = 1600, unsigned int SCR_HEIGHT = 900, std::string name = "TerrainEngine OpenGL");
	~Window();
	GLFWwindow * w;
	GLFWwindow * getWindow() const { return w; }

	void processInput(float frameTime); //input handler 输入处理

	// screen settings
	// 屏幕设置
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	void terminate() {
		glfwTerminate();
	}

	bool isWireframeActive() {
		return Window::wireframe;
	}

	// return if the main loop must continue
	// 如果主循环必须继续，则返回
	bool continueLoop() {
		return !glfwWindowShouldClose(this->w);
	}

	//put this at the end of the main
	// 把这个放在main的最后
	void swapBuffersAndPollEvents() {
		glfwSwapBuffers(this->w);
		glfwPollEvents();
	}

	static Camera * camera;

private:
	int oldState, newState;
	int gladLoader(); // set mouse input and load opengl functions   设置鼠标输入并加载opengl函数

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// avoid infinite key press
	// 避免无限按键
	static bool keyBools[10];

	static bool mouseCursorDisabled;

	// wireframe mode
	// 线框模式
	static bool wireframe;

	//avoid to make the mouse to jump at the start of the program
	// 避免使鼠标在程序开始时跳跃
	static bool firstMouse;// = true;
	static float lastX;
	static float lastY;

	std::string name;
};


