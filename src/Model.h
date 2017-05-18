#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL.h>
#include "Mesh.h"

class Model {
public:
	vector<Mesh> meshes;
	string directory;

	Model();
	Model(GLchar* path);
	void Model::Draw(Shader shader, GLint drawMode);
	void Model::loadModel(string path);
	void Model::processNode(aiNode* node, const aiScene* scene);
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint Model::TextureFromFile(const char* path, string directory);
};

