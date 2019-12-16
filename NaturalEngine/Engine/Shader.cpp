#include "Shader.h"

Shader::Shader(std::string name) : name(name)
{
	linked = false;
	isCompute = false;
	ID = glCreateProgram();
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

Shader::Shader(std::string name, const char * computeShader): name(name)
{
	linked = false;
	isCompute = false;
	ID = glCreateProgram();

	this->attachShader(BaseShader(computeShader));
	this->linkProgram();
}

Shader * Shader::attachShader(BaseShader s)
{
	if (!isCompute)
	{
		glAttachShader(ID, s.getShad());
		if (s.getName() == "COMPURE")
			isCompute = true;
		this->shaders.push_back(s.getShad());
	}
	else
	{
		std::cout << "ERROR: TRYING TO LINK A COMPUTE SHADER TO COMPUTE PROGRAM" << std::endl;
	}

	return this;
}

void Shader::linkProgram()
{
	glLinkProgram(ID);

	if (checkCompileErrors(ID, "PROGRAM", ""))
	{
		linked = true;
		std::cout << "PROGRAM" << name << " CORRECTLY LINKED" << std::endl;

		while (!shaders.empty())
		{
			glDeleteShader(shaders.back());
			shaders.pop_back();
		}
	}
	else
	{
		std::cout << "ERROR: WHILE LINKED TO " << name << " PROGRAM" << std::endl;
	}

}

void Shader::use()
{
	if (linked)
		glUseProgram(ID);
	else
	{
		std::cout << "PROGRAM NOT LINKED!" << std::endl;
	}
}


// 实现程序统一的功能
//---------------------------------------------------------------
void Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

//---------------------------------------------------------------
void Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

//---------------------------------------------------------------
void Shader::setFloat(const std::string & name, float value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

//---------------------------------------------------------------
void Shader::setVec2(const std::string & name, glm::vec2 vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform2fv(location, 1, glm::value_ptr(vector));
}

//---------------------------------------------------------------
void Shader::setVec3(const std::string & name, glm::vec3 vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(vector));
}

//---------------------------------------------------------------
void Shader::setVec4(const std::string & name, glm::vec4 vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform4fv(location, 1, glm::value_ptr(vector));
}

//---------------------------------------------------------------
void Shader::settMat4(const std::string & name, glm::mat4 matrix) const
{
	unsigned int mat = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(mat, 1, false, glm::value_ptr(matrix));
}

//---------------------------------------------------------------
void Shader::setSampler2D(const std::string & name, unsigned int texture, int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
	this->setInt(name, id);
}

//---------------------------------------------------------------
void Shader::setSampler3D(const std::string & name, unsigned int texture, int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_3D, texture);
	this->setInt(name, id);
}
