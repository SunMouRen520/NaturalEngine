#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 定义几个相机移动的动作，用作抽象，以避免使用特定于窗口系统的输入方法
enum Camera_Movement
{
	FORWORD,
	BACK,
	LEFT,
	RIGHT
};

// 默认的相机的值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2000.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;
const float MAX_FOV = 100.0f;


// 一个相机的抽象基类，处理输入并计算相应的欧拉角，向量，矩阵用于OpenGL
class Camera
{
public:
	// 相机属性
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// 欧拉角
	float Yaw;
	float Pitch;

	// 选择相机
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// 向量构造函数
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :Front(glm::vec3(0.0f, 0.0f, -5.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		Up = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVector();
	}

	// 标量指构造函数
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -0.1f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX,posY,posZ);
		Up = glm::vec3(upX,upY,upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVector();
	}

	// 返回使用欧拉角和LookAt矩阵计算的视图矩阵
	glm::mat4 GetViewMatrix()
	{
		UpdateCameraVector();
		
		return glm::lookAt(Position, Position + Front, Up);
	}

	// 处理任何从类似键盘和输入系统接受的输入，接受摄像机定义的ENUM形式的输入参数（从窗口系统中抽象它）
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		switch (direction)
		{
		case FORWORD:
			Position += Front * velocity;
			break;
		case BACK:
			Position -= Front * velocity;
			break;
		case LEFT:
			Position += Right * velocity;
			break;
		case RIGHT:
			Position -= Right * velocity;
			break;
		}
	}

	float Randon2D(glm::vec2 st)
	{
		return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
	}

	float InterpolateNoise(int ind, float x, float y)
	{
		float integer_x = float(x);
		float fractional_x = glm::fract(x);
		float integer_y = float(y);
		float fractional_y = glm::fract(y);
		glm::vec2 randomInput = glm::vec2(integer_x, integer_y);
		float a = Randon2D(randomInput);
		float b = Randon2D(randomInput + glm::vec2(1.0f, 0.0f));
		float c = Randon2D(randomInput + glm::vec2(0.0f, 1.0f));
		float d = Randon2D(randomInput + glm::vec2(1.0f, 1.0f));

		glm::vec2 w = glm::vec2(fractional_x, fractional_y);
		w = w * w * w * (10.0f + w * (-15.0f + 6.0f * w));
		float k0 = a,
			k1 = b - a,
			k2 = c - a,
			k3 = d - c - b + a;

		return k0 + k1 * w.x + k2 * w.y + k3 * w.x * w.y;
	}

	float perlin(glm::vec2 st, int octaves, float freq, float gDispFactor)
	{
		glm::mat2 m;
		m[0][0] = 0.8;
		m[0][1] = -0.6;
		m[1][0] = 0.6;
		m[1][1] = 0.8;

		float persistence = 0.5;
		float total = 0.0,
			frequency = 0.005 * freq,
			amplitude = gDispFactor;
		for (int i = 0; i < octaves; i++)
		{
			frequency *= 2.0;
			amplitude *= persistence;

			glm::vec2 v = frequency * st;
			total += InterpolateNoise(0, v.x, v.y) * amplitude;
		}

		return total * total * total;
	}

	void ProjectCameraPosition()
	{
		glm::vec2 st = glm::vec2(Position.x, Position.y);
		float y = perlin(st, 13, 0.01, 20);
		Position.y = y;
	}

	// 处理从鼠标输入系统接受的输入，
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// 确保当pithch超过界限时，屏幕不会反转
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// 使用更新的欧拉角、（前面、右边和上面）的向量
		UpdateCameraVector();
	}

	// 处理从鼠标滚动事件接受到的输入，只需要输入垂直轮轴
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom < MAX_FOV)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom > MAX_FOV)
			Zoom = MAX_FOV;
	}

private:
	// 以相机的欧拉角计算前向量（更新）
	void UpdateCameraVector()
	{
		// 计算新的前向量
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		// 还要重新计算右和向上向量
		Right = glm::normalize(glm::cross(Front, WorldUp));	 // 将向量归一化，因为你向上或向下看的次数越多，它们的长度就越接近0，这会导致运动变慢。

		Up = glm::normalize(glm::cross(Right, Front));
	}

};

#endif
