#ifndef __test_TEXTURE__
#define __test_TEXTURE__

#include "stb_image.h"
#include <gl/freeglut.h>

class  Texture {
public:
	GLuint id ;
	int width ;
	int height ;
	int mode ;   // 3: RGB, 4: RGBA
	unsigned char *data ; 
	Texture() { id=0; width=0; height=0; data = NULL; mode = 0; }
	int  Load( const char *file , bool flipping = true, int clamp = GL_REPEAT  ) ;
	void  Unload() ; 
	void flip() ;
} ;

#endif