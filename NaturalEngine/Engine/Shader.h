#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

#include "Engine/BaseShader.h"

class Shader
{
public:
	Shader(std::string name);
	Shader(std::string name, const char* computeShader);
	Shader * attachShader(BaseShader s);

	void linkProgram();
	virtual ~Shader();

	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, glm::vec2 vector)const;
	void setVec3(const std::string &name, glm::vec3 vector)const;
	void setVec4(const std::string &name, glm::vec4 vector)const;
	void setMat4(const std::string &name, glm::mat4 matrix)const;
	void setSampler2D(const std::string &name, unsigned int texture, int id) const;
	void setSampler3D(const std::string &name, unsigned int texture, int id) const;

private:
	unsigned int ID;

	bool linked, isCompute;
	std::list<unsigned int> shaders;
	std::string name;
};