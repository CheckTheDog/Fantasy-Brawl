#include "j1Filesystem.h"
#include "p2Log.h"
#include "p2Defs.h"

#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")
#pragma comment (lib, "PhysFS/libx86/physfs-static.lib")

j1Filesystem::j1Filesystem() : j1Module() {
	name.assign = "filesystem";

	//The base path needs to be initiated by PhysFS for modules to load config
	basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);

	//We include the root file to our paths by default
	addPath(".");
}

j1Filesystem::~j1Filesystem() {
	PHYSFS_deinit();
}

bool j1Filesystem::Awake(const pugi::xml_node& config) {
	LOG("j1Filesystem : Loading filesystem");
	bool ret = true;

	//Adding all paths inside configuration in order
	for (pugi::xml_node path = config.child("path"); path; path.next_sibling("path"))
		addPath(path.child_value());

	//Getting the write directory from SDL
	writingPath = SDL_GetPrefPath("CheckTheDog", "FantasyBrawl");

	if (PHYSFS_setWriteDir(writingPath) == 0) {
		LOG("j1Filesystem : Error while creating the write dir: %s", PHYSFS_getLastError());
		ret = false;
	}
	else {
		LOG("j1Filesystm: Writing directory created - %s", writingPath);
		ret = addPath(writingPath, "save/");
	}

	return ret;
}

bool j1Filesystem::CleanUp() {
	return true;
}

bool j1Filesystem::addPath(const char* path, const char* mountPoint) {
	bool ret = true;

	if (PHYSFS_mount(path, mountPoint, 1) == 0) {
		LOG("j1Filesystem : Error while adding the path %s - %s", path, PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

bool j1Filesystem::fileExists(const char* file) const {
	return PHYSFS_exists(file);
}

bool j1Filesystem::isDirectory(const char* name) const {
	return PHYSFS_isDirectory(name);
}

bool j1Filesystem::changeWriteDir(const char* dir) const {
	bool ret = true;

	if (PHYSFS_setWriteDir(dir) == 0) {
		LOG("j1Filesystem : Error while changing write directory - %s", PHYSFS_getLastError());
		ret = false;
	}
	else
		LOG("j1Filesystem : Write directory changed to %s", dir);

	return ret;
}

unsigned int j1Filesystem::Load(const char * file, char ** buffer) const {
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);
	if (fs_file != NULL) {
		int size = PHYSFS_fileLength(fs_file);
		if (size > 0) {
			*buffer = new char[size];
			if (size != PHYSFS_readBytes(fs_file, *buffer, size)) {
				LOG("mdFilesystem : Error while reading file %s - &s", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = size;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("j1Filesystem : Error while closing file %s - %s", file, PHYSFS_getLastError());
	}
	else
		LOG("j1Filesystem : Error while opening file %s - %s", file, PHYSFS_getLastError());

	return ret;
}

unsigned int j1Filesystem::Save(const char* file, char* buffer, unsigned int size) const {
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);
	if (fs_file != NULL) {
		if (size != PHYSFS_writeBytes(fs_file, (const void*)buffer, size))
			LOG("j1Filesystem : Error while writing to file %s - %s", file, PHYSFS_getLastError());
		else
			ret = size;

		if (PHYSFS_close(fs_file) == 0)
			LOG("j1Filesystem : Error while closing file %s - %s", file, PHYSFS_getLastError());
	}
	else
		LOG("j1Filesystem : Error while opening file %s - %s", file, PHYSFS_getLastError());

	return ret;
}
