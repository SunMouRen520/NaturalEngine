#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// ���弸������ƶ��Ķ��������������Ա���ʹ���ض��ڴ���ϵͳ�����뷽��
enum Camera_Movement
{
	FORWORD,
	BACK,
	LEFT,
	RIGHT
};

// Ĭ�ϵ������ֵ
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2000.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;
const float MAX_FOV = 100.0f;


// һ������ĳ�����࣬�������벢������Ӧ��ŷ���ǣ���������������OpenGL
class Camera
{
public:
	// �������
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// ŷ����
	float Yaw;
	float Pitch;

	// ѡ�����
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// �������캯��
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :Front(glm::vec3(0.0f, 0.0f, -5.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVector();
	}

	// ����ָ���캯��
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -0.1f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX,posY,posZ);
		WorldUp = glm::vec3(upX,upY,upZ);
		Yaw = yaw;
		Pitch = pitch;
		UpdateCameraVector();
	}

	// ����ʹ��ŷ���Ǻ�LookAt����������ͼ����
	glm::mat4 GetViewMatrix()
	{
		UpdateCameraVector();
		
		return glm::lookAt(Position, Position + Front, Up);
	}

	// �����κδ����Ƽ��̺�����ϵͳ���ܵ����룬��������������ENUM��ʽ������������Ӵ���ϵͳ�г�������
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

	float Random2D(glm::vec2 st)
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
		float a = Random2D(randomInput);
		float b = Random2D(randomInput + glm::vec2(1.0f, 0.0f));
		float c = Random2D(randomInput + glm::vec2(0.0f, 1.0f));
		float d = Random2D(randomInput + glm::vec2(1.0f, 1.0f));
	
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
		for (int i = 0; i < octaves; ++i)
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

	// ������������ϵͳ���ܵ����룬
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// ȷ����pithch��������ʱ����Ļ���ᷴת
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// ʹ�ø��µ�ŷ���ǡ���ǰ�桢�ұߺ����棩������
		UpdateCameraVector();
	}

	// ������������¼����ܵ������룬ֻ��Ҫ���봹ֱ����
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom < MAX_FOV)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom > MAX_FOV)
			Zoom = MAX_FOV;
	}
	void invertPitch() {
		this->Pitch = -Pitch;
		UpdateCameraVector();
	}

private:
	 // �������ŷ���Ǽ���ǰ���������£�
	void UpdateCameraVector()
	{
		// �����µ�ǰ����
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
	
		// ��Ҫ���¼����Һ���������
		Right = glm::normalize(glm::cross(Front, WorldUp));	 // ��������һ������Ϊ�����ϻ����¿��Ĵ���Խ�࣬���ǵĳ��Ⱦ�Խ�ӽ�0����ᵼ���˶�������
	
		Up = glm::normalize(glm::cross(Right, Front));
	}



};

#endif
