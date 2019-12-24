#include "Window.h"

unsigned int Window::SCR_WIDTH = 1600;
unsigned int Window::SCR_HEIGHT = 900;

Camera * Window::camera = 0;

bool Window::keyBools[10] = { false,false, false, false, false, false, false, false, false, false };
bool Window::wireframe = false;
bool Window::firstMouse = true;
float Window::lastX = SCR_WIDTH / 2.0f;
float Window::lastY = SCR_HEIGHT / 2.0f;
bool Window::mouseCursorDisable = true;

Window::Window(int & success, unsigned int SCR_W, unsigned int SCR_H, std::string name) :name (name)
{
	Window::SCR_WIDTH = SCR_W;
	Window::SCR_HEIGHT = SCR_H;
	success = 1;

	// glfw: ��ʼ������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// glfw: ��������
	this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str(), NULL, NULL);
	if (!this->window)
	{
		std::cout << "Faild to create GLFW window" << std::endl;
		glfwTerminate();
		success = 0;
		return;
	}

	glfwMakeContextCurrent(this->window);
	glfwSetFramebufferSizeCallback(this->window, &Window::framebuffer_size_callbacke);
	glfwSetCursorPosCallback(this->window, &Window::mouse_callback);
	glfwSetScrollCallback(this->window, &Window::scroll_callback);

	Window::camera = 0;
	oldState = newState = GLFW_RELEASE;

	success = gladLoader() && success;
	if (success)
	{
		std::cout << "GLFW window correctly initialize" << std::endl;
	}
}

Window::~Window()
{
	this->terminate();
}

void Window::processInput(float frameTime)
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWORD, frameTime);
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACK, frameTime);
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, frameTime);
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, frameTime);

	newState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT);

	if (newState == GLFW_RELEASE && oldState == GLFW_PRESS)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, (mouseCursorDisable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
		mouseCursorDisable = !mouseCursorDisable;
		if (mouseCursorDisable)
			firstMouse = true;
	}

	oldState = newState;

	// �߿�
	if (glfwGetKey(this->window, GLFW_KEY_T) == GLFW_PRESS)
	{
		if (keyBools[4] == false)
		{
			wireframe = !wireframe;
			keyBools[4] = true;
		}
	}
	else if(glfwGetKey(this->window, GLFW_KEY_T) == GLFW_RELEASE)
	{
		if (keyBools[4] == true)
		{
			keyBools[4] = false;
		}
	}
}

// �����ڴ�С�����仯��ʱ�򣬵��ô˺���
void Window::framebuffer_size_callbacke(GLFWwindow * window, int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

// ������ƶ�ʱ�����ô˺�����
void Window::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;	// ��ΪY������ӵײ����������ߵ���

	if (!mouseCursorDisable)
	{
		Window::camera->ProcessMouseMovement(xoffset, yoffset);
	}
}

// �������ֹ���ʱ�����ô˻ص���
void Window::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	Window::camera->ProcessMouseScroll(yoffset);
}

int Window::gladLoader()
{
	// ����GLFWץס���ǵ����
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// ��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild to initialize glad" << std::endl;
		return 0;
	}
	return 1;
}
