#include <ldv/sdl_video_tools.h>

#include <ldt/log.h>

#include <lm/log.h>
#include <stdexcept>

/*! \file sdl_video_tools.h
    \brief Video functions.

	Miscellaneous video and SDL related functionality.
*/

using namespace ldv;

void ldv::set_cursor_visible(bool v)
{
#ifdef LIBDANSDL2_DEBUG
	lm::log(log_lsdl::get()).debug()<<"set_cursor_visible "<<v<<std::endl;
#endif

	SDL_ShowCursor(v);
}

//!Sets the vsync value.

void ldv::set_vsync(bool v) {

#ifdef LIBDANSDL2_DEBUG
	lm::log(log_lsdl::get()).debug()<<"set_vsync "<<v<<std::endl;
#endif

	SDL_GL_SetSwapInterval(v ? 1 : 0);
}

//!Returns the vsync value.

bool ldv::get_vsync()
{
	return SDL_GL_GetSwapInterval();
}

//!Creates a SDL surface of wxh with the given depth.
SDL_Surface * ldv::new_sdl_surface(
	int _w,
	int _h,
	int _bpp,
	bool _alpha
) {

	auto result=SDL_CreateRGBSurface(
		0, _w, _h, _bpp,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		0xff000000,
		0x00ff0000,
		0x0000ff00,
		_alpha ? 0x000000ff : 0
#else
		0x000000ff,
		0x0000ff00,
		0x00ff0000,
		_alpha ? 0xff000000 : 0
#endif
	);

	if(nullptr==result) {

		throw std::runtime_error(std::string("ldv::new_sdl_surface() failed ")+SDL_GetError());
	}

	SDL_SetSurfaceBlendMode(result, SDL_BLENDMODE_BLEND);

	return result;
}

//!Creates a SDL_Surface with the same format and size as the one given.

SDL_Surface * ldv::new_sdl_surface(SDL_Surface const * porigin)
{
	SDL_Rect box{0,0, porigin->w, porigin->h};
	return new_sdl_surface(porigin, box, 0, 0);
}

//!Creates a SDL_Surface with the same format as the one given, the specified size (2nd) flags (3rd parameter) and background color in integer form (4th parameter, check SDL documentation on this).

SDL_Surface * ldv::new_sdl_surface(SDL_Surface const * porigin, const SDL_Rect& pbox, Uint32 pflags, Uint32 pcolor)
{
	SDL_Surface * origin=const_cast <SDL_Surface *> (porigin);
	SDL_Surface * copy=nullptr;

	copy=SDL_CreateRGBSurface(
		pflags,
		pbox.w, pbox.h, origin->format->BitsPerPixel,
		origin->format->Rmask, origin->format->Gmask, origin->format->Bmask, origin->format->Amask);

	SDL_FillRect(copy, nullptr, pcolor);

	return copy;
}

//!Copies a SDL_Surface.

SDL_Surface * ldv::copy_sdl_surface(SDL_Surface const * porigin)
{
	SDL_Rect pbox{0, 0, porigin->w, porigin->h};
	return copy_sdl_surface(porigin, pbox, porigin->flags, 0);
}

//!Copies part of a SDL surface  with the specified flags.

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

//!Loads image into SDL_Surface.

//This goes well with open_gl. Use this one.

SDL_Surface * ldv::load_image(const std::string& path) {

	SDL_Surface * temp=IMG_Load(path.c_str());
	if (!temp) {
		throw std::runtime_error(std::string("ldv::load_image() : unable to load image ")+path);
	}

	return temp;
}

SDL_Surface * ldv::load_image_from_memory(const unsigned char * _seq, size_t _size) {

	SDL_RWops * rw=SDL_RWFromConstMem(_seq, _size);
	if(nullptr==rw) {
		//TODO: Every exception should belong to the library.
		throw std::runtime_error(std::string("ldv::load_image_from_memory() : unable to load data ")+SDL_GetError());
	}

	SDL_Surface * result=IMG_Load_RW(rw, 0);
	if(nullptr==result) {
		SDL_RWclose(rw);
		throw std::runtime_error(std::string("ldv::load_image_from_memory() : unable to load data ")+IMG_GetError());
	}

	SDL_RWclose(rw);
	return result;
}

SDL_Surface * ldv::load_image_from_memory(const std::vector<unsigned char>& _seq) {

	return load_image_from_memory(_seq.data(), _seq.size());
}

//!Gets the uint32 value of a surface pixel.

//!Does not work with textures and is mostly obsolete.

Uint32 ldv::get_pixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;

	if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	switch(bpp) {
		case 1:			return *p;
		case 2:			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
			else return p[0] | p[1] << 8 | p[2] << 16;
		case 4:			return *(Uint32 *)p;
		default:		return 0;
	}
}

//Directly from the old SDL docs.
void ldv::set_pixel(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

ldv::video_display_mode	ldv::get_display_info(int _display) {

	int displays=SDL_GetNumVideoDisplays();
	if(displays < 0) {

		throw std::runtime_error("ldv::get_display_info was unable to retrieve number of displays");
	}

	if(_display >= displays) {

		throw std::runtime_error("ldv::get_display_info, requested invalid display "+std::to_string(_display));
	}

	SDL_DisplayMode dm;
	if(0!=SDL_GetCurrentDisplayMode(_display, &dm)) {

		throw std::runtime_error("ldv::get_display_info failed");
	}

	return video_display_mode{
		dm.w, dm.h, dm.refresh_rate
	};
}
