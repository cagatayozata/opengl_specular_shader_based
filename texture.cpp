#include "texture.h"

int Texture::Load( const char *file , bool flipping, int clamp  ) {
	// open the image file
	data = stbi_load( file, &width, &height, &mode, 0 ) ;
	if ( data == NULL ) {
		return  NULL ;
	}
	if ( flipping ) flip() ;

	// generate an OpenGL Texture Object in the context
	glGenTextures(1, &id ) ;
	glBindTexture( GL_TEXTURE_2D , id ) ;
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, clamp);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, clamp);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
	if ( mode == 3 ) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else if ( mode == 4 ) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} else return NULL ;

    // remove image from main memory. (since it was transferred to Texture object in Display Card's memory)
	stbi_image_free(data);
	data = NULL ;

	return 1; 
}

void  Texture::Unload() {
	if ( id == 0 ) {
		glDeleteTextures(1, &id ) ;
		id=0 ; width=0; height=0; mode = 0 ;
	}
}

void  Texture::flip() {
	int width_in_bytes = width * mode;
    unsigned char *top = NULL;
    unsigned char *bottom = NULL;
    unsigned char temp = 0;
    int half_height = height / 2;

    for (int row = 0; row < half_height; row++) {
       top = data + row * width_in_bytes;
       bottom = data + (height - row - 1) * width_in_bytes;
       for (int col = 0; col < width_in_bytes; col++) {
         temp = *top;
         *top = *bottom;
         *bottom = temp;
         top++;
         bottom++;
      }
    }
}