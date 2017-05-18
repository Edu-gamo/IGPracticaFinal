#include "material.h"

Material::Material(char *DiffPath, char*SpectPath, float Shini) {
	this->Shininess = Shini;

	//Reservar memoria
	glGenTextures(1, &TextDiff);

	//Enlazar textura reservada en memoria
	glBindTexture(GL_TEXTURE_2D, TextDiff);

	//Propiedades (Wrapping y filtrado)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Cargar imagen
	int width, height;
	unsigned char* image = SOIL_load_image(DiffPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//Liberar imagen
	SOIL_free_image_data(image);

	//Reservar memoria
	glGenTextures(1, &TextSpec);

	//Enlazar textura reservada en memoria
	glBindTexture(GL_TEXTURE_2D, TextSpec);

	//Propiedades (Wrapping y filtrado)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Cargar imagen
	image = SOIL_load_image(SpectPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//Liberar imagen
	SOIL_free_image_data(image);

	//Liberar puntero de textura
	glBindTexture(GL_TEXTURE_2D, 0);

}


Material::~Material() {
}

void Material::SetMaterial(Shader *shad) {
	glUniform1i(glGetUniformLocation(shad->Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shad->Program, "material.specular"), 1);
}

void Material::SetShininess(Shader *shad) {
	glUniform1f(glGetUniformLocation(shad->Program, "material.shininess"), this->Shininess);
}

void Material::ActivateTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextDiff);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextSpec);
}
