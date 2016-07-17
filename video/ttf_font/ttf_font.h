#ifndef LIBDANSDL2_TTF_FONT_H
#define LIBDANSDL2_TTF_FONT_H

#include <memory>
#include <stdexcept>
#include <string>

#include <SDL2/SDL_ttf.h>

namespace ldv
{

class ttf_font
{
	public:

					ttf_font(const std::string&, int);
					ttf_font(const ttf_font&);
					ttf_font& operator=(const ttf_font&);
					~ttf_font();

	TTF_Font const *		get_font() const {return font;}
	const std::string&		get_name() const {return name;}
	int				get_size() const {return size;}

	private:

	void				load_font();

	std::string			name;
	int				size;
	TTF_Font *			font;
};

}

#endif
