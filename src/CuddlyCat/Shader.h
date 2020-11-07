#pragma once
#include "define.h"
#include <iostream>
#include "glad/gl.h"
#include "CuddlyCat/glm/glm.hpp"
NS_CC_BEGIN

class Shader
{
public:
	Shader();
	~Shader();
	bool initByFile(const std::string& vertexPath, const std::string& fragmentPath);

	unsigned int id() 
	{
		return _id;
	}

	const std::string& getVertexPath() {
		return _vertexPath;
	}
	const std::string& getFragmentPath() {
		return _fragmentPath;
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void use() const
	{
		glUseProgram(_id);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform1i(loc, (int)value);
		}
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform1i(loc, value);
		}
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform1f(loc, value);
		}
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform2fv(loc, 1, &value[0]);
		}
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform2f(loc, x, y);
		}
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform3fv(loc, 1, &value[0]);
		}
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform3f(loc, x, y, z);
		}
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform4fv(loc, 1, &value[0]);
		}
	}
	void setVec4(const std::string &name, float x, float y, float z, float w) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniform4f(loc, x, y, z, w);
		}
		
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		int loc = glGetUniformLocation(_id, name.c_str());
		if (loc != -1) {
			glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
	}
private:
	bool readFile(const std::string& filePath, std::string& codeStr);
	bool checkCompileErrors(unsigned int shader, std::string type);

private:
	unsigned int _id;
	std::string _vertexPath;
	std::string _fragmentPath;
};

NS_CC_END