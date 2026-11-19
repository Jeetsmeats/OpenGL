#include "Model.h"

#include <assimp/material.h>
#include <iostream>

Model::Model(const std::string &path) {
    loadModel(path);
}

void Model::loadModel(const std::string &path) {

    Assimp::Importer importer;
    // GenSmoothNormals: only generates normals when the file has none, so
    // processMesh can always read mNormals without a null check crashing.
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    this->_dir = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    std::cout << "Loaded model " << path << ": " << _meshes.size() << " mesh(es), "
              << _loadedTextures.size() << " texture(s)" << std::endl;
}

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].Draw(shader);
    }
}

void Model::processNode(aiNode *node, const aiScene *scene) {

    // process all node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }

    // recursively process all node's children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {                 // process vertices

        Vertex vertex;
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {                    // process indices
        const aiFace &face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // mMaterialIndex is unsigned, so the old ">= 0" check was always true;
    // compare against the real material count instead.
    if (mesh->mMaterialIndex < scene->mNumMaterials) {                      // process textures
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {                     // load textures if there is any
        aiString str;
        mat->GetTexture(type, i, &str);

        // Meshes usually share textures: reuse one already uploaded instead of
        // decoding and uploading the same file again for every mesh.
        bool cached = false;
        for (const Texture &loaded : _loadedTextures) {
            if (std::strcmp(loaded.path.C_Str(), str.C_Str()) == 0) {
                textures.push_back(loaded);
                cached = true;
                break;
            }
        }
        if (cached)
            continue;

        Texture texture;
        texture.id = texture.loadTexture(_dir + "/" + str.C_Str());
        texture.type = typeName;
        texture.path = str;

        textures.push_back(texture);
        _loadedTextures.push_back(texture);
    }
    return textures;
}
