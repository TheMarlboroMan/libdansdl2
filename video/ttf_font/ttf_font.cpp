#include "ttf_font.h"

using namespace ldv;

ttf_font::ttf_font(const std::string& f, int t)
	:name(f), size(t), font(nullptr)
{
	load_font();
}

ttf_font::ttf_font(const ttf_font& f)
	:name(f.name), size(f.size), font(nullptr)
{
	load_font();
}

ttf_font& ttf_font::operator=(const ttf_font& f)
{
	name=f.name;
	size=f.size;
	font=nullptr;
	load_font();
	return *this;
}

ttf_font::~ttf_font()
{
	if(font)
	{
		TTF_CloseFont(font);
	}
}

void ttf_font::load_font()
{
	font=TTF_OpenFont(name.c_str(), size);
	if(font==nullptr)
	{
		throw std::runtime_error("ERROR: unable to open font "+name);
	}
}
