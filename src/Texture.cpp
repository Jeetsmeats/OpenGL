#include <iostream>

// third part libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

// defined libraries
#include "Texture.h"

Texture::Texture() : id(0) {}

unsigned int Texture::loadTexture(const std::string &path) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

	// texture settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture onto gpu
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;        // e.g. greyscale specular / roughness maps
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB;
        // GL assumes each pixel row starts on a 4-byte boundary. RGB/RED rows
        // of odd widths don't, which shears the texture diagonally.
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (format == GL_RED) {
            // sample as (r, r, r, 1) so vec3(texture(...)) stays grey, not red
            GLint swizzle[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << path << " - " << stbi_failure_reason() << std::endl;
        glDeleteTextures(1, &this->id);
        return 0;
    }

    return this->id;
}
