#ifndef SCENELEMENTS_H
#define SCENELEMENTS_H

#include <Camera.h>
#include <glm/glm.hpp>
#include "Engine/buffer.h"
#include <random>

struct sceneElements
{
	glm::vec3 lightPos, lightColor, lightDir, fogColor, seed;
	glm::mat4 projMatrix;
	Camera* cam;
	FrameBufferObject * sceneFBO;
	bool wireframe = false;

};



#endif // !SCENELEMENTS_H
