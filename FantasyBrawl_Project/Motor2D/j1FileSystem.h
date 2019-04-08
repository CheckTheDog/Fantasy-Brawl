#ifndef _j1FILESYSTEM_H_
#define _j1FILESYSTEM_H_

#include "j1Module.h"

class j1Filesystem : public j1Module 
{
public:
	j1Filesystem();
	virtual ~j1Filesystem();

	bool Awake(const pugi::xml_node& config);
	bool CleanUp();

	bool addPath(const char* path, const char* mountPoint = NULL);
	bool fileExists(const char* file) const;
	bool isDirectory(const char* name) const;
	bool changeWriteDir(const char* dir) const;

	unsigned int Load(const char* file, char** buffer) const;

	unsigned int Save(const char* file, char* buffer, unsigned int size) const;

private:
	char* basePath = NULL;
	char* writingPath = NULL;
};

#endif // j1_FILESYSTEM_H_