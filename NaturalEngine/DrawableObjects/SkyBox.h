#pragma once

#include <glm/glm.hpp>
#include "Engine/ScreenSpaceShader.h"
#include "drawableObjects.h"
#include <glm/gtc/matrix_transform.hpp>

struct colorPreset
{
	glm::vec3 cloudColorBottom, skyColorTop, skyColorBottom, lightColor, fogColor;
};

// 这个类处理天空及其颜色，它在屏幕空间中绘制空间，作为唯一的片段着色器
// 如果有一天它将执行大气光散射，它不使用通常的天空盒，因为它是一个很好的占位符。

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