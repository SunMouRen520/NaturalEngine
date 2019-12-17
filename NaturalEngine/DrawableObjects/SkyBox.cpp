#include "SkyBox.h"

#include "sceneElements.h"
#include "Engine/Window.h"
#include "imgui/imgui.h"

SkyBox::SkyBox()
{
	skyColorTop = glm::vec3(0.5, 0.7, 0.8) * 1.05f;
	skyColorBottom = glm::vec3(0.9, 0.9, 0.95);

	skyBoxShader = new ScreenSpaceShader("NaturalEngine/Shader/sky.frag");
	skyBoxFBO = new FrameBufferObject(Window::SCR_WIDTH, Window::SCR_HEIGHT);

	SunsetPreset1();
	DefaultPreset();
}

SkyBox::~SkyBox()
{
	delete skyBoxFBO;
	delete skyBoxShader;
}

void SkyBox::draw()
{
	sceneElements * s = drawableObject::scene;
	skyBoxFBO->bind();

	Shader& shader = skyBoxShader->getShader();
	shader.use();

	shader.setVec2("resolution", glm::vec2(Window::SCR_WIDTH, Window::SCR_HEIGHT));
	shader.setMat4("inv_proj", glm::inverse(s->projMatrix));
	shader.setMat4("inv_view", glm::inverse(s->cam->GetViewMatrix()));

	shader.setVec3("lightDirection", glm::normalize(s->lightPos - s->cam->Position));

	shader.setVec3("skyColorTop", skyColorTop);
	shader.setVec3("shyColorBottom", skyColorBottom);

	skyBoxShader->draw();
}

void SkyBox::update()
{
	auto sigmod = [](float v) {return 1 / (1.0 + exp(8.0 - v * 40.0)); };
	mixSkyColorPreset(sigmod(scene->lightDir.y), highSunPreset, presetSunset);
}

void SkyBox::setGui()
{
	ImGui::Begin("Sky control: ");
	ImGui::TextColored(ImVec4(1, 1, 0, 1),"Sky Color Control");
	ImGui::ColorEdit3("Sky Top Color", (float*)&skyColorTop);		// 编辑3个颜色的浮点数
	ImGui::ColorEdit3("Sky Bottom Color", (float*)&skyColorBottom);		// 编辑3个颜色的浮点数
	ImGui::End();
}

colorPreset SkyBox::DefaultPreset()
{
	colorPreset preset;

	preset.cloudColorBottom = (glm::vec3(65., 70., 80.)*(1.5f / 255.f));

	preset.skyColorTop = glm::vec3(0.5, 0.7, 0.8)*1.05f;
	preset.skyColorBottom = glm::vec3(0.9, 0.9, 0.95);

	preset.lightColor = glm::vec3(255, 255, 230) / 255.f;
	preset.fogColor = glm::vec3(0.5, 0.6, 0.7);

	highSunPreset = preset;

	return preset;
}

colorPreset SkyBox::SunsetPreset()
{
	colorPreset preset;

	preset.cloudColorBottom = glm::vec3(89, 96, 109) / 255.f;
	preset.skyColorTop = glm::vec3(177, 174, 119) / 255.f;
	preset.skyColorBottom = glm::vec3(234, 125, 125) / 255.f;

	preset.lightColor = glm::vec3(255, 171, 125) / 255.f;
	preset.fogColor = glm::vec3(85, 97, 120) / 255.f;

	presetSunset = preset;

	return preset;
}

colorPreset SkyBox::SunsetPreset1()
{
	colorPreset preset;

	preset.cloudColorBottom = glm::vec3(97, 98, 120) / 255.f;
	preset.skyColorTop = glm::vec3(133, 158, 214) / 255.f;
	preset.skyColorBottom = glm::vec3(241, 161, 161) / 255.f;

	preset.lightColor = glm::vec3(255, 201, 201) / 255.f;
	preset.fogColor = glm::vec3(128, 153, 179) / 255.f;

	presetSunset = preset;

	return preset;
}

void SkyBox::mixSkyColorPreset(float v, colorPreset p1, colorPreset p2)
{
	float a = std::min(std::max(v, 0.0f), 1.0f);
	float b = 1.0 - a;

	skyColorTop = p1.skyColorTop * a + p2.skyColorTop * b;
	skyColorBottom = p1.skyColorBottom * a + p2.skyColorBottom * b;
	scene->lightColor = p1.lightColor * a + p2.lightColor * b;
	scene->fogColor = p1.fogColor * a + p2.fogColor * b;

}
