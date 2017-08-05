#include "texture.h"
#include <stdexcept>
#include <vector>
#include <cstdint>

using namespace ldv;

texture::texture(const surface& s):
	index(0), mode(GL_RGB), w(0), h(0)
{
	load(s.get_surface());
}

texture::~texture()
{
	glDeleteTextures(1, &index);
}

void texture::replace(const surface& s)
{
	load(s.get_surface());
}

void texture::load(const SDL_Surface * surface)
{
	w=surface->w;
	h=surface->h;

	//Si el índice no existe se pide una nueva textura a OpenGL. En caso contrario
	//se trabaja sobre la original.
	if(!index)
	{
		glGenTextures(1, &index);
	}
	
	glBindTexture(GL_TEXTURE_2D, index);

/*	switch(surface->format->BytesPerPixel)
	{
		case 1:
			mode=GL_ALPHA;
		break;
		case 3: // no alpha channel
			if (surface->format->Rmask == 0x000000ff)
			    mode=GL_RGB;
			else
			    mode=GL_BGR;
		break;
		case 4: // contains an alpha channel
			if (surface->format->Rmask == 0x000000ff)
			    mode=GL_RGBA;
			else
			    mode=GL_BGRA;
		break;
		default:
			throw std::runtime_error("Image is not true color in texture::load");
		break;
	    }
*/

//std::cout<<"Format -> r:"<<std::hex<<surface->format->Rmask<<" g:"<<surface->format->Gmask<<" b:"<<surface->format->Bmask<<" a:"<<surface->format->Amask<<std::endl;

 	if(surface->format->BytesPerPixel==4) 
	{
		if(surface->format->Rmask==0x000000ff)
		{
			mode=GL_RGBA;
		}
		else
		{
			//This makes it all go boom... However, if we don't do it, colours in fonts become disarranged.
//			mode=GL_BGRA;
			mode=GL_RGBA;
		}
	}
	else 
	{
		mode=GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
}
