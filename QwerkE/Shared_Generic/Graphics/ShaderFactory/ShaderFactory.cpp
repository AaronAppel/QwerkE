#include "pch.h"

void ShaderFactory::SetVertName(eShaderTypes shaderType, std::string &shaderString)
{
	switch (shaderType) // shader name comment for easier debugging
	{
	case eShader_Basic2D:
		shaderString.append("// Basic2D.vert");
		break;
	case eShader_Basic3D:
		shaderString.append("// Basic3D.vert");
		break;
	case eShader_BasicLighting:
		shaderString.append("// BasicLighting.vert");
		break;
	case eShader_BasicMaterial:
		shaderString.append("// BasicMaterial.vert");
		break;
	case eShader_LightObject:
		shaderString.append("// LightObject.vert");
		break;
	}
}
void ShaderFactory::SetFragName(eShaderTypes shaderType, std::string &shaderString)
{
	switch (shaderType) // shader name comment for easier debugging
	{
	case eShader_Basic2D:
		shaderString.append("// Basic2D.frag");
		break;
	case eShader_Basic3D:
		shaderString.append("// Basic3D.frag");
		break;
	case eShader_BasicLighting:
		shaderString.append("// BasicLighting.frag");
		break;
	case eShader_BasicMaterial:
		shaderString.append("// BasicMaterial.frag");
		break;
	case eShader_LightObject:
		shaderString.append("// LightObject.frag");
		break;
	}
}
void ShaderFactory::SetVersion(std::string &shaderString)
{
	// TODO:: get and store the openGL version for reference
	int _OpenGLVersion = 330;
	std::string _OpenGLprofile = "core";
	shaderString.append("\n#version " + std::to_string(_OpenGLVersion) + " " + _OpenGLprofile + "\n");
}
void ShaderFactory::OpenMain(std::string &shaderString)
{
	shaderString.append("\nvoid main()\n{");
}
void ShaderFactory::CloseMain(std::string &shaderString)
{
	shaderString.append("\n}");
}
const char* ShaderFactory::CreateVertexShader(eShaderTypes shaderType)
{
	std::string vertString;

	SetFragName(shaderType, vertString);
	SetVersion(vertString);

	switch (shaderType)
	{
	case eShader_Basic2D:
		vertString.append("\nin vec3 a_Position;"); // begin header
		break;
	case eShader_Basic3D:
		vertString.append(
			"\nattribute vec3 a_Position;"
			"\nattribute vec2 a_UV;"
			"\nuniform mat4 u_WorldMat;"
			"\nuniform mat4 u_ViewMat;"
			"\nuniform mat4 u_ProjMat;"
			"\nout vec2 v_UV;");
		break;
	case eShader_BasicLighting:
		break;
	case eShader_BasicMaterial:
		break;
	case eShader_LightObject:
		break;
	default:
		return nullptr;
		break;
	}

	OpenMain(vertString); // end header / begin body

	switch (shaderType)
	{
	case eShader_Basic2D:
		vertString.append("\ngl_Position = vec4(a_Position, 1.0);");
		break;
	case eShader_Basic3D:
		vertString.append("\ngl_Position = u_ProjMat * u_ViewMat * u_WorldMat * vec4(a_Position, 1.0);\nv_UV = a_UV;"); // open body
		break;
	case eShader_BasicLighting:
		break;
	case eShader_BasicMaterial:
		break;
	case eShader_LightObject:
		break;
	default:
		return nullptr;
		break;
	}

	CloseMain(vertString); // end body / end shader

	const char* returnString = _strdup(vertString.c_str());

	return returnString;
}

const char* ShaderFactory::CreateFragmentShader(eShaderTypes shaderType)
{
	std::string fragString;

	SetFragName(shaderType, fragString);
	SetVersion(fragString);

	switch (shaderType) /* Header */
	{
	case eShader_Basic2D:
		fragString.append("\nuniform vec4 u_Color;"); // begin header
		fragString.append("\nout vec4 color;");
		break;
	case eShader_Basic3D:
		fragString.append(
			"\nuniform vec4 u_Color;"
			"\nuniform sampler2D u_Texture1;"
			"\nin vec2 v_UV;");
		//"out vec4 color;\n");
		break;
	case eShader_BasicLighting:
		fragString.append(material_struct);
		fragString.append(material_uniform);
		break;
	case eShader_BasicMaterial:
		break;
	case eShader_LightObject:
		break;
	default:
		return nullptr;
		break;
	}

	OpenMain(fragString); // end header / begin body

	switch (shaderType) /* Body */
	{
	case eShader_Basic2D:
		fragString.append("\ncolor = vec4(1,1,1,1);//u_Color;"); // open body
		break;
	case eShader_Basic3D:
		//fragString.append("color = texture(u_Texture1, v_UV) * u_Color;\n");
		fragString.append("\ngl_FragColor = texture(u_Texture1, v_UV) * u_Color;");
		break;
	case eShader_BasicLighting:
		break;
	case eShader_BasicMaterial:
		break;
	case eShader_LightObject:
		break;
	default:
		return nullptr;
		break;
	}

	CloseMain(fragString); // end body / end shader

	const char* returnString = _strdup(fragString.c_str());

	return returnString;
}