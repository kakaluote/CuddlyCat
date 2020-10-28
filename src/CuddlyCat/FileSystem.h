#pragma once
#include "define.h"
#include "FileSystem.h"
#include <string>


NS_CC_BEGIN

class FileSystem
{
public:
	static FileSystem* getInstance();
	static void destroyInstance();

	FileSystem();
	~FileSystem();
	bool init();

	std::string getPath(const std::string& path);
private:
};

NS_CC_END