#ifndef __SHADERFACTORY_H__
#define __SHADERFACTORY_H__

/* Work in progress */

//enum eShaderTypes {};
// NOTE: add \n to beginning of string ex."\nText"
class ShaderFactory
{
public:
	ShaderFactory() {}
	~ShaderFactory() {}

	void SetVertName(eShaderTypes shaderType, std::string &shaderString);
	void SetFragName(eShaderTypes shaderType, std::string &shaderString);
	void SetVersion(std::string &shaderString);

	void OpenMain(std::string &shaderString);
	void CloseMain(std::string &shaderString);

	const char* CreateVertexShader(eShaderTypes shaderName);
	const char* CreateFragmentShader(eShaderTypes shaderName);

private:
	/* Header */
	const char* material_struct =
		"\nstruct Material {"
		"\nvec3 ambient;"
		"\nvec3 diffuse;"
		"\nvec3 specular;"
		"\nfloat shininess;"
		"\n}; ";
	const char* material_uniform = "\nuniform Material u_Material;";

	const char* u_Color_header = "\nuniform vec4 u_Color;";
	const char* u_LightColor_header = "\nuniform vec4 u_LightColor;";
	const char* u_LightPos_header = "\nuniform vec3 u_LightPos;";
	const char* u_CamPos_header = "\nuniform vec3 u_CamPos;";

	/* Body */
};

#endif //__SHADERFACTORY_H__
