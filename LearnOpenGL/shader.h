#ifndef SHADER_H
#define SHADER_H

#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
public:
	unsigned int ID; //program ID

	Shader(const car* vertexPath, const char* fragmentPath); //program ID

	void use(); //activates the shader

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};
#endif
