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

using namespace glm;
using namespace std;
const GLint WIDTH = 1600, HEIGHT = 600;
bool WIDEFRAME = false;
float textureChange = 0.0f;
float rotationY = 0.0f;
float rotationX = 0.0f;
float traslationY = 0.0f;
float traslationX = 0.0f;
float autoRotation = false;
Camera myCamera;

int modelNum = 0;

Object cube1;
Model car;

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
	Shader myShader2 = Shader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	Shader myShader3 = Shader("./src/3DVertex.vertexshader", "./src/3DFragment.fragmentshader");

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


	//TEXTURAS
	// Crear la textura
	GLuint texture;

	//Reservar memoria en openGL
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//parametros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	int width, height;
	unsigned char* image = SOIL_load_image("./src/textures/suelo.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//libera la imagen
	SOIL_free_image_data(image);
	
	//libera el puntero de la textura
	glBindTexture(GL_TEXTURE_2D, 0);

	//liberar el buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//liberar el buffer de vertices
	glBindVertexArray(0);


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
	cube1 = Object(vec3(50.0f), vec3(0.0f), vec3(0.0f), Object::cube);

	//Material
	Material texMat = Material("./src/textures/difuso.png", "./src/textures/especular.png", 32.0f);

	//Luz
	Light directional = Light(vec3(0), vec3(1, -1, 0), vec3(0, 1, 0), vec3(0, 1, 0), vec3(0, 1, 0), Light::DIRECTIONAL, 0);

	//Modelo
	//car = Model("./src/models/Aventador/Avent.obj");
	//car = Model("./src/models/nanosuit/nanosuit.obj");

	//Cubos
	Object cubes[5];
	for (int i = 0; i < 5; i++) {
		//cubes[i] = Object(vec3(50.0f), vec3(0.0f), vec3(0.0f), Object::cube);
		vec3 scal = vec3(rand() % 4 + 5);
		vec3 rot = vec3(0.0f, rand() % 90, 0.0f);
		vec3 pos = vec3(rand() % 50 - 25, 0.0f, rand() % 50 - 25);
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

		myShader.USE();

		texMat.ActivateTextures();
		texMat.SetMaterial(&myShader);
		texMat.SetShininess(&myShader);

		proj = perspective(radians(myCamera.GetFOV()), aspectRatio, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "projection"), 1, GL_FALSE, value_ptr(proj));

		//Camara
		myCamera.DoMovement(window);
		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));

		glUniformMatrix4fv(glGetUniformLocation(myShader.Program, "model"), 1, GL_FALSE, value_ptr(cube1.GetModelMatrix()));

		directional.SetLight(&myShader, myCamera.cameraPos);

		//Plano suelo
		myShader2.USE();
		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(myShader2.Program, "Texture1"), 1);
		glUniformMatrix4fv(glGetUniformLocation(myShader2.Program, "projection"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(myShader2.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));
		glUniformMatrix4fv(glGetUniformLocation(myShader2.Program, "matriz"), 1, GL_FALSE, value_ptr(GenerateModelMatrix(vec3(50.0f), vec3(90.0f, 0.0f, 0.0f), vec3(0.0f, -5.0f, 0.0f))));


		//Modelo
		myShader3.USE();
		glUniformMatrix4fv(glGetUniformLocation(myShader3.Program, "proj"), 1, GL_FALSE, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(myShader3.Program, "view"), 1, GL_FALSE, value_ptr(myCamera.LookAt()));
		//glUniformMatrix4fv(glGetUniformLocation(myShader3.Program, "matriz"), 1, GL_FALSE, value_ptr(GenerateModelMatrix(vec3(1.0f), vec3(0.0f), vec3(0.0f, -5.0f, 0.0f))));
		
		//Viewport 1
		glViewport(0, 0, screenWidth / 2, screenHeight);

		//Cubo fondo
		myShader.USE();
		cube1.Draw();

		//Plano suelo
		myShader2.USE();
		printVAO(VAO);

		//Cubos
		for (int i = 0; i < 5; i++) {
			myShader3.USE();
			glUniformMatrix4fv(glGetUniformLocation(myShader3.Program, "matriz"), 1, GL_FALSE, value_ptr(cubes[i].GetModelMatrix()));
			cubes[i].Draw();
		}

		//Viewport 2
		glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight);

		//Cubo fondo
		myShader.USE();
		cube1.Draw();

		//Plano suelo
		myShader2.USE();
		printVAO(VAO);

		//Modelo
		/*myShader3.USE();
		car.Draw(myShader3, GL_FILL);*/

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