#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
	GLint programID;
public:
	Shader() : programID(NULL) {};
	GLuint LoadShader(const char *file, GLenum shaderType);
	void  on();
	void  off();
	void  uniform(const char *var, GLfloat val);
	void  uniform(const char *var, GLint val);
	void  uniform(const char *var, const glm::mat4& val);
	void  uniform(const char *var, const glm::mat3& val);
	void  uniform(const char *var, const glm::vec4& val);
	void  uniform(const char *var, const glm::vec3& val);
	void  uniformTexture(const char *texture, GLuint slot, GLuint texID);
	void  init(const char *vertex_shader, const char *frag_shader);
	void  ExitOnGLError(const char* error_message);


};