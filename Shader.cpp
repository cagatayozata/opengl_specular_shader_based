#include "Shader.h"
#include <stdio.h>


GLuint Shader::LoadShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	char* glsl_source;

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);

		if (NULL != (glsl_source = (char*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(char), file_size, file))
			{
				glsl_source[file_size] = '\0';

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, &glsl_source, NULL);
					glCompileShader(shader_id);
					ExitOnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", file_size);

		fclose(file);
	}
	else
	{
		if (NULL != file)
			fclose(file);
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);
	}

	return shader_id;
}

void Shader::ExitOnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
		exit(EXIT_FAILURE);
	}
}

void Shader::init(const char *vertex_shader, const char *frag_shader) {
	int VertexShader, FragmentShader;
	programID = glCreateProgram();
	VertexShader = LoadShader(vertex_shader, GL_VERTEX_SHADER);
	ExitOnGLError("Error: Vertex Shader Compile");
	FragmentShader = LoadShader(frag_shader, GL_FRAGMENT_SHADER);
	ExitOnGLError("Error: Fragment Shader Compile");
	glAttachShader(programID, VertexShader);
	glAttachShader(programID, FragmentShader);
	glLinkProgram(programID);
	ExitOnGLError("Error: Compile or Link Error");

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void Shader::on() {
	if (programID != NULL) {
		glUseProgram(programID);
	}
}

void Shader::off() {
	glUseProgram(0);
}

void Shader::uniform(const char *var, GLfloat val) {
	int loc = glGetUniformLocation(programID, var);
	glUniform1f(loc, val);
	ExitOnGLError("Error: Uniform Float Error");
}

void Shader::uniform(const char *var, GLint val) {
	int loc = glGetUniformLocation(programID, var);
	glUniform1i(loc, val);
	ExitOnGLError("Error: Uniform int Error");
}

void  Shader::uniform(const char *var, const glm::mat4& val) {
	int loc = glGetUniformLocation(programID, var);
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(val));
	ExitOnGLError("Error: Uniform mat4 Error");
}

void  Shader::uniform(const char *var, const glm::mat3& val) {
	int loc = glGetUniformLocation(programID, var);
	glUniformMatrix3fv(loc, 1, GL_FALSE, value_ptr(val));
	ExitOnGLError("Error: Uniform mat3 Error");
}

void  Shader::uniform(const char *var, const glm::vec4& val) {
	int loc = glGetUniformLocation(programID, var);
	glUniform4fv(loc, 1, value_ptr(val));
	ExitOnGLError("Error: Uniform vec4 Error");
}

void  Shader::uniform(const char *var, const glm::vec3& val) {
	int loc = glGetUniformLocation(programID, var);
	glUniform3fv(loc, 1, value_ptr(val));
	ExitOnGLError("Error: Uniform vec3 Error");
}


void  Shader::uniformTexture(const char *texture, GLuint slot, GLuint texID) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texID);
	int loc = glGetUniformLocation(programID, texture);
	glUniform1i(loc, slot);
}