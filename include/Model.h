#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

class Model {
private:

    std::vector<Mesh> _meshes;
    std::string _dir;

    /// @brief Loads a model from a given path.
    /// @param path The path to the model file.
    void loadModel(std::string path);

    /// @brief Processes a node in the scene.
    /// @param node The node to process.
    /// @param scene The scene containing the node.
    void processNode(aiNode *node, const aiScene *scene);


    /// @brief Processes a mesh in the scene.
    /// @param mesh The mesh to process.
    /// @param scene The scene containing the mesh.
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    /// @brief Loads material textures from an AI material.
    /// @param mat The AI material to load textures from.
    /// @param type The texture type to load.
    /// @param typeName The name of the texture type.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
public:

    /// @brief Constructs a Model from a given path.
    /// @param path The path to the model file.
    Model(char *path);

    /// @brief Draws the model using the given shader.
    /// @param shader The shader to use for drawing.
    void Draw(Shader &shader);
};

#endif // __MODEL_H__
