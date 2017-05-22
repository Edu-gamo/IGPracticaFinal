#include "Camera.h"
#include <iostream>

Camera::Camera() {

}

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov, GLfloat speed) {
	this->cameraPos = position;
	//this->cameraFront = vec3(0.0f, 0.0f, -1.0f);
	this->cameraFront = normalize(-direction);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = normalize(cross(up, direction));
	this->cameraUp = cross(direction, cameraRight);
	this->Sensitivity = sensitivity;
	this->FOV = fov;
	this->SPEED = speed;
	this->firstMouse = true;
	this->Lastframe = 0;
	this->YAW = -90;
	this->PITCH = 0;
}

void Camera::DoMovement(GLFWwindow * window) {
	float actualTime = glfwGetTime();
	Deltatime = actualTime - Lastframe;
	Lastframe = actualTime;
	GLfloat cameraSpeed = SPEED * Deltatime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraFront * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraFront * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;
	LastMx = xpos;
	LastMy = ypos;

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	YAW += xoffset;
	PITCH += yoffset;

	PITCH = clamp(PITCH, -89.f, 89.f);
	YAW = mod(YAW, 360.f);

	vec3 front;
	front.x = cos(radians(PITCH)) * cos(radians(YAW));
	front.y = sin(radians(PITCH));
	front.z = cos(radians(PITCH)) * sin(radians(YAW));
	cameraFront = normalize(front);
}

void Camera::MouseScroll(GLFWwindow* window, double yScroll) {
	if (FOV >= 1.0f && FOV <= 100.0f) FOV -= yScroll;
	if (FOV <= 1.0f) FOV = 1.0f;
	if (FOV >= 100.0f) FOV = 100.0f;
}

mat4 Camera::LookAt() {

	vec3 f(normalize((cameraPos + cameraFront) - cameraPos));
	vec3 s(normalize(cross(f, cameraUp)));
	vec3 u(cross(s, f));

	mat4 Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = -dot(s, cameraPos);
	Result[3][1] = -dot(u, cameraPos);
	Result[3][2] = dot(f, cameraPos);
	return Result;

}

GLfloat Camera::GetFOV() {
	return FOV;
}
