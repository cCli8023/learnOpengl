#pragma once
#include <string>

class openglShader
{
public:
	openglShader();
	bool start();
	unsigned int id();
private:
	std::string _vertexPath =	"E:/lr/ubuntu/share/lr/learnOpengl/proj/openglStudy/shader/shader.vert";
	std::string _fragPath =		"E:/lr/ubuntu/share/lr/learnOpengl/proj/openglStudy/shader/shader.frag";
	unsigned int _shaderId;
};

