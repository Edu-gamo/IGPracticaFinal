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

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
float textureChange = 0.0f;
float rotationY = 0.0f;
float rotationX = 0.0f;
float traslationY = 0.0f;
float traslationX = 0.0f;
float autoRotation = false;
Camera myCamera;

int modelNum = 0;

Object cube1, cube2;

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
	//Shader myShader = Shader("./src/PhongVertex.vertexshader", "./src/PhongFragment.fragmentshader");
	//Shader myShader = Shader("./src/DirectionalVertex.vertexshader", "./src/DirectionalFragment.fragmentshader");
	//Shader myShader = Shader("./src/PuntualVertex.vertexshader", "./src/PuntualFragment.fragmentshader");
	//Shader myShader = Shader("./src/FocalVertex.vertexshader", "./src/FocalFragment.fragmentshader");
	//Shader myShader = Shader("./src/Phong2Vertex.vertexshader", "./src/Phong2Fragment.fragmentshader");
	Shader myShader = Shader("./src/ShaderVertexPhongTexture.vertexshader", "./src/ShaderFragmentPhongTexture.fragmentshader");
	Shader myShader2 = Shader("./src/SimpleCubeVertex.vertexshader", "./src/SimpleCubeFragment.fragmentshader");

	//Matriz Projection
	float aspectRatio = screenWidth / screenHeight;
	if (screenWidth < screenHeight) aspectRatio = screenHeight / screenWidth;
	mat4 proj;


	//Camara
	vec3 cPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraDir = normalize(cPos - cameraTarget);
	myCamera = Camera(cPos, cameraDir, 0.05f, 60.0f, 3.0f);

	cube1 = Object(vec3(1.0f), vec3(0.0f), vec3(0.0f), Object::cube);
	traslationX = cube1.GetPosition().x;
	traslationY = cube1.GetPosition().y;

	cube2 = Object(vec3(0.1f), vec3(0.0f), vec3(0.0f, 0.0f, 1.0f), Object::cube);

	//Material
	Material texMat = Material("./src/textures/difuso.png", "./src/textures/especular.png", 32.0f);

	//Luz
	Light directional = Light(vec3(0), vec3(1, -1, 0), vec3(0, 1, 0), vec3(0, 1, 0), vec3(0, 1, 0), Light::DIRECTIONAL, 0);
	Light puntual1 = Light(cube2.GetPosition(), vec3(0), vec3(1, 0, 0), vec3(1, 0, 0), vec3(1, 0, 0), Light::POINT, 0);

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

		myShader.USE();

		texMat.ActivateTextures();
		texMat.SetMaterial(&myShader);
		texMat.SetShininess(&myShader);

		proj = perspective(radians(myCamera.GetFOV()), aspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "projection"), 1, GL_FALSE, value_ptr(proj));

		//Camara
		myCamera.DoMovement(window);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));

		cube1.Move(vec3(traslationX, traslationY, 0.0f));
		cube1.Rotate(vec3(rotationX, rotationY, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(cube1.GetModelMatrix()));

		directional.SetLight(&myShader, myCamera.cameraPos);

		puntual1.SetAtt(1.0f, 0.7f, 1.8f);
		puntual1.SetLight(&myShader, myCamera.cameraPos);
		cube1.Draw();

		myShader2.USE();
		glUniformMatrix4fv(glGetUniformLocation(myShader2.Program, "proj"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(myShader2.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));
		glUniformMatrix4fv(glGetUniformLocation(myShader2.Program, "matriz"), 1, GL_FALSE, value_ptr(cube2.GetModelMatrix()));
		cube2.Draw();

		

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	//Trasladar cubo
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) traslationY += 0.1f;
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) traslationY -= 0.1f;
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) traslationX += 0.1f;
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) traslationX -= 0.1f;

	//Rotacion del cubo
	if (key == GLFW_KEY_KP_8 && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationX -= 1.0f;
	if (key == GLFW_KEY_KP_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationX += 1.0f;
	if (key == GLFW_KEY_KP_6 && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationY += 1.0f;
	if (key == GLFW_KEY_KP_4 && (action == GLFW_PRESS || action == GLFW_REPEAT)) rotationY -= 1.0f;
}

void printVAO(GLuint VAO) {
	glBindVertexArray(VAO);
	if (WIDEFRAME) {
		//pintar con lineas
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else {
		//pintar con triangulos
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
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