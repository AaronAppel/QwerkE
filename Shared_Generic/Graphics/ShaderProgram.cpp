// include openGL extensions

ShaderProgram::ShaderProgram(const char* vertFileDir, const char* fragFileDir, const char* geoFileDir);
{
	Init(vertFileDir, fragFileDir, geoFileDir);
}

ShaderProgram::ShaderProgram(eShaderTypes type)
{
    Init(type);
}

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	CleanUp();
}

bool ShaderProgram::Init(const char* vertFileDir, const char* fragFileDir, const char* geoFileDir)
{
	// load strings
	if (m_VertString == 0 && vertFileDir != 0)
		m_VertString = LoadCompleteFile(vertFileDir, 0);
	if (m_FragString == 0 && fragFileDir != 0)
		m_FragString = LoadCompleteFile(fragFileDir, 0);
	if (m_GeoString == 0 && geoFileDir != 0)
		m_GeoString = LoadCompleteFile(geoFileDir, 0);
	// otherwise loaded by ShaderFactory

    return BuildShaderProgram();
}
// Returns new shader handle is successful, else 0
GLuint ShaderProgram::ReCompile1Shader(GLenum shaderType, const char* shaderString)
{
	// TODO: Finish writing function. Handle all shader types.
	// May want to recompile one shader and link it to update changes made or a new shader altogether.
	GLuint result = 0;
	result = CompileShader(shaderType, shaderString);

	if (result > 0) // Success
	{
		// re-link shaders
		result = LinkShaders(m_ProgramHandle, m_VertShaderHandle, m_FragShaderHandle);
		if (result > 0)
		{
			// OutputMessage("ReCompileShader(): Success recompiling %s. Changes saved.\n", GetShaderName(shaderString));
			if (shaderType == GL_VERTEX_SHADER) { m_VertShaderHandle = result; }
			if (shaderType == GL_FRAGMENT_SHADER) { m_FragShaderHandle = result; }
			if (shaderType == GL_GEOMETRY_SHADER) { m_GeoShaderHandle = result; }
			m_ProgramHandle = result; // update program handle
		}
	}
	else
	{
		// OutputMessage("ReCompileShader(): Error recompiling %s. No changes were made.\n", GetShaderName(shaderString));
	}

	return result;
}

GLuint ShaderProgram::ReCompileShader()
{
    CleanUp(); // TODO: evaluate which shaders to keep based on changes to strings or handles

    BuildShaderProgram(); // build using new string data
}

void SetShaderStringData(GLenum shaderType, const char* shaderString)
{
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		m_VertString = shaderstring;
		break;
	case GL_FRAGMENT_SHADER:
		m_FragString = shaderstring;
		break;
	case GL_GEOMETRY_SHADER:
		m_GeoString = shaderstring;
		break;
	}
}

void ShaderProgram::CleanUp()
{
	if (m_VertString)
		delete[] m_VertString;
	if (m_FragString)
		delete[] m_FragString;
	if (m_GeoString)
		delete[] m_GeoString;

	if (m_VertShaderHandle) // destroy shaders
	{
		glDetachShader(m_ProgramHandle, m_VertShaderHandle);
		glDeleteShader(m_VertShaderHandle);
	}
	if (m_FragShaderHandle)
	{
		glDetachShader(m_ProgramHandle, m_FragShaderHandle);
		glDeleteShader(m_FragShaderHandle);
	}
	if (m_GeoShaderHandle)
	{
		glDetachShader(m_ProgramHandle, m_GeoShaderHandle);
		glDeleteShader(m_GeoShaderHandle);
	}
	if (m_ProgramHandle)
		glDeleteProgram(m_ProgramHandle);

	m_VertString = 0; // reset strings
	m_FragString = 0;
	m_GeoString = 0;

	m_VertShaderHandle = 0;	// reset handles
	m_FragShaderHandle = 0;
	m_GeoShaderHandle = 0;
	m_ProgramHandle = 0;
}

bool BuildShaderProgram()
{
    // shader strings should have been loaded by Init(), ShaderFactory, or SetStringData()
    // compile shaders
    if (m_VertString) m_VertShaderHandle = CompileShader(GL_VERTEX_SHADER, m_VertString);
    if (m_FragString) m_FragShaderHandle = CompileShader(GL_FRAGMENT_SHADER, m_FragString);
    if (m_GeoString) m_GeoShaderHandle = CompileShader(GL_GEOMETRY_SHADER, m_GeoString);

    if (m_VertShaderHandle == 0 && m_FragShaderHandle == 0 && m_GeoShaderHandle == 0) // null handles
    {
        CleanUp(); // deallocate memory
        assert(0); // DEBUG:
        return false;
    }

    // link shaders
    // TODO: Can you even make a vert or frag only shader? Do I need asserts for that?
    m_ProgramHandle = LinkShaders(m_VertShaderHandle, m_FragShaderHandle, m_GeoShaderHandle);
    if (m_ProgramHandle == 0) { return false; } // check if null program

    return true; // shader built properly
}
// Returns new shader handle is successful, else 0
GLuint ShaderProgram::CompileShader(GLenum shaderType, const char* shaderString)
{
	GLuint shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, &shaderString, NULL);
	glCompileShader(shaderHandle);

	GLint success = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Error compiling shader
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);

		char* shaderTypeString;
		if (shaderType == GL_VERTEX_SHADER)
			shaderTypeString = "GL_VERTEX_SHADER";
		else if (shaderType == GL_FRAGMENT_SHADER)
			shaderTypeString = "GL_FRAGMENT_SHADER";
		else if (shaderType == GL_GEOMETRY_SHADER)
			shaderTypeString = "GL_GEOMETRY_SHADER";

		char* next_token = 0;
		char* ShaderName = strtok_s((char*)shaderString, "\n", &next_token);
		// OutputMessage("\n%s: ShaderProgram: CompileShader() %s compile error-> ", ShaderName, shaderTypeString);
		// OutputMessage(infoLog); // OpenGL message

		// cleanup
		glDeleteShader(shaderHandle);
		shaderHandle = 0;
	}

	return shaderHandle;
}
// Returns new program handle is successful, else 0
GLuint ShaderProgram::LinkShaders(GLuint vert, GLuint frag, GLuint geo)
{
	GLuint result = 0;
	result = glCreateProgram(); // create new program

	// TODO: Handle NULL handles
	glAttachShader(result, vert); // link vertex shader
	glAttachShader(result, frag); // link fragment shader
	glAttachShader(result, geo); // link geometry shader

	glLinkProgram(result);
	// GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if program is not a program object.
	// GL_INVALID_OPERATION is generated if program is the currently active program object and transform feedback mode is active.

	GLint success;
	glGetProgramiv(result, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(result, 512, NULL, infoLog);
		// OutputMessage("\nShaderProgram: LinkShaders() linking error-> ");
		// OutputMessage(infoLog); // OpenGL message

		CleanUp(); // deallocate memory
		return false;
	}

	glDeleteShader(vert); // no longer needed
	glDeleteShader(frag);
	glDeleteShader(geo);

	return result;
}
/* Texture setup */
void ShaderProgram::SetupTextures(GLuint textures[], int numTextures)
{
	for (int i = 0; i < numTextures; i++)
	{
		std::string id = "Texture";
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		id.append(std::to_string(i));
		this->SetUniformInt1(id.c_str(), i);
	}
}
/* Uniform value assignment */
// int
void ShaderProgram::SetUniformInt1(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(m_ProgramHandle, name), value);
}
void ShaderProgram::SetUniformInt2(const char* name, int value1, int value2)
{
	glUniform2i(glGetUniformLocation(m_ProgramHandle, name), value1, value2);
}
void ShaderProgram::SetUniformInt3(const char* name, int value1, int value2, int value3)
{
	glUniform3i(glGetUniformLocation(m_ProgramHandle, name), value1, value2, value3);
}
void ShaderProgram::SetUniformInt4(const char* name, int value1, int value2, int value3, int value4)
{
	glUniform4i(glGetUniformLocation(m_ProgramHandle, name), value1, value2, value3, value4);
}
// float
void ShaderProgram::SetUniformFloat1(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(m_ProgramHandle, name), value);
}
void ShaderProgram::SetUniformFloat2(const char* name, float value1, float value2)
{
	glUniform2f(glGetUniformLocation(m_ProgramHandle, name), value1, value2);
}
void ShaderProgram::SetUniformFloat3(const char* name, float value1, float value2, float value3)
{
	glUniform3f(glGetUniformLocation(m_ProgramHandle, name), value1, value2, value3);
}
void ShaderProgram::SetUniformFloat4(const char* name, float value1, float value2, float value3, float value4)
{
	glUniform4f(glGetUniformLocation(m_ProgramHandle, name), value1, value2, value3, value4);
}
// matrix
void ShaderProgram::SetUniformMat4(const char* name, const GLfloat* matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ProgramHandle, name), 1, GL_FALSE, matrix);
}