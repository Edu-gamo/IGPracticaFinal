//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>

#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "Material.h"
#include "Light.h"
#include "Model.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

using namespace glm;
using namespace std;
const GLint WIDTH = 1600, HEIGHT = 600;
Camera myCamera;

int modelNum = 0;

Object cuboFondo;
Model model;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void printVAO(GLuint VAO);
mat4 GenerateModelMatrix(vec3 scale, vec3 rotation, vec3 position);

int main() {
	//initGLFW
//TODO

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
//TODO
	GLFWwindow* window;

	if (!glfwInit()) exit(EXIT_FAILURE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//set GLEW and inicializate
//TODO
	glfwMakeContextCurrent(window);

	glewExperimental = GLU_TRUE;

	if (GLEW_OK != glewInit()) {
		cout << "Error al iniciar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
//TODO
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//set windows and viewport
//TODO
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glViewport(0, 0, screenWidth, screenHeight);

	//fondo
//TODO
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 1.0, 1.0, 1.0);

	glCullFace(GL_FRONT_AND_BACK);
	glFrontFace(GL_CW);

	//cargamos los shader
	Shader myShader = Shader("./src/ShaderVertexPhongTexture.vertexshader", "./src/ShaderFragmentPhongTexture.fragmentshader");
	Shader myShaderGamma = Shader("./src/ShaderVertexPhongTextureGC.vertexshader", "./src/ShaderFragmentPhongTextureGC.fragmentshader");
	Shader shaderLuzes = Shader("./src/SimpleCubeVertex.vertexshader", "./src/SimpleCubeFragment.fragmentshader");

	// Definir el buffer de vertices
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};

	// Definir el EBO
	GLuint IndexBufferObject[]{
		0,1,2,
		3,0,2 };

	// Crear los VBO, VAO y EBO
	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Establecer el objeto
	glBindVertexArray(VAO);
	//Declarar el VBO y el EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Enlazar el buffer con openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBufferObject), IndexBufferObject, GL_STATIC_DRAW);

	//Establecer las propiedades de los vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0); //POSICION ARRAY, Nº VALORES, LARGO TOTAL VERTICES, VALOR QUE EMPIEZA EN EL VECTOR
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)3);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	//Matriz Projection
	float aspectRatio = screenWidth / screenHeight;
	if (screenWidth < screenHeight) aspectRatio = screenHeight / screenWidth;
	mat4 proj;

	//Camara
	vec3 cPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraDir = normalize(cPos - cameraTarget);
	myCamera = Camera(cPos, cameraDir, 0.05f, 60.0f, 3.0f);

	//Cubo fondo
	cuboFondo = Object(vec3(50.0f), vec3(0.0f), vec3(0.0f), Object::cube);

	//Materiales
	Material texMat = Material("./src/textures/difuso.png", "./src/textures/especular.png", 32.0f);
	Material texMatSuelo = Material("./src/textures/suelo.png", "./src/textures/suelo.png", 32.0f);
	Material modelTex1 = Material("./src/models/nanosuit/arm_dif.png", "./src/models/nanosuit/arm_showroom_spec.png", 32.0f);
	Material modelTex2 = Material("./src/models/nanosuit/body_dif.png", "./src/models/nanosuit/body_showroom_spec.png", 32.0f);
	Material modelTex3 = Material("./src/models/nanosuit/glass_dif.png", "./src/models/nanosuit/glass_dif.png", 32.0f);
	Material modelTex4 = Material("./src/models/nanosuit/hand_dif.png", "./src/models/nanosuit/hand_showroom_spec.png", 32.0f);
	Material modelTex5 = Material("./src/models/nanosuit/helmet_diff.png", "./src/models/nanosuit/helmet_showroom_spec.png", 32.0f);
	Material modelTex6 = Material("./src/models/nanosuit/leg_dif.png", "./src/models/nanosuit/leg_showroom_spec.png", 32.0f);

	//Luzes
	Light directional = Light(vec3(0), vec3(1, -1, 0), vec3(0.2f, 0.2f, 0.2f), vec3(0.2f, 0.2f, 0.2f), vec3(0.2f, 0.2f, 0.2f), Light::DIRECTIONAL, 0);

	Object luz1 = Object(vec3(1.0f), vec3(0.0f), vec3(0.0f, 15.0f, 0.0f), Object::cube);
	Light puntual1 = Light(luz1.GetPosition(), vec3(0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), Light::POINT, 0);
	Object luz2 = Object(vec3(1.0f), vec3(0.0f), vec3(10.0f, 15.0f, 10.0f), Object::cube);
	Light puntual2 = Light(luz2.GetPosition(), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), Light::POINT, 1);

	Object luz3 = Object(vec3(1.0f), vec3(0.0f), vec3(10.0f, 15.0f, -10.0f), Object::cube);
	Light focal1 = Light(luz3.GetPosition(), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), Light::SPOT, 0);
	Object luz4 = Object(vec3(1.0f), vec3(0.0f), vec3(-10.0f, 15.0f, -10.0f), Object::cube);
	Light focal2 = Light(luz4.GetPosition(), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), Light::SPOT, 1);

	//Modelo
	model = Model("./src/models/nanosuit/nanosuit.obj");

	//Cubos
	Object cubes[5];
	for (int i = 0; i < 5; i++) {
		//cubes[i] = Object(vec3(50.0f), vec3(0.0f), vec3(0.0f), Object::cube);
		vec3 scal = vec3(rand() % 4 + 5);
		vec3 rot = vec3(0.0f, rand() % 90, 0.0f);
		vec3 pos = vec3(rand() % 50 - 25, -5.0f + scal.y/2, rand() % 50 - 25);
		cubes[i] = Object(scal, rot, pos, Object::cube);
	}

	//Bloquear cursor a la ventana
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Establecer el color de fondo
		glViewport(0, 0, screenWidth, screenHeight);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0, 0.0, 0.0, 1.0);

		//Camara
		myCamera.DoMovement(window);
		proj = perspective(radians(myCamera.GetFOV()), aspectRatio, 0.1f, 100.0f);

		//Viewport 1
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glViewport(0, 0, screenWidth / 2, screenHeight);

		shaderLuzes.USE();
		glUniformMatrix4fv(glGetUniformLocation(shaderLuzes.Program, "proj"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(shaderLuzes.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));
		glUniformMatrix4fv(glGetUniformLocation(shaderLuzes.Program, "matriz"), 1, GL_FALSE, value_ptr(luz1.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(shaderLuzes.Program, "col"), puntual1.GetColor().x, puntual1.GetColor().y, puntual1.GetColor().z);
		luz1.Draw();
		glUniformMatrix4fv(glGetUniformLocation(shaderLuzes.Program, "matriz"), 1, GL_FALSE, value_ptr(luz2.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(shaderLuzes.Program, "col"), puntual2.GetColor().x, puntual2.GetColor().y, puntual2.GetColor().z);
		luz2.Draw();
		glUniformMatrix4fv(glGetUniformLocation(shaderLuzes.Program, "matriz"), 1, GL_FALSE, value_ptr(luz3.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(shaderLuzes.Program, "col"), focal1.GetColor().x, focal1.GetColor().y, focal1.GetColor().z);
		luz3.Draw();
		glUniformMatrix4fv(glGetUniformLocation(shaderLuzes.Program, "matriz"), 1, GL_FALSE, value_ptr(luz4.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(shaderLuzes.Program, "col"), focal2.GetColor().x, focal2.GetColor().y, focal2.GetColor().z);
		luz4.Draw();

		myShader.USE();
		directional.SetLight(&myShader, myCamera.cameraPos);

		puntual1.SetAtt(1.0f, 0.09f, 0.032f);
		puntual1.SetLight(&myShader, myCamera.cameraPos);
		puntual2.SetAtt(1.0f, 0.09f, 0.032f);
		puntual2.SetLight(&myShader, myCamera.cameraPos);

		focal1.SetAtt(1.0f, 0.09f, 0.032f);
		focal1.SetAperture(radians(5.0f), radians(10.0f));
		focal1.SetLight(&myShader, myCamera.cameraPos);
		focal2.SetAtt(1.0f, 0.09f, 0.032f);
		focal2.SetAperture(radians(5.0f), radians(10.0f));
		focal2.SetLight(&myShader, myCamera.cameraPos);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "projection"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));

		//Cubo fondo
		texMat.ActivateTextures();
		texMat.SetMaterial(&myShader);
		texMat.SetShininess(&myShader);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(cuboFondo.GetModelMatrix()));
		cuboFondo.Draw();

		//Plano suelo
		texMatSuelo.ActivateTextures();
		texMatSuelo.SetMaterial(&myShader);
		texMatSuelo.SetShininess(&myShader);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(GenerateModelMatrix(vec3(50.0f), vec3(90.0f, 0.0f, 0.0f), vec3(0.0f, -5.0f, 0.0f))));
		printVAO(VAO);

		//Cubos
		for (int i = 0; i < 5; i++) {
			texMat.ActivateTextures();
			texMat.SetMaterial(&myShader);
			texMat.SetShininess(&myShader);
			glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(cubes[i].GetModelMatrix()));
			cubes[i].Draw();
		}

		//Modelo
		modelTex1.ActivateTextures();
		modelTex1.SetMaterial(&myShader);
		modelTex1.SetShininess(&myShader);
		modelTex2.ActivateTextures();
		modelTex2.SetMaterial(&myShader);
		modelTex2.SetShininess(&myShader);
		modelTex3.ActivateTextures();
		modelTex3.SetMaterial(&myShader);
		modelTex3.SetShininess(&myShader);
		modelTex4.ActivateTextures();
		modelTex4.SetMaterial(&myShader);
		modelTex4.SetShininess(&myShader);
		modelTex5.ActivateTextures();
		modelTex5.SetMaterial(&myShader);
		modelTex5.SetShininess(&myShader);
		modelTex6.ActivateTextures();
		modelTex6.SetMaterial(&myShader);
		modelTex6.SetShininess(&myShader);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(GenerateModelMatrix(vec3(1.0f), vec3(0.0f), vec3(0.0f, -5.0f, 0.0f))));
		model.Draw(myShader, GL_FILL);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		//Viewport 2
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight);

		myShader.USE();
		directional.SetLight(&myShader, myCamera.cameraPos);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "projection"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));

		//Cubo fondo
		texMat.ActivateTextures();
		texMat.SetMaterial(&myShader);
		texMat.SetShininess(&myShader);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(cuboFondo.GetModelMatrix()));
		cuboFondo.Draw();

		//Plano suelo
		texMatSuelo.ActivateTextures();
		texMatSuelo.SetMaterial(&myShader);
		texMatSuelo.SetShininess(&myShader);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(GenerateModelMatrix(vec3(50.0f), vec3(90.0f, 0.0f, 0.0f), vec3(0.0f, -5.0f, 0.0f))));
		printVAO(VAO);

		//Cubos
		for (int i = 0; i < 5; i++) {
			texMat.ActivateTextures();
			texMat.SetMaterial(&myShader);
			texMat.SetShininess(&myShader);
			glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(cubes[i].GetModelMatrix()));
			cubes[i].Draw();
		}

		//Modelo
		modelTex1.ActivateTextures();
		modelTex1.SetMaterial(&myShader);
		modelTex1.SetShininess(&myShader);
		modelTex2.ActivateTextures();
		modelTex2.SetMaterial(&myShader);
		modelTex2.SetShininess(&myShader);
		modelTex3.ActivateTextures();
		modelTex3.SetMaterial(&myShader);
		modelTex3.SetShininess(&myShader);
		modelTex4.ActivateTextures();
		modelTex4.SetMaterial(&myShader);
		modelTex4.SetShininess(&myShader);
		modelTex5.ActivateTextures();
		modelTex5.SetMaterial(&myShader);
		modelTex5.SetShininess(&myShader);
		modelTex6.ActivateTextures();
		modelTex6.SetMaterial(&myShader);
		modelTex6.SetShininess(&myShader);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(GenerateModelMatrix(vec3(1.0f), vec3(0.0f), vec3(0.0f, -5.0f, 0.0f))));
		model.Draw(myShader, GL_FILL);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	
	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void printVAO(GLuint VAO) {
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

mat4 GenerateModelMatrix(vec3 scal, vec3 rotation, vec3 position) {
	mat4 scaleMat;
	scaleMat = scale(scaleMat, scal);

	mat4 rotationMat;
	rotationMat = rotate(rotationMat, radians(rotation[0]), vec3(1.0f, 0.0f, 0.0f));
	rotationMat = rotate(rotationMat, radians(rotation[1]), vec3(0.0f, 1.0f, 0.0f));
	rotationMat = rotate(rotationMat, radians(rotation[2]), vec3(0.0f, 0.0f, 1.0f));

	mat4 traslationMat;
	traslationMat = translate(traslationMat, position);

	return traslationMat * rotationMat * scaleMat;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	myCamera.MouseMove(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	myCamera.MouseScroll(window, yoffset);
}