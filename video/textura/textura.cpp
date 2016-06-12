#include "textura.h"

using namespace DLibV;

Textura::Textura(const Superficie& s):
	indice(0), w(0), h(0)
{
	cargar(s.acc_superficie());
}

Textura::Textura(const Textura& t):
	indice(t.indice), w(t.w), h(t.h)
{

}

Textura::~Textura()
{
	//TODO: Cosas terribles ocurrirán si alguna vez copiamos una textura.
	//Deberíamos duplicar la textura en el constructor de copia y en el
	//operador de asignación.

	glDeleteTextures(1, &indice);
}
	
Textura& Textura::operator=(const Textura& t)
{
	w=t.w;
	h=t.h;
	indice=t.indice;
	return *this;
}

void Textura::cargar(const SDL_Surface * superficie)
{
	w=superficie->w;
	h=superficie->h;

	glGenTextures(1, &indice);
	glBindTexture(GL_TEXTURE_2D, indice);

	int mode=GL_RGB;
 	if(superficie->format->BytesPerPixel==4) mode=GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, superficie->w, superficie->h, 0, mode, GL_UNSIGNED_BYTE, superficie->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
