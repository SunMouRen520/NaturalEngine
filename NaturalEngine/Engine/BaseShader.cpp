#include "BaseShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

BaseShader::BaseShader(const char * shaderPath)
{
	path = std::string(shaderPath);
	std::string shaderCode = loadShaderFromFile(shaderPath);
	const char* shaderString = shaderCode.c_str();

	shadtype = getShaderType(shaderPath);
	shad = glCreateShader(shadtype.type);
	glShaderSource(shad, 1, &shaderString, NULL);
	glCompileShader(shad);
	checkCompileErrors(shad, shadtype.name.c_str(), getShaderName(shaderPath));
}

BaseShader::~BaseShader()
{
	//glDeleteShader(shad);
}


bool checkCompileErrors(unsigned int shader, std::string type, std::string shaderName)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR: SHADER" << shaderName << "COMPILATION ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR: PROGRAM_LINK_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

	return success;
}


std::string BaseShader::loadShaderFromFile(const char * shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	//const char * shaderSource = 0;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// 打开文件
		shaderFile.open(shaderPath);


		std::stringstream shaderStream;
		// 将文件的缓冲区内容读入流
		shaderStream << shaderFile.rdbuf();
		// 关闭文件
		shaderFile.close();
		//shaderFile.clear();
		// 将流转换为字符串
		shaderCode = shaderStream.str();
		//shaderSource = shaderCode.c_str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR: SHADER  " << getShaderName(shaderPath) << "  FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	return shaderCode;
}


std::string getShaderName(const char * path)
{
	std::string pathstr = std::string(path);
	const size_t last_slash_idx = pathstr.find_last_of("/");
	if (std::string::npos != last_slash_idx)
	{
		pathstr.erase(0, last_slash_idx + 1);
	}

	return pathstr;
}

ShaderType getShaderType(const char * path)
{
	std::string type = getShaderName(path);
	const size_t last_slash_idx = type.find_last_of(".");
	if (std::string::npos != last_slash_idx)
	{
		type.erase(0, last_slash_idx + 1);
	}

	if (type == "vert")
		return ShaderType(GL_VERTEX_SHADER, "VERTEX");
	if (type == "frag")
		return ShaderType(GL_FRAGMENT_SHADER, "FRAGMENT");
	if (type == "tes")
		return ShaderType(GL_TESS_EVALUATION_SHADER, "TESS_EVALUATION");
	if (type == "tcs")
		return ShaderType(GL_TESS_CONTROL_SHADER, "TESS_CONTROL");
	if (type == "geom")
		return ShaderType(GL_GEOMETRY_SHADER, "GEOMETRY");
	if (type == "comp")
		return ShaderType(GL_COMPUTE_SHADER, "COMPUTE");

}



