#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number) {
	this->Lpos = pos;
	this->Ldirection = dir;
	this->Lambient = ambient;
	this->Ldiffuse = diffuse;
	this->Lspecular = specular;
	this->lightNumber = number;
	this->LightType = lType;

	switch (lType) {
	case DIRECTIONAL:
		break;
	case POINT:
		break;
	case SPOT:
		break;
	default:
		break;
	}

}

Light::~Light() {}

void Light::SetAtt(float constant, float lineal, float quadratic) {
	this->c1 = constant;
	this->c2 = lineal;
	this->c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	this->MinAperture = min;
	this->MaxAperture = max;
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;

	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType) {
	case DIRECTIONAL:
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.direction"), this->Ldirection.x, this->Ldirection.y, this->Ldirection.z);

		glUniform3f(glGetUniformLocation(shad->Program, "dlight.ambient"), this->Lambient.x, this->Lambient.y, this->Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.diffuse"), this->Ldiffuse.x, this->Ldiffuse.y, this->Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.specular"), this->Lspecular.x, this->Lspecular.y, this->Lspecular.z);
		break;
	case POINT:
		variable = "plight[" + std::to_string(lightNumber) + "]";

		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".position").c_str()), this->Lpos.x, this->Lpos.y, this->Lpos.z);

		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".constant").c_str()), this->c1);
		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".linear").c_str()), this->c2);
		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".quadratic").c_str()), this->c3);

		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".ambient").c_str()), this->Lambient.x, this->Lambient.y, this->Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".diffuse").c_str()), this->Ldiffuse.x, this->Ldiffuse.y, this->Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".specular").c_str()), this->Lspecular.x, this->Lspecular.y, this->Lspecular.z);

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";

		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".position").c_str()), this->Lpos.x, this->Lpos.y, this->Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".direction").c_str()), this->Ldirection.x, this->Ldirection.y, this->Ldirection.z);

		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".aperturaMin").c_str()), this->MinAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".aperturaMax").c_str()), this->MaxAperture);

		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".constant").c_str()), this->c1);
		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".linear").c_str()), this->c2);
		glUniform1f(glGetUniformLocation(shad->Program, (char*)(variable + ".quadratic").c_str()), this->c3);

		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".ambient").c_str()), this->Lambient.x, this->Lambient.y, this->Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".diffuse").c_str()), this->Ldiffuse.x, this->Ldiffuse.y, this->Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (char*)(variable + ".specular").c_str()), this->Lspecular.x, this->Lspecular.y, this->Lspecular.z);

		break;
	default:
		break;
	}
}

void Light::Rotate(vec3 rotation) {
	//opcional
	this->Ldirection = this->Ldirection + rotation;
}

void Light::SetDirection(vec3 dir) {
	this->Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	this->Lpos = pos;
}

vec3 Light::GetColor() {
	return this->Lambient + this->Ldiffuse + this->Lspecular;
}