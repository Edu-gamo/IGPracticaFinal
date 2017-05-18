#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;

class Camera {
public:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	GLfloat Deltatime;
	GLfloat Lastframe;
	GLfloat LastMx;
	GLfloat LastMy;
	GLfloat Sensitivity;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW;
	GLfloat FOV;
	GLfloat SPEED;

	Camera();
	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov, GLfloat speed);
	void DoMovement(GLFWwindow * window);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();

};

