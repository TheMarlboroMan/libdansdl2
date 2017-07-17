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

/*
texture::texture(const texture& t)
	:index(0), mode(t.mode), w(t.w), h(t.h)
{

	//TODO: This isn't working.
	glBindTexture(GL_TEXTURE_2D, t.index);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, mode, 0, 0, w, h, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	std::vector<uint8_t> raw_img;

	//TODO: Test this with other sizes.
	int bpp=mode==GL_RGBA ? 4 : 3;
	raw_img.reserve((w*h)*bpp);

	glGetTexImage(GL_TEXTURE_2D, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());
	glGenTextures(1, &index);
	glBindTexture(GL_TEXTURE_2D, index);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());
}

texture& texture::operator=(const texture& t)
{
	//TODO: This isn't working.
	mode=t.mode;
	w=t.w;
	h=t.h;

	glBindTexture(GL_TEXTURE_2D, t.index);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, mode, 0, 0, w, h, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	std::vector<uint8_t> raw_img;

	//TODO: Test this with other sizes.
	int bpp=mode==GL_RGBA ? 4 : 3;
	raw_img.reserve((w*h)*bpp);

	glGetTexImage(GL_TEXTURE_2D, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());
	glGenTextures(1, &index);
	glBindTexture(GL_TEXTURE_2D, index);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());

	return *this;
}
*/

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
