#include "textura.h"

using namespace DLibV;

Textura::Textura(const Superficie& s):
	indice(0), w(0), h(0)
{
	cargar(s.acc_superficie());
}

Textura::~Textura()
{
	glDeleteTextures(1, &indice);
}

//TODO: Either invalidate copies or make it generate a new texture.
Textura& Textura::operator=(const Textura& t)
{
	//TODO: Check if texture exists before doing any of this...*/
	/*
	This should get the image data. Later we could put it somewhere.
	
		void glGetTexImage (GLenum target,
                    GLint       level,
                    GLenum      format, // GL will convert to this format
                    GLenum      type,   // Using this data type per-pixel
                    GLvoid *    img);
                    
	*/
	
	w=t.w;
	h=t.h;
	indice=t.indice;
	return *this;
}

void Textura::reemplazar(const Superficie& s)
{
	cargar(s.acc_superficie());
}

void Textura::cargar(const SDL_Surface * superficie)
{
	w=superficie->w;
	h=superficie->h;

	//Si el índice no existe se pide una nueva textura a OpenGL. En caso contrario
	//se trabaja sobre la original.
	if(!indice)
	{
		glGenTextures(1, &indice);
	}
	
	glBindTexture(GL_TEXTURE_2D, indice);

	int mode=GL_RGB;
 	if(superficie->format->BytesPerPixel==4) mode=GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, superficie->pixels);
	
	//TODO: This should go somewhere else.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
