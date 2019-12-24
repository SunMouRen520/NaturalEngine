#ifndef GLAD_H
#define GLAD_H
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Engine/Window.h"
#include "Engine/Shader.h"
#include "Engine/ScreenSpaceShader.h"
#include "Engine/texture.h"

#include <iostream>
#include "Camera.h"

#include "DrawableObjects/drawableObjects.h"
#include "DrawableObjects/sceneElements.h"
#include "DrawableObjects/SkyBox.h"
#include "DrawableObjects/GUI.h"
#include "DrawableObjects/CloudsModel.h"
#include "DrawableObjects/VolumetricClouds.h"
#include "DrawableObjects/Water.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main()
{
	// ���������
	glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	Camera camera(startPosition);

	// ����
	int sucess;
	Window window(sucess, 1600, 900);
	if (!sucess) return -1;

	window.camera = &camera;
	GUI gui(window);
	
	glm::vec3 fogColor(0.5, 0.6, 0.7);
	glm::vec3 lightColor(255, 255, 230);
	lightColor /= 255.0;

	FrameBufferObject SceneFBO(Window::SCR_WIDTH, Window::SCR_HEIGHT);
	glm::vec3 lightPosition;
	glm::vec3 seed(0.0, 0.0, 0.0);
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom),(float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
	glm::vec3 lightDir = glm::vec3(-0.5, 0.5, 1.0);


	// ÿ������������Ҫ��Ⱦ��Щ����
	sceneElements  scene;
	scene.cam = &camera;
	scene.sceneFBO = &SceneFBO;
	scene.lightColor = lightColor;
	scene.lightPos = lightPosition;
	scene.lightDir = lightDir;
	scene.seed = seed;
	scene.fogColor = fogColor;
	scene.projMatrix = proj;

	drawableObject::scene = &scene;

	int gridLength = 120;


	float waterHeight = 120;
	Water water(glm::vec2(0.0, 0.0), gridLength, waterHeight);

	SkyBox skybox;
	CloudsModel cloudsModel(&scene, &skybox);

	VolumetricClouds volumetricClouds(Window::SCR_WIDTH, Window::SCR_HEIGHT, &cloudsModel);
	// �ֱ��ʽϵ͵�֡��������������Ⱦ�ĸ���
	VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel);

	gui.subscribe(&skybox);
	gui.subscribe(&cloudsModel);
	gui.subscribe(&water);

	ScreenSpaceShader PostProcessing("Shader/post_processing.frag");
	ScreenSpaceShader fboVisualizer("Shader/visualizeFbo.frag");
	
	// ��ѭ��
	while (window.continueLoop())
	{
		scene.lightDir = glm::normalize(scene.lightDir);
		scene.lightPos = scene.lightDir * 1e6f + camera.Position;

		// ����
		float frametime = 1 / ImGui::GetIO().Framerate;
		window.processInput(frametime);

		// ����
		gui.update();
		skybox.update();
		cloudsModel.update();

		SceneFBO.bind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �л��߿�ģʽ
		if (scene.wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// ������ã���ͼ����
		glm::mat4 view = scene.cam->GetViewMatrix();
		scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.0f, 10000000.0f);

		

		scene.cam->invertPitch();
		
		ScreenSpaceShader::disableTest();

		reflectionVolumetricClouds.draw();

		Shader& post = PostProcessing.getShader();
		post.use();
		post.setVec2("resolution", glm::vec2(1280, 720));

		post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		PostProcessing.draw();

		ScreenSpaceShader::enableTest();
		
		// ���Ƶ��κ�ˮ��
		scene.sceneFBO->bind();


		// ����������Ⱦ����
		ScreenSpaceShader::disableTest();

		volumetricClouds.draw();
		skybox.draw();
		
		water.draw();

		// �������Ƶ���Ⱦ����κ�Ӧ��һЩ���ڴ���
		// ��Ļ�ϵĻ�
		unbindCurrentFrameBuffer();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		post.use();
		post.setVec2("resolution", glm::vec2(Window::SCR_WIDTH, Window::SCR_HEIGHT));
		post.setVec3("cameraPosition", scene.cam->Position);
		post.setSampler2D("screenTexture", SceneFBO.tex, 0);
		post.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
		post.setSampler2D("depthTex", SceneFBO.depthTex, 2);
		post.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(VolumetricClouds::cloudDistance), 3);

		post.setBool("wireframe", scene.wireframe);

		post.setMat4("VP", scene.projMatrix * view);
		PostProcessing.draw();

		// �Ӿ�����
		Shader& fboVisualizerShader = fboVisualizer.getShader();
		fboVisualizerShader.use();
		fboVisualizerShader.setSampler2D("fboTex", volumetricClouds.getCloudsTexture(), 0);

		gui.draw();

		// glfw: ��������������IO�¼�
		window.swapBuffersAndPollEvents();
	}


	return 0;
}