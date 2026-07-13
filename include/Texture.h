#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <assimp/scene.h>
#include <string>

class Texture {
public:

	unsigned int id;
	std::string type;
	aiString path;

	Texture();

	/// @brief Load texture onto the GPU and return the texture ID
	/// @param path The file path of the texture to load
	/// @return The OpenGL texture ID
	unsigned int loadTexture(const std::string &path);
};
#endif // __TEXTURE_H__
