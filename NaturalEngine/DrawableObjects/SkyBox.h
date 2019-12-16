#pragma once

#include <glm/glm.hpp>
#include "Engine/ScreenSpaceShader.h"
#include "drawableObjects.h"
#include <glm/gtc/matrix_transform.hpp>

struct colorPreset
{
	glm::vec3 cloudColorBottom, skyColorTop, skyColorBottom, lightColor, fogColor;
};

// ����ദ����ռ�����ɫ��������Ļ�ռ��л��ƿռ䣬��ΪΨһ��Ƭ����ɫ��
// �����һ������ִ�д�����ɢ�䣬����ʹ��ͨ������պУ���Ϊ����һ���ܺõ�ռλ����

class SkyBox : public drawableObject
{
public:
	//friend class VolumetricClouds;
	SkyBox();
	~SkyBox();

	virtual void draw();
	virtual void update();
	virtual void setGui();

	colorPreset DefaultPreset();
	colorPreset SunsetPreset();
	colorPreset SunsetPreset1();

	void mixSkyColorPreset(float v, colorPreset pl, colorPreset p2);

	unsigned int getSkyTexture()
	{
		return skyBoxFBO->tex;
	}

private:
	glm::vec3 skyColorTop, skyColorBottom;

	ScreenSpaceShader * skyBoxShader;
	FrameBufferObject * skyBoxFBO;

	colorPreset presetSunset, highSunPreset;

};