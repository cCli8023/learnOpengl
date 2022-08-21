#pragma once
#include <string>

class openglShader
{
public:
	openglShader();
	bool start();
	unsigned int id();
private:
	std::string _vertexPath = " ";
	std::string _fragPath = " ";
	unsigned int _shaderId;
};

