#pragma once
#include <glm/glm.hpp>
#include "Engine/Shader.h"
#include "sceneElements.h"
#include "Engine/ScreenSpaceShader.h"
#include "imgui/imgui.h"
#include "SkyBox.h"

#define INT_CEIL(n,d) (int)ceil((float)n/d)

// 云模型负责收集渲染体积云所需的属性和着色器，同时，它还产生了模拟云层的噪音。
class CloudsModel : public drawableObject
{
public:
	CloudsModel(sceneElements * scene, SkyBox * sky);
	~CloudsModel();

	// 假的实现，需要让这个类成为一个drawableObject来订阅GUI类，必须是固定的
	virtual void draw() {};

	virtual void update();
	virtual void setGui();

private:
	void generateWeatherMap();
	void generateModelTextures();
	void initVariables();
	void initShaders();
private:
	Shader * volumetricCloudShader, *weatherShader;
	ScreenSpaceShader * postProcessingShader;

	float coverage, cloudSpeed, crispiness, curliness, density, absorption;
	float earthRadius, sphereInnerRadius, sphereOuterRadius;
	float perlinFrequency;
	bool enableGodRays;
	bool enablePowder;
	bool postProcess;

	glm::vec3 cloudColorTop, cloudColorBottom;
	glm::vec3 seed, oldSeed;
	unsigned int perlinTex, worley32, weatherTex;

	sceneElements * scene;
	SkyBox * sky;

};