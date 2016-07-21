#include "sdl_video_tools.h"
#include <stdexcept>

using namespace ldv;

void ldv::set_cursor_visible(bool v) 
{
	SDL_ShowCursor(v);
}

SDL_Surface * ldv::new_sdl_surface(SDL_Surface const * porigin)
{
	SDL_Rect box{0,0, porigin->w, porigin->h};
	return new_sdl_surface(porigin, box, 0, 0);
}

SDL_Surface * ldv::new_sdl_surface(SDL_Surface const * porigin, const SDL_Rect& pbox, Uint32 pflags, Uint32 pcolor)
{
	SDL_Surface * origin=const_cast <SDL_Surface *> (porigin);
	SDL_Surface * copy=nullptr;

	copy=SDL_CreateRGBSurface(
//		origin->flags|pflags,
		pflags,
		pbox.w, pbox.h, origin->format->BitsPerPixel,
		origin->format->Rmask, origin->format->Gmask, origin->format->Bmask, origin->format->Amask);

	SDL_FillRect(copy, nullptr, pcolor);

	return copy;
}

SDL_Surface * ldv::copy_sdl_surface(SDL_Surface const * porigin)
{
	SDL_Rect pbox{0, 0, porigin->w, porigin->h};
	return copy_sdl_surface(porigin, pbox, porigin->flags, 0);
}

SDL_Surface * ldv::copy_sdl_surface(SDL_Surface const * porigin, const SDL_Rect& pbox, Uint32 pflags, Uint32 pcolor)
{
	SDL_Surface * copy=new_sdl_surface(porigin, pbox, pflags, pcolor);
	SDL_Surface * origin=const_cast <SDL_Surface *> (porigin);
	SDL_Rect * box=const_cast <SDL_Rect *> (&pbox);

	if(copy)
	{
		SDL_BlitSurface(origin, box, copy, nullptr);
	}

	return copy;
}

SDL_Surface * ldv::load_image(const std::string& path, const SDL_Window * window)
{
        SDL_Surface * temp=IMG_Load(path.c_str());
        if (!temp) 
        {
		throw std::runtime_error(std::string("ldv::load_image() : unable to load image on ")+path);
        }

	//This is a hack at best.
	if(!window) 
	{
		SDL_Surface * optimized=SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);
      	        SDL_FreeSurface(temp);
		return optimized;
	}
	else
	{
		SDL_Surface * optimized=SDL_ConvertSurface(temp, SDL_GetWindowSurface(const_cast<SDL_Window *>(window))->format, 0);
      	        SDL_FreeSurface(temp);
		return optimized;
	}
}

SDL_Surface * ldv::load_image(const std::string& path)
{
        SDL_Surface * temp=IMG_Load(path.c_str());
        if (!temp) 
        {
		throw std::runtime_error(std::string("ldv::load_image() : unable to load image ")+path);
        }

	return temp;
}

Uint32 ldv::SDL_GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	
	/* Here p is the address to the pixel we want to retrieve */
	if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	switch(bpp) 
	{
		case 1:
			return *p;
		break;

		case 2:
			return *(Uint16 *)p;
		break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
			else return p[0] | p[1] << 8 | p[2] << 16;
		break;

		case 4:
			return *(Uint32 *)p;
		break;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
		break;
	}
}
