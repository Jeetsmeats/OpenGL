#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

class Texture {
public: 	

	unsigned int textureId;

	Texture();

	/// @brief Load texture onto the GPU and return the texture ID
	/// @param path The file path of the texture to load
	/// @return The OpenGL texture ID
	unsigned int loadTexture(const std::string &path); 
};
#endif // __TEXTURE_H__