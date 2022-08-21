#include "openglShader.h"
#include <io.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

openglShader::openglShader()
{
	std::string configPath("./configFile");
	int ret = _access(configPath.c_str(), 0);
	if (ret != 0) {
		std::cout << configPath << " no exsit " << std::endl;
	}
	//第一行是顶点着色器的路径，第二行是片段着色器的路径
	//后期改用json

	std::ifstream configFile(configPath.c_str(), std::ios::in | std::ios::binary);

	std::getline(configFile, _vertexPath);
	std::getline(configFile, _fragPath);

#if 1
	std::cout << _vertexPath.find('\r') << std::endl;
#endif
	_vertexPath.pop_back();
	_fragPath.pop_back();
	std::cout << _vertexPath << std::endl;
	std::cout << _fragPath << std::endl;
	configFile.close();
}


static std::string getCode(const std::string & src) {
	std::ifstream ifile(src.c_str());
	std::stringstream buf;

	buf << ifile.rdbuf();
	ifile.close();

	return buf.str();
}

static unsigned int compileVerShader(const char* pSrc, GLenum type) {
	unsigned int shaderID;
	shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &pSrc, NULL);
	glCompileShader(shaderID);
	int  success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	
	return shaderID;
}

static unsigned int compileVerShader(const char* pSrc) {
	return compileVerShader(pSrc, GL_VERTEX_SHADER);
}
static unsigned int compileFragShader(const char* pSrc) {
	return compileVerShader(pSrc, GL_FRAGMENT_SHADER);
}

static void deleteShader(unsigned int id) {
	glDeleteShader(id);
}

static unsigned int  shaderProgram(std::vector<unsigned int > shaderIds) {
	std::cout << "shader cnt : " << shaderIds.size() << std::endl;
	int  success;
	char infoLog[512];
	unsigned int shaderProgram;

	shaderProgram = glCreateProgram();
	for (int i = 0; i < shaderIds.size(); i++) {
		glAttachShader(shaderProgram, shaderIds.at(i));
	}
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::GL_LINK_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	return shaderProgram;
}

bool openglShader::start()
{
	std::cout << "opengl shader start " << std::endl;

	int ret = _access(_vertexPath.c_str(), 0);
	if (ret != 0) {
		std::cout << " no exsit  " << _vertexPath << std::endl;
		perror(" :");
		return false;
	}
	ret = _access(_fragPath.c_str(), 0);
	if (ret != 0) {
		std::cout << " no exsit  " << _fragPath << std::endl;
		return false;
	}

	std::string verSrc = getCode(_vertexPath);
	std::string fragSrc = getCode(_fragPath);
	std::cout << verSrc << std::endl;
	std::cout << fragSrc << std::endl;
	const char* pVer = verSrc.c_str();
	const char* pFrag = fragSrc.c_str();
	std::cout << "get code ok" << std::endl;

	unsigned int verShader = compileVerShader(pVer);
	if (verShader == 0) {
		std::cout << "compile ver fail" << std::endl;
		return false;
	}
	unsigned int fragShader = compileFragShader(pFrag);
	if (fragShader == 0) {
		std::cout << "compile frag fail" << std::endl;
		return false;
	}
	_shaderId  = shaderProgram({ verShader, fragShader });

	deleteShader(verShader);
	deleteShader(fragShader);

	return false;
}

unsigned int openglShader::id()
{
	return _shaderId;
}
