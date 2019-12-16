#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include "DrawableObjects/drawableObjects.h"


// 重构那些只写入屏幕空间的着色器（如：post processint,ShaderToy.com）,因此只需要片段着色器，模型只需要两个三角形
class ScreenSpaceShader : public drawableObject
{
public:
	ScreenSpaceShader(const char* fragmentPath);
	~ScreenSpaceShader();

	Shader * const getShaderPtr()
	{
		return shad;
	}

	Shader & const getShader()
	{
		return *shad;
	}

	virtual void draw();
	static void drawQuad();

	static void disableTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	static void enableTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

private:
	void initializeQuad();

private:
	Shader * shad;
	static unsigned int quadVAO, quadVBO;
	static bool initialized;
};