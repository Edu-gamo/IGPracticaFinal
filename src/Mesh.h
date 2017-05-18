#pragma once
#include <GL\glew.h>
#include <vector>
#include <glm.hpp>
#include <assimp\Importer.hpp>
#include "Shader.h"

using namespace std;
using namespace glm;

struct Vertex{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	Mesh(vector<Vertex> vert, vector<GLuint> ind, vector<Texture> text);
	void Draw(Shader MeshShader, GLint DrawMode);
	void setupMesh();
};

