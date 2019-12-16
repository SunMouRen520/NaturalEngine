#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"



class Window
{
public:
	//��ʼ��
	Window(int &success, unsigned int SCR_W, unsigned int SCR_H, std::string name = "NaturalEngine OpenGL");
	~Window();
	GLFWwindow *window;
	GLFWwindow *getwindow() const { return window; }

	// ���봦��
	void processInput(float frameTime);

	void terminate()
	{
		glfwTerminate();
	}


	// �����������ѭ�������
	void swapBuffersAndPollEvents()
	{
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	// ������رմ��ڣ���һֱѭ��
	bool continueLoop()
	{
		return !glfwWindowShouldClose(this->window);
	}

public:
	static Camera *camera;
	// ��Ļ����
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

private:
	// �ص�
	static void framebuffer_size_callbacke(GLFWwindow* window, int width, int heigth);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	// ������겢����OpenGL����
	int gladLoader();

private:
	std::string name;
	

	

	// �������ް���
	static bool keyBools[10];

	static bool mouseCursorDisable;
	
	// �߿�ģʽ
	static bool wireframe;

	int oldState, newState;

	// �����ڳ���ʼʱ������Ծ
	static bool firstMouse;	// = true
	static float lastX;
	static float lastY;

};
