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
#include "DrawableObjects/Terrain.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <functional>
#include <stb_image.h>
#include <vector>

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
	Terrain terrain(gridLength);

	float waterHeight = 120;
	Water water(glm::vec2(0.0, 0.0), gridLength, waterHeight);
	terrain.waterPtr = &water;

	SkyBox skybox;
	CloudsModel cloudsModel(&scene, &skybox);

	VolumetricClouds volumetricClouds(Window::SCR_WIDTH, Window::SCR_HEIGHT, &cloudsModel);
	// �ֱ��ʽϵ͵�֡��������������Ⱦ�ĸ���
	VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel);

	gui.subscribe(&terrain);
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

		//glFlush();

		// ����
		float frametime = 1 / ImGui::GetIO().Framerate;
		window.processInput(frametime);

		// ����
		terrain.updateTilesPositions();
		cloudsModel.update();
		gui.update();
		skybox.update();		

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

		// ������ˮ�з��仺������
		water.bindReflectionFBO();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene.cam->invertPitch();
		scene.cam->Position.y -= 2 * (scene.cam->Position.y - water.getHeight());

		terrain.up = 1.0;
		terrain.draw();

		FrameBufferObject const& reflFBO = water.getReFlectionFBO();
		
		ScreenSpaceShader::disableTest();

		reflectionVolumetricClouds.draw();

		// ���°󶨷��仺����
		water.bindReflectionFBO();

		Shader& post = PostProcessing.getShader();
		post.use();
		post.setVec2("resolution", glm::vec2(1280, 720));
		post.setSampler2D("screenTexture", reflFBO.tex, 0);
		post.setSampler2D("depthTex", reflFBO.depthTex, 2);
		post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		PostProcessing.draw();
				
		ScreenSpaceShader::enableTest();

		scene.cam->invertPitch();
		scene.cam->Position.y += 2 * abs(scene.cam->Position.y - water.getHeight());

		// ������ˮ�����仺������
		water.bindRefractionFBO();
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		terrain.up = -1.0;
		terrain.draw();
		
		// ���Ƶ��κ�ˮ��
		scene.sceneFBO->bind();
		terrain.draw();
		water.draw();

		// ����������Ⱦ����
		ScreenSpaceShader::disableTest();

		volumetricClouds.draw();
		skybox.draw();
				

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

}


int main010()
{

	// camera and window setup
	// ����ʹ�������
	glm::vec3 startPosition(0.0f, 800.0f, 0.0f);
	Camera camera(startPosition);

	int success;
	Window window(success, 1600, 900);
	if (!success) return -1;

	//Window class needs camera address to perform input handling
	// ��������Ҫ�������ַ��ִ�����봦��
	window.camera = &camera;

	GUI gui(window);

	glm::vec3 fogColor(0.5, 0.6, 0.7);
	glm::vec3 lightColor(255, 255, 230);
	lightColor /= 255.0;

	FrameBufferObject SceneFBO(Window::SCR_WIDTH, Window::SCR_HEIGHT);
	glm::vec3 lightPosition, seed;
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
	glm::vec3 lightDir = glm::vec3(-.5, 0.5, 1.0);

	//Every scene object need these informations to be rendered
	// ÿ������������Ҫ��Ⱦ��Щ��Ϣ
	sceneElements scene;
	scene.cam = &camera;
	scene.sceneFBO = &SceneFBO;
	scene.lightPos = lightPosition;
	scene.lightColor = lightColor;
	scene.fogColor = fogColor;
	scene.seed = seed;
	scene.projMatrix = proj;

	scene.lightDir = lightDir;

	drawableObject::scene = &scene;

	int gridLength = 120;
	Terrain terrain(gridLength);

	float waterHeight = 120.;
	Water water(glm::vec2(0.0, 0.0), gridLength, waterHeight);
	terrain.waterPtr = &water;

	SkyBox skybox;
	CloudsModel cloudsModel(&scene, &skybox);

	VolumetricClouds volumetricClouds(Window::SCR_WIDTH, Window::SCR_HEIGHT, &cloudsModel);
	// �ֱ��ʽϵ͵�֡��������������Ⱦ�����
	VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel); // (expected) lower resolution framebuffers, so the rendering will be faster

	gui.subscribe(&terrain)
		.subscribe(&skybox)
		.subscribe(&cloudsModel)
		.subscribe(&water);

	ScreenSpaceShader PostProcessing("Shader/post_processing.frag");
	ScreenSpaceShader fboVisualizer("Shader/visualizeFbo.frag");

	while (window.continueLoop())
	{
		scene.lightDir = glm::normalize(scene.lightDir);
		scene.lightPos = scene.lightDir*1e6f + camera.Position;


		// input
		// ����
		float frametime = 1 / ImGui::GetIO().Framerate;
		window.processInput(frametime);

		//update tiles position to make the world infinite, clouds weather map and sky colors
		// ���´�ש��λ�ã�ʹ�������ޣ�������ͼ����յ���ɫ
		terrain.updateTilesPositions();
		cloudsModel.update();
		gui.update();
		skybox.update();

		SceneFBO.bind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// toggle/untoggle wireframe mode
		// �л��߿�ģʽ
		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Camera (View Matrix) setting
		// ���(��ͼ����)����
		glm::mat4 view = scene.cam->GetViewMatrix();
		scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);


		//draw to water reflection buffer object
		// ������ˮ�з��仺������
		water.bindReflectionFBO();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.cam->invertPitch();
		scene.cam->Position.y -= 2 * (scene.cam->Position.y - water.getHeight());

		terrain.up = 1.0;
		terrain.draw();
		FrameBufferObject const& reflFBO = water.getReFlectionFBO();

		ScreenSpaceShader::disableTest();

		reflectionVolumetricClouds.draw();

		// ���°󶨷��仺����;reflVolumetricClouds�⿪��
		water.bindReflectionFBO(); //rebind refl buffer; reflVolumetricClouds unbound it


		Shader& post = PostProcessing.getShader();
		post.use();
		post.setVec2("resolution", glm::vec2(1280, 720));
		post.setSampler2D("screenTexture", reflFBO.tex, 0);
		post.setSampler2D("depthTex", reflFBO.depthTex, 2);
		post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		PostProcessing.draw();

		ScreenSpaceShader::enableTest();

		scene.cam->invertPitch();
		scene.cam->Position.y += 2 * abs(scene.cam->Position.y - water.getHeight());

		//draw to water refraction buffer object
		// ������ˮ�����仺������
		water.bindRefractionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		terrain.up = -1.0;
		terrain.draw();

		// draw terrain and water
		// ���Ƶ��κ�ˮ��
		scene.sceneFBO->bind();
		terrain.draw();
		water.draw();

		//disable test for quad rendering
		// ����������Ⱦ����
		ScreenSpaceShader::disableTest();

		volumetricClouds.draw();
		skybox.draw();

		// blend volumetric clouds rendering with terrain and apply some post process
		// ���������Ⱦ����κ�Ӧ��һЩ���ڴ���
		// on-screen drawing
		// ��Ļ�ϵĻ�
		unbindCurrentFrameBuffer();
		//Shader& post = PostProcessing.getShader();
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



		// Texture visualizer
		// �����Ӿ�
		Shader& fboVisualizerShader = fboVisualizer.getShader();
		fboVisualizerShader.use();
		fboVisualizerShader.setSampler2D("fboTex", volumetricClouds.getCloudsTexture(), 0);
		//fboVisualizer.draw(); //for debugging purposes

		gui.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// glfw: ��������������ѯIO�¼�(����/�ͷż����ƶ�����)
		window.swapBuffersAndPollEvents();
	}
}
