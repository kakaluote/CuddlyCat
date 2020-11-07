#include "Shader.h"

#include <fstream>
#include <sstream>


NS_CC_BEGIN


Shader::Shader():_id(0)
{
	
}

Shader::~Shader()
{
	if (_id != 0) {
		GL_CHECK(glDeleteProgram(_id));
	}
}

bool Shader::initByFile(const std::string& vertexPath, const std::string& fragmentPath)
{
	_vertexPath = vertexPath;
	_fragmentPath = fragmentPath;

	std::string vertexCode;
	std::string fragmentCode;
	if (!readFile(vertexPath, vertexCode) || !readFile(fragmentPath, fragmentCode))
	{
		return false;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	bool somethingWrong = false;

	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (!checkCompileErrors(vertex, "VERTEX")) {
		somethingWrong = true;
	}
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (!checkCompileErrors(fragment, "FRAGMENT")) {
		somethingWrong = true;
	}
	// shader Program
	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);
	if (!checkCompileErrors(_id, "PROGRAM")) {
		somethingWrong = true;
	}
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (somethingWrong) {
		return false;
	}

	return true;
}


bool Shader::readFile(const std::string& filePath, std::string& codeStr)
{
	std::ifstream fs;

	fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fs.open(filePath);
		std::stringstream ss;
		ss << fs.rdbuf();
		fs.close();
		codeStr = ss.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "Error: read shader file "<< filePath << std::endl;
		return false;
	}

	return true;
}


bool Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
	}
	return true;
}

NS_CC_END


