#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "Engine/Shader.h"
#include "Engine/buffer.h"
#include <GLFW/glfw3.h>
#include "Engine/Window.h"
#include "drawableObjects.h"

class Water : public drawableObject
{
public:
	Water(glm::vec2 position, float scale, float height);
	virtual ~Water();

	virtual void draw();
	virtual void setGui();
	void bindRefractionFBO();	//ÕÛÉä
	void bindReflectionFBO();	//·´Éä
	void unbindFBO();

	void setPosition(glm::vec2 position, float scale, float height)
	{
		glm::mat4 identity;
		glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
		glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position.x, height, position.y));
		this->modelMatrix = transMatrix * scaleMatrix;
	}

	void setHeight(float height)
	{
		float scale = modelMatrix[0][0];
		float position_x = modelMatrix[3][0];
		float position_y = modelMatrix[3][2];

		glm::mat4 identity;
		glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
		glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position_x, height, position_y));
		this->modelMatrix = transMatrix * scaleMatrix;
	}

	float getHeight()
	{
		return height;
	}
	

	glm::mat4 getModelMatrix()
	{
		return modelMatrix;
	}

	FrameBufferObject const& getReFlectionFBO()
	{
		return *reflectionFBO;
	}

	static const int FBOw = 1280;
	static const int FBOh = 720;

private:
	void drawVertices();

	unsigned int planeVAO, planeVBO, planeEBO;
	float scale, height;
	FrameBufferObject * refractionFBO;
	FrameBufferObject * reflectionFBO;

	unsigned int dudvMap, normalMap;
	glm::mat4 modelMatrix;
	Shader * shad;

};