#include "GUI.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Engine/utils.h"

GUI::GUI(Window & w)
{
	//GUI
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(w.getwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::draw()
{
	sceneElements& scene = *this->scene;

	for (drawableObject* obj : subscribers)
	{
		obj->setGui();
	}

	ImGui::Begin("Scene controls: ");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Other controls");
	if (ImGui::DragFloat3("Light Position", &scene.lightDir[0], 0.01, -1.0, 1.0))
	{
		auto saturate = [](float v) {return std::min(std::max(v, 0.0f), 0.8f); };
		scene.lightDir.y = saturate(scene.lightDir.y);
	}
	ImGui::InputFloat3("Camera Position", &(scene.cam->Position[0]), 7);
	ImGui::ColorEdit3("Light Color", (float*)&scene.lightColor);
	ImGui::ColorEdit3("Fog Color", (float*)&scene.fogColor);
	ImGui::SliderFloat("Camera Speed", &scene.cam->MovementSpeed, 0.0, SPEED*3.0);

	ImGui::Checkbox("Wireframe Mode", &scene.wireframe);

	if (ImGui::Button("Generate Seed"))
	{
		scene.seed = genRandomVec3();
	}
	ImGui::SameLine();
	if (ImGui::Button("Use Default Seed"))
		scene.seed = glm::vec3(0.0, 0.0, 0.0);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// 实际图纸
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void GUI::update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

GUI & GUI::subscribe(drawableObject * subscriber)
{
	// TODO: 在此处插入 return 语句
	subscribers.push_back(subscriber);

	return *this;
}
