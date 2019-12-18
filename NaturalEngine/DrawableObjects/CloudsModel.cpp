#include "CloudsModel.h"
#include <glad/glad.h>
#include "Engine/texture.h"

CloudsModel::CloudsModel(sceneElements * scene, SkyBox * sky)
{
	initVariables();
	initShaders();
	generateModelTextures();
}

CloudsModel::~CloudsModel()
{
	delete volumetricCloudShader;
	delete postProcessingShader;
	delete weatherShader;
}

void CloudsModel::update()
{
	seed = scene->seed;
	if (seed != oldSeed)
	{
		generateWeatherMap();
		oldSeed = seed;
	}
}

void CloudsModel::setGui()
{
	ImGui::Begin("Clouds control:");
	ImGui::TextColored(ImVec4(1, 0, 1, 1), "Post Processing");
	ImGui::Checkbox("Post Processing (Gaussian Blur)", &postProcess);
	ImGui::Checkbox("God Rays", &enableGodRays);
	ImGui::TextColored(ImVec4(1, 0, 1, 1), "Clouds rendering");

	ImGui::SliderFloat("Coverage", &coverage, 0.0f, 1.0f);
	ImGui::SliderFloat("Speed", &cloudSpeed, 0.0f, 5.0E3);
	ImGui::SliderFloat("Crispiness", &crispiness, 0.0f, 120.0f);
	ImGui::SliderFloat("Curliness", &curliness, 0.0f, 3.0f);
	ImGui::SliderFloat("Density", &density, 0.0f, 1.0f);
	ImGui::SliderFloat("Light absorption", &absorption, 0.0f, 1.5f);
	ImGui::Checkbox("Enable sugar power effect", &enablePowder);

	ImGui::TextColored(ImVec4(1, 0, 1, 1), "Demo control");
	ImGui::SliderFloat("Sky demo radius", &earthRadius, 10000.0f, 5000000.0f);
	ImGui::SliderFloat("Clouds bottom height", &sphereInnerRadius, 1000.0f, 15000.0f);
	ImGui::SliderFloat("CLouds top height", &sphereOuterRadius, 1000.0f, 40000.0f);

	if (ImGui::SliderFloat("Clouds frequency", &perlinFrequency, 0.0f, 4.0f))
	{
		generateWeatherMap();
	}

	ImGui::TextColored(ImVec4(1, 0, 1, 1), "Clouds colors");
	glm::vec3 * cloudBottomColor = &cloudColorBottom;
	ImGui::ColorEdit3("Cloud color", (float*)cloudBottomColor);		// 编辑云的颜色

	ImGui::End();
}

void CloudsModel::generateWeatherMap()
{
	BindTexture2D(weatherTex, 0);
	weatherShader->use();
	weatherShader->setVec3("seed", scene->seed);
	weatherShader->setFloat("perlinFrequency", perlinFrequency);
	std::cout << "computing weather!" << std::endl;
	glDispatchCompute(INT_CEIL(1027, 8), INT_CEIL(1024, 8), 1);
	std::cout << "weather computed!!" << std::endl;

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void CloudsModel::generateModelTextures()
{
	if (!perlinTex)
	{
		//计算着色器
		Shader comp("perlinWorley");
		comp.attachShader("Shader/perlinworley.comp");
		comp.linkProgram();

		// 制作贴图
		this->perlinTex = GenerateTexture3D(128, 128, 128);
		// 计算
		comp.use();
		comp.setVec3("u_resolution", glm::vec3(128, 128, 128));
		std::cout << "computing perlinworly" << std::endl;
		glActiveTexture(GL_TEXTURE0);
		comp.setInt("outVolTex", 0);
		glDispatchCompute(INT_CEIL(128, 4), INT_CEIL(128, 4), INT_CEIL(128, 4));
		std::cout << "computed!" << std::endl;
		glGenerateMipmap(GL_TEXTURE_3D);
	}

	////////////////////
	if (!worley32)
	{
		// 计算着色器
		Shader worley_git("worleyComp");
		worley_git.attachShader("Shader/worley.comp");
		worley_git.linkProgram();

		// 制作贴图
		this->worley32 = GenerateTexture3D(32, 32, 32);

		// 计算
		worley_git.use();
		worley_git.setVec3("u_resolution", glm::vec3(32, 32, 32));
		std::cout << "computing worley 32!" << std::endl;
		glDispatchCompute(INT_CEIL(32, 4), INT_CEIL(32, 4), INT_CEIL(32, 4));
		std::cout << "computed!" << std::endl;
		glGenerateMipmap(GL_TEXTURE_3D);
	}

	/////////////////
	if (!weatherTex)
	{
		this->weatherTex = GenerateTexture2D(1024, 1024);

		generateWeatherMap();

		seed = scene->seed;
		oldSeed = seed;
	}
}

void CloudsModel::initVariables()
{
	cloudSpeed = 450.0;
	coverage = 0.45;
	crispiness = 40.0;
	curliness = 0.1;
	density = 0.02;
	absorption = 0.35;

	earthRadius = 600000.0;
	sphereInnerRadius = 5000.0;
	sphereOuterRadius = 17000.0;

	perlinFrequency = 0.8;

	enableGodRays = false;
	enablePowder = false;
	postProcess = true;

	seed = glm::vec3(0.0, 0.0, 0.0);
	oldSeed = glm::vec3(0.0, 0.0, 0.0);

	cloudColorTop = (glm::vec3(169.0, 149.0, 149.0) * (1.5f / 255.0f));
	cloudColorBottom = (glm::vec3(65.0, 70.0, 80.0) * (1.5f / 255.0f));

	weatherTex = 0;
	perlinTex = 0;
	worley32 = 0;
}

void CloudsModel::initShaders()
{
	volumetricCloudShader = new Shader("volumetricCloudShader", "Shader/volumetric_clouds.comp");
	postProcessingShader = new ScreenSpaceShader("Shader/clouds_post.frag");

	// 计算着色器
	weatherShader = new Shader("weatherMap");
	weatherShader->attachShader("Shader/weather.comp");
	weatherShader->linkProgram();
}
