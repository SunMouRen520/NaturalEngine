#pragma once
#include <glm/glm.hpp>
#include "Engine/Shader.h"
#include "sceneElements.h"
#include "Engine/ScreenSpaceShader.h"
#include "imgui/imgui.h"
#include "SkyBox.h"

#define INT_CEIL(n,d) (int)ceil((float)n/d)

// ��ģ�͸����ռ���Ⱦ�������������Ժ���ɫ����ͬʱ������������ģ���Ʋ��������
class CloudsModel : public drawableObject
{
public:
	friend class VolumetricClouds;

	CloudsModel(sceneElements * scene, SkyBox * sky);
	~CloudsModel();

	// �ٵ�ʵ�֣���Ҫ��������Ϊһ��drawableObject������GUI�࣬�����ǹ̶���
	virtual void draw() {};

	virtual void update();
	virtual void setGui();
	
private:
	void generateWeatherMap();
	void generateModelTextures();
	void initVariables();
	void initShaders();
public:
	Shader * volumetricCloudShader, *weatherShader;
	ScreenSpaceShader * postProcessingShader;
	bool postProcess;
	float coverage, cloudSpeed, crispiness, curliness, density, absorption;
	float earthRadius, sphereInnerRadius, sphereOuterRadius;
	float perlinFrequency;
	bool enableGodRays;
	bool enablePowder;
	

	glm::vec3 cloudColorTop, cloudColorBottom;
	glm::vec3 seed, oldSeed;
	unsigned int perlinTex, worley32, weatherTex;

	
	SkyBox * sky;
	sceneElements * scene;

};