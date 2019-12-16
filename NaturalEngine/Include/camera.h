#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
// 定义相机移动的几个可能选项。用作抽象，以避免使用特定于窗口系统的输入方法
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
// 默认的相机值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2000.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;
const float MAX_FOV = 100.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
// 一个抽象的摄像机类，处理输入并计算相应的欧拉角、向量和矩阵，用于OpenGL
class Camera
{
public:
	// Camera Attributes
	// 相机属性
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	// 欧拉角
	float Yaw;
	float Pitch;
	// Camera options
	// 相机的选择
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	// 构造函数与向量
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -5.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	// 标量值构造函数
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	// 返回使用欧拉角和LookAt矩阵计算的视图矩阵
	glm::mat4 GetViewMatrix()
	{
		updateCameraVectors();
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	// 处理从任何类似键盘的输入系统接收的输入。接受摄像机定义的ENUM形式的输入参数(从窗口系统中抽象它)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;

		//projectCameraPosition();
	}


	float Random2D(glm::vec2 st)
	{
		return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
	}

	float InterpolatedNoise(int ind, float x, float y) {
		float integer_X = floor(x);
		float fractional_X = glm::fract(x);
		float integer_Y = floor(y);
		float fractional_Y = glm::fract(y);
		glm::vec2 randomInput = glm::vec2(integer_X, integer_Y);
		float a = Random2D(randomInput);
		float b = Random2D(randomInput + glm::vec2(1.0, 0.0));
		float c = Random2D(randomInput + glm::vec2(0.0, 1.0));
		float d = Random2D(randomInput + glm::vec2(1.0, 1.0));
		//float v1 = Random2D(randomInput);
		//float v2 = Random2D(randomInput + vec2(1.0, 0.0));
		//float v3 = Random2D(randomInput + vec2(0.0, 1.0));
		//float v4 = Random2D(randomInput + vec2(1.0, 1.0));

		glm::vec2 w = glm::vec2(fractional_X, fractional_Y);
		w = w * w*w*(10.0f + w * (-15.0f + 6.0f*w));

		//fractional_X = smoothstep(0.0, 1.0, fractional_X);
		//fractional_Y = smoothstep(0.0, 1.0, fractional_Y);
		//return a + fractional_X*(b-a) + fractional_Y*c + fractional_X*fractional_Y*(d-c) - a*fractional_Y - fractional_X*fractional_Y*(b-a);
		float k0 = a,
			k1 = b - a,
			k2 = c - a,
			k3 = d - c - b + a;

		return k0 + k1 * w.x + k2 * w.y + k3 * w.x*w.y;
	}

	float perlin(glm::vec2 st, int octaves, float freq, float gDispFactor) {

		//st *= freq;
		glm::mat2 m;// = glm::mat2(0.8, -0.6, 0.6, 0.8);
		m[0][0] = 0.8;
		m[0][1] = -0.6;
		m[1][0] = 0.6;
		m[1][1] = 0.8;
		//int numOctaves = 10;
		float persistence = 0.5;
		float total = 0.0,
			frequency = 0.005*freq,
			amplitude = gDispFactor;
		for (int i = 0; i < octaves; ++i) {
			frequency *= 2.0;
			amplitude *= persistence;

			//st = frequency*m*st;

			glm::vec2 v = frequency * st;

			total += InterpolatedNoise(0, v.x, v.y) * amplitude;
		}
		return total*total*total;
	}

	void projectCameraPosition() {
		glm::vec2 st = glm::vec2(Position.x,Position.z);
		float y = perlin(st, 13, 0.01, 20.);
		Position.y = y;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	// 处理从鼠标输入系统接收到的输入。期望在x和y方向上的偏移值。
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		// 确保当音高超出界限时，屏幕不会翻转
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		// 使用更新的欧拉角更新前面、右边和上面的向量
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	// 处理从鼠标滚动轮事件接收到的输入。只需要输入垂直轮轴
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= MAX_FOV)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= MAX_FOV)
			Zoom = MAX_FOV;
	}

	void invertPitch() {
		this->Pitch = -Pitch;
		updateCameraVectors();
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	// 从相机的(更新)欧拉角计算前向量
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		// 计算新的前向量
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		// 还要重新计算右向上向量
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
															 // 将向量归一化，因为你向上或向下看的次数越多，它们的长度就越接近0，这会导致运动变慢。
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif