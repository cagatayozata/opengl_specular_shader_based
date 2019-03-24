#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

void ExitOnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
		exit(EXIT_FAILURE);
	}
}

GLuint LoadShader(const char* filename, GLenum shader_type)
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