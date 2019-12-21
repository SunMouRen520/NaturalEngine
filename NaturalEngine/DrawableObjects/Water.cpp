#include "Water.h"
#include "sceneElements.h"
#include "imgui/imgui.h"
#include "Engine/utils.h"
#include "Terrain.h"

const int res = 2;

Water::Water(glm::vec2 position, float scale, float height) : scale(scale),height(height)
{
	shad = new Shader("WaterShader");
	shad->attachShader("Shader/water.frag");
	shad->attachShader("Shader/water.vert");
	shad->linkProgram();

	glm::mat4 identity;
	glm::mat4 scaleMatrix = glm::scale(identity, glm::vec3(scale, scale, scale));
	glm::mat4 transMatrix = glm::translate(identity, glm::vec3(position.x, height, position.y));
	this->modelMatrix = transMatrix * scaleMatrix;

	refractionFBO = new FrameBufferObject(FBOw, FBOh);
	reflectionFBO = new FrameBufferObject(FBOw, FBOh);

	initializePlaneVAO(2, Terrain::tileW, &planeVAO, &planeVBO, &planeEBO);

}

Water::~Water()
{
}

void Water::draw()
{
	// draw water plane
	shad->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	sceneElements * se = drawableObject::scene;

	this->setHeight(height);

	shad->setMat4("modelMatrix", modelMatrix);
	shad->setMat4("gVP", se->projMatrix * se->cam->GetViewMatrix());

	shad->setVec3("u_LightColor", se->lightColor);
	shad->setVec3("u_LightPosition", se->lightPos);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionFBO->tex);
	shad->setInt("reflectionTex", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionFBO->tex);
	shad->setInt("refractionTex", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvMap);
	shad->setInt("waterDUDV", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	shad->setInt("normalMap", 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, refractionFBO->depthTex);
	shad->setInt("depthMap", 4);

	float waveSpeed = 0.25;
	float time = glfwGetTime();

	float moveFactor = waveSpeed * time;

	shad->setFloat("moveFactor", moveFactor);

	shad->setVec3("cameraPosition", se->cam->Position);

	this->drawVertices();
	glDisable(GL_BLEND);
}

void Water::setGui()
{
	ImGui::Begin("Water Control");
	ImGui::SliderFloat("Water Height", &height, 0.0f, 1000.0f);
	ImGui::End();
}

void Water::bindRefractionFBO()
{
	refractionFBO->bind();
}

void Water::bindReflectionFBO()
{
	reflectionFBO->bind();
}

void Water::unbindFBO()
{
	unbindCurrentFrameBuffer(Window::SCR_WIDTH, Window::SCR_HEIGHT);
}

void Water::drawVertices()
{
	glBindVertexArray(planeVAO);
	shad->use();
	glDrawElements(GL_TRIANGLES, (res - 1) * (res - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
