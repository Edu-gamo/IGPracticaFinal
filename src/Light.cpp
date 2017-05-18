#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number) {
	this->Lpos = pos;
	this->Ldirection = dir;
	this->Lambient = ambient;
	this->Ldiffuse = diffuse;
	this->Lspecular = specular;
	this->LightType = lType;
	this->lightNumber = number;

	/*switch (lType) {
		case DIRECTIONAL:
			break;
		case POINT:
			break;
		case SPOT:
			break;
		default:
			break;
	}*/

}

Light::~Light(){}


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
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable+".position")), this->Lpos.x, this->Lpos.y, this->Lpos.z);

		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".constant")), this->c1);
		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".linear")), this->c2);
		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".quadratic")), this->c3);

		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".ambient")), this->Lambient.x, this->Lambient.y, this->Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".diffuse")), this->Ldiffuse.x, this->Ldiffuse.y, this->Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".specular")), this->Lspecular.x, this->Lspecular.y, this->Lspecular.z);

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".position")), this->Lpos.x, this->Lpos.y, this->Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".direction")), this->Ldirection.x, this->Ldirection.y, this->Ldirection.z);

		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".aperturaMin")), this->MinAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".aperturaMax")), this->MaxAperture);

		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".constant")), this->c1);
		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".linear")), this->c2);
		glUniform1f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".quadratic")), this->c3);

		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".ambient")), this->Lambient.x, this->Lambient.y, this->Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".diffuse")), this->Ldiffuse.x, this->Ldiffuse.y, this->Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (GLchar*)&(variable + ".specular")), this->Lspecular.x, this->Lspecular.y, this->Lspecular.z);

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