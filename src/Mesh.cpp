#include "Mesh.h"
#include <cstddef>
#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh() {

	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	glGenBuffers(1, &this->_EBO);

	glBindVertexArray(this->_VAO);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	// bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}


// One shared 1x1 black texture, created on first use.
static unsigned int blackTexture() {
    static unsigned int id = 0;
    if (id == 0) {
        const unsigned char black[4] = {0, 0, 0, 255};
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, black);
    }
    return id;
}

void Mesh::Draw(Shader& shader) {

    // The lighting shader samples material.diffuse from unit 0 and
    // material.specular from unit 1, so bind the first map of each type there.
    // Sampler uniforms are ints (texture unit numbers): setFloat would fail
    // with GL_INVALID_OPERATION and leave every sampler reading unit 0.
    unsigned int diffuseId = 0;
    unsigned int specularId = 0;
    for (const Texture &texture : this->textures) {
        if (texture.type == "texture_diffuse" && diffuseId == 0)
            diffuseId = texture.id;
        else if (texture.type == "texture_specular" && specularId == 0)
            specularId = texture.id;
    }

    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    // Meshes without a map get a 1x1 black texture ("no highlight") instead
    // of whatever the previous draw left bound on that unit. (Binding 0 also
    // samples black, but drivers warn about it: it is an incomplete texture.)
    unsigned int fallback = blackTexture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseId ? diffuseId : fallback);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularId ? specularId : fallback);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(this->_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
