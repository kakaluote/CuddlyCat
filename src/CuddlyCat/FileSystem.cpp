#include "FileSystem.h"

NS_CC_BEGIN

static FileSystem *s_SharedFileSystem = nullptr;

FileSystem* FileSystem::getInstance()
{
	if (!s_SharedFileSystem)
	{
		s_SharedFileSystem = new FileSystem;
		s_SharedFileSystem->init();
	}

	return s_SharedFileSystem;
}

void FileSystem::destroyInstance()
{
	CC_SAFE_DELETE(s_SharedFileSystem);
}

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{
}

bool FileSystem::init()
{
	return true;
}

std::string FileSystem::getPath(const std::string& path)
{
	return path;
}


NS_CC_END