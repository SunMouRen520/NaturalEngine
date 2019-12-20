#pragma once
#include "Engine/ScreenSpaceShader.h"
#include "Engine/buffer.h"
#include "Engine/texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include <algorithm>
#include "drawableObjects.h"
#include "CloudsModel.h"

// 这个类将处理FrameBufferObject中云的呈现，并负责uniforms 和 调用 draw 命令
class VolumetricClouds : public drawableObject
{
public:
	VolumetricClouds(int SW, int SH, CloudsModel * model);
	~VolumetricClouds();
	virtual void draw();

	void generateWeatherMap();

	enum cloudsTextureNames {fragColor, bloom, alphaness, cloudDistance};

	unsigned int getCloudsTexture()
	{
		return (model->postProcess ? cloudsPostProcessingFBO->getColorAttachmentTex(0) : getCloudsRawTexture());
	}

	unsigned int getCloudsTexture(int i)
	{
		return cloudsFBO->getColorAttachmentTex(i);
	}

	unsigned int getCloudsRawTexture()
	{
		return cloudsFBO->getColorAttachmentTex(0);
	}

private:
	int SCR_WIDTH, SCR_HEIGHT;

	TextureSet * cloudsFBO;
	FrameBufferObject * cloudsPostProcessingFBO;
	CloudsModel * model;
};