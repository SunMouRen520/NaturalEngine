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

// ����ཫ����FrameBufferObject���Ƶĳ��֣�������uniforms �� ���� draw ����
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