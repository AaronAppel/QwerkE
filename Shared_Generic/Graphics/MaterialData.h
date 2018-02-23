#ifndef __MaterialData_H__
#define __MaterialData_H__

class MaterialData
{
public:
	std::string name = "";

	// Texture handles
	GLuint ambientHandle = 0;
	GLuint diffuseHandle = 0;
	GLuint specularHandle = 0;
	float shine = 30.0f;

	// Constructors
	MaterialData() {};
	MaterialData(std::string materialName, GLuint ambient, GLuint diffuse, GLuint specular, float shiny = 1.0f)
	{
		name = materialName;
		ambientHandle = ambient;
		diffuseHandle = diffuse;
		specularHandle = specular;
		shine = shiny;
	};
};

#endif //__MaterialData_H__
