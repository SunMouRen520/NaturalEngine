#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include "DrawableObjects/drawableObjects.h"


// �ع���Щֻд����Ļ�ռ����ɫ�����磺post processint,ShaderToy.com��,���ֻ��ҪƬ����ɫ����ģ��ֻ��Ҫ����������
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