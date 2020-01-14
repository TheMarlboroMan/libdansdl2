#include <ldv/ttf_font.h>

using namespace ldv;

//!Construct a font from a filename with the given size.

//!This may be glitchy in terms of exception handling, as load_font may
//!throw and there is no exception control here.

//TODO: load_font may throw!!!.

ttf_font::ttf_font(const std::string& f, int t)
	:name(f), size(t), font(nullptr)
{
	load_font();
}

//!Copy constructor.

//!Does not copy the ttf data, but load the font again.
//!It may be glitchy in terms of exception handling.

//TODO: This can throw!!.

ttf_font::ttf_font(const ttf_font& f)
	:name(f.name), size(f.size), font(nullptr)
{
	load_font();
}

//!Assignment operator.

//!Loads the font instead of copying the font data. 
//!May throw a std::runtime_error.

ttf_font& ttf_font::operator=(const ttf_font& f)
{
	name=f.name;
	size=f.size;
	font=nullptr;
	load_font();
	return *this;
}

//!Class destructor.

//!Closes the font.

ttf_font::~ttf_font()
{
	if(nullptr!=font) {
		TTF_CloseFont(font);
	}
}

//!Loads the font.

//!Calls TTF_OpenFont and throws std::runtime_error when the font cannot
//!be loaded.

void ttf_font::load_font()
{
	font=TTF_OpenFont(name.c_str(), size);
	if(font==nullptr) {
		throw std::runtime_error("ERROR: unable to open font "+name);
	}
}
