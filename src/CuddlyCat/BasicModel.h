#pragma once

#include "define.h"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

NS_CC_BEGIN



class BasicModel 
{
public:
	BasicModel();
	~BasicModel();

	bool init();
	void draw();
   
private:
	vector<glm::vec3> _vertices;
	vector<unsigned int> _indices;

	unsigned int _VBO;
	unsigned int _VAO;
	unsigned int _EBO;
};





NS_CC_END