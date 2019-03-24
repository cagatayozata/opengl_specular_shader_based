
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartupKey\"" )
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp" 
#include "Shader.h"
#include "texture.h"
#include "camera.h"

using namespace glm;

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 900

// Perspective Camera Parameters
#define FOVY 70.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE  100.0f

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool upKey = false, downKey = false, rightKey = false, leftKey = false;
bool wKey = false, sKey = false, aKey = false, dKey = false, spaceKey = false;
int  winWidth, winHeight; // current Window width and height
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light0 = {
	vec3(0),
	vec3(0.2),
	vec3(0.5),
	vec3(1.0)
};

mat4 projectionMatrix, viewMatrix, modelMatrix;


GLuint cubeVAO, cubeVBO;

Texture container, specular;
Shader shader_simple, shader_light;
Camera cam(0, 0, 3, 0, 0, 0.1);


void display()
{
	static float angle = 0, offset = 0;
	angle++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
	// Draw Lamp ( Point Light Source )
	shader_simple.on();
	modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, vec3(0.5, 1.0, 0.0));
	modelMatrix = scale(modelMatrix, vec3(0.05f));

	light0.position = vec3( modelMatrix * vec4(light0.position, 1.0));

	shader_simple.uniform("model", modelMatrix);
	shader_simple.uniform("view", cam.LookAt());
	shader_simple.uniform("projection", projectionMatrix);

	shader_simple.uniform("color", light0.diffuse);
	
	
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	shader_simple.off();

	// Draw Cube with lighting.
	shader_light.on();
	mat4 model = mat4(1.0f);
	model = translate(model, vec3(0, 0.2, -2));
	model = rotate(model, radians(angle), vec3(-1, 1, 1));

	
	shader_light.uniform("model", model);
	shader_light.uniform("view", cam.LookAt());
	shader_light.uniform("projection", projectionMatrix);

	shader_light.uniformTexture("material.diffuse", 0, container.id);
	shader_light.uniformTexture("material.specular", 1, specular.id);
	shader_light.uniform("material.shininess", 64.0f);

	shader_light.uniform("light.position", light0.position);
	shader_light.uniform("light.ambient", light0.ambient);
	shader_light.uniform("light.diffuse", light0.diffuse);
	shader_light.uniform("light.specular", light0.specular);

	shader_light.uniform("viewPos", cam.position());
	
	
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	shader_light.off();
	
	glutSwapBuffers();

	offset += 0.001f; 
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeydown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	switch (key) {
	case 'w':
	case 'W': wKey = true; break;
	case 's':
	case 'S': sKey = true; break;
	case 'a':
	case 'A': aKey = true; break;
	case 'd':
	case 'D': dKey = true; break;
	case ' ': spaceKey = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyup(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	switch (key) {
	case 'w':
	case 'W': wKey = false; break;
	case 's':
	case 'S': sKey = false; break;
	case 'a':
	case 'A': aKey = false; break;
	case 'd':
	case 'D': dKey = false; break;
	case ' ': spaceKey = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_upKey, GLUT_KEY_downKey, GLUT_KEY_rightKey, GLUT_KEY_rightKey
//
void onSpecialKeydown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: upKey = true; break;
	case GLUT_KEY_DOWN: downKey = true; break;
	case GLUT_KEY_LEFT: leftKey = true; break;
	case GLUT_KEY_RIGHT: rightKey = true; break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_upKey, GLUT_KEY_downKey, GLUT_KEY_rightKey, GLUT_KEY_rightKey
//
void onSpecialKeyup(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: upKey = false; break;
	case GLUT_KEY_DOWN: downKey = false; break;
	case GLUT_KEY_LEFT: leftKey = false; break;
	case GLUT_KEY_RIGHT: rightKey = false; break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_leftKey_BUTTON , GLUT_rightKey_BUTTON
// states  : GLUT_upKey , GLUT_downKey
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	projectionMatrix = perspective(glm::radians(45.0f), winWidth * 1.0f / winHeight, 0.1f, 10.0f);

}

void onMovedownKey(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function	
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	//glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (upKey) cam.Forward();
	if (downKey) cam.Backward();
	if (leftKey) cam.Left();
	if (rightKey) cam.Right();



	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif


void setupCube() {
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	

	glGenBuffers(2, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	
	glBindVertexArray(0);
}

void Init() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Error: glew init error\n");
		exit(1);
	}

	printf("OpenGL Version : %s\n", glGetString(GL_VERSION));
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shader_simple.init("lamp.vs", "lamp.fs");
	shader_light.init("lighting.vs", "lighting.fs");
	setupCube();
	container.Load("images/container.png");
	specular.Load("images/container_specular.png");
}


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Shader Based OpenGL");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeydown);
	glutSpecialFunc(onSpecialKeydown);

	glutKeyboardUpFunc(onKeyup);
	glutSpecialUpFunc(onSpecialKeyup);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMovedownKey);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}