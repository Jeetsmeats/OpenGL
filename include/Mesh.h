#include <vector>
#include <string>

#include "Shader.h"
#include "Texture.h"
#ifndef __MESH_H__
#define __MESH_H__

class Mesh {
public:

	// mesh properties
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	/// @brief Constructs a new Mesh object
	/// @param vertices The vertices of the mesh
	/// @param indices The indices for indexed drawing
	/// @param textures The textures associated with the mesh
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	
	/// @brief Draws the mesh using the provided shader
	/// @param shader The shader to use for drawing the mesh
	void Draw(Shader& shader);
private:
	unsigned int _VAO, _VBO, _EBO;

	/// @brief Create the mesh to run the shader
	void setupMesh();
};

#endif // __MESH_H__
