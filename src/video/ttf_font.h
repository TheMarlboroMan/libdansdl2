#pragma once

#include <SDL2/SDL_ttf.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace ldv
{

//!A ttf font for use with ttf_representations.

//!Actually it is just a wrapper for the SDL_TTF extension functions
//!in an object oriented way. Works nicely with the ttf_manager found under the
//!"tools" project.

class ttf_font
{
	public:

					ttf_font(const std::string&, int);
					ttf_font(const ttf_font&);
					ttf_font& operator=(const ttf_font&);
					~ttf_font();

	//!Get the underlying font data.
	TTF_Font const *		get_font() const {return font;}
	//!Get the font name (actually, the filename).
	const std::string&		get_name() const {return name;}
	//!Get the font size.
	int				get_size() const {return size;}

	private:

	void				load_font();

	std::string			name;
	int				size;
	TTF_Font *			font;
};

}

