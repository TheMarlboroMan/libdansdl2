#include "ttf_representation.h"

using namespace ldv;

ttf_representation::ttf_representation(const ttf_font& pfont, rgba_color pcolor, std::string ptext)
	:raster_representation(rect{0,0,0,0}, rect{0,0,0,0}, colorif(pcolor.a)), 
	font(&pfont),
	text(ptext),
	mode(render_mode::blended), 
	text_color{pcolor.r, pcolor.g, pcolor.b},
	bg_shaded(rgba8(0,0,0,255))
{
	create_texture();
	update_view_position();
}

ttf_representation::ttf_representation(const ttf_representation& o)
	:raster_representation(o), 
	font(o.font),
	text(o.text),
	mode(o.mode),
	text_color(o.text_color),
	bg_shaded(o.bg_shaded)
{
	reset_texture();
	create_texture();
}

ttf_representation::~ttf_representation()
{
	free_texture();
}

ttf_representation& ttf_representation::operator=(const ttf_representation& o)
{
	raster_representation::operator=(o);
	font=o.font;
	text=o.text;
	mode=o.mode;
	text_color=o.text_color;
	bg_shaded=o.bg_shaded;
	create_texture();

	return *this;
}

//!Creates the texture from text.

//!This internal function is called whenever a change on text or font is 
//!detected. It changes the clip and position of the representation.
//!It may throw a std::runtime_error if the representation cannot be prepared
//!, which happens when the TTF_* functions fail.
//!Subsequent calls reuse the same texture data.
//!A change of color will not trigger a new texture.
//!This could be considered a bug. There's also another bug where rgb must be
//!converted to bgr...

//TODO: What if I want to change colors?
//TODO: Check RGB and BGR and endianess and such.

void ttf_representation::create_texture()
{	
	//The text is prepared line by line in different surfaces.

	auto explode=[](const std::string& text, char delimiter)
	{
		std::vector<std::string> result;
		std::string temp;

		for(const char& c : text) 
		{
			if(c==delimiter) 
			{	
				result.push_back(temp);
				temp="";
			}
			else
			{
				temp+=c;
			}
		}

		result.push_back(temp);
		return result;
	};

	std::vector<std::string> lines=explode(text, '\n');

	//Measuring the full resulting texture...
	int total_h=0, h=0, w=0, tw=0;

	for(const std::string& c : lines)
	{
		TTF_SizeUTF8(const_cast<TTF_Font*>(font->get_font()), c.c_str(), &tw, &h);
		if(tw > w) w=tw;
	}

	total_h=(lines.size() * h);

	//This is going to render a surface. Alpha will be always 1. When rendering it will be applied and colorised.
	//Also, notice the hack. The shaded thing will create a BGRA, so we change the colors.

	SDL_Color sdl_col{(Uint8)colorif(text_color.r), (Uint8)colorif(text_color.g), (Uint8)colorif(text_color.b), (Uint8)colorif(1.f)};
	if(mode==render_mode::blended) sdl_col=SDL_Color{(Uint8)colorif(text_color.b), (Uint8)colorif(text_color.g), (Uint8)colorif(text_color.r), (Uint8)colorif(1.f)};

	SDL_Surface * s=nullptr;

	switch(mode)
	{
		case render_mode::solid:
			s=TTF_RenderUTF8_Solid(const_cast<TTF_Font*>(font->get_font()), "a", sdl_col);
		break;
		case render_mode::shaded:
			s=TTF_RenderUTF8_Shaded(const_cast<TTF_Font*>(font->get_font()), "a", sdl_col, 
				SDL_Color{(Uint8)colorif(bg_shaded.r), (Uint8)colorif(bg_shaded.g), (Uint8)colorif(bg_shaded.b), (Uint8)colorif(bg_shaded.a)});
		break;
		case render_mode::blended:
			s=TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(font->get_font()), "a", sdl_col);
		break;
	}

	std::unique_ptr<canvas> cnv(canvas::create(w, total_h, s->format->BitsPerPixel, s->format->Rmask, s->format->Gmask, s->format->Bmask, s->format->Amask));
	SDL_FreeSurface(s);

	//Creating surfaces for each lines, pasting them on the canvas...
	int y=0;

	for(std::string& c : lines)
	{
		SDL_Surface * surf=nullptr;

		text_replace(c, "\t", "    ");
		const char * cad=c.size() ? c.c_str() : " \0";

		switch(mode)
		{
			case render_mode::solid:
				surf=TTF_RenderUTF8_Solid(const_cast<TTF_Font*>(font->get_font()), cad, sdl_col);
			break;
			case render_mode::shaded:
				surf=TTF_RenderUTF8_Shaded(const_cast<TTF_Font*>(font->get_font()), cad, sdl_col, 
					SDL_Color{(Uint8)colorif(bg_shaded.r), (Uint8)colorif(bg_shaded.g), (Uint8)colorif(bg_shaded.b), (Uint8)colorif(bg_shaded.a)});
			break;
			case render_mode::blended:
				surf=TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(font->get_font()), cad, sdl_col);
			break;
		}

		if(!surf)
		{
			throw std::runtime_error("Unable to prepare ttf_representation : "+std::string(TTF_GetError())+" : "+c);
		}
		else
		{
			SDL_Rect pos{0, y, 0, 0}; 
			SDL_BlitSurface(surf, nullptr, cnv->get_surface(), &pos);
			SDL_FreeSurface(surf);
			y+=h;
		}
	}

	if(!get_texture())
	{
		ldv::texture * tex=new ldv::texture(*cnv);
		set_texture(*tex);
	}
	else
	{
		get_texture()->replace(*cnv);
	}
	
	const auto &ref_tex=get_texture();
	set_blend(representation::blends::alpha);
	set_clip({0,0, ref_tex->get_w(), ref_tex->get_h()});
	//This must be triggered: dimensions would be left at 0 and cameras would fail.
	set_location({0, 0, (unsigned)ref_tex->get_w(), (unsigned)ref_tex->get_h()});

}

//!Sets a new ttf font.

//!Triggers a cleanse and recreation of the texture.

void ttf_representation::set_font(const ttf_font& f)
{
	font=&f;
	free_texture();
	create_texture();
}

//!Sets the text as a single character.

void ttf_representation::set_text(const char c)
{
	//This is absurd...
	std::string temp("");
	temp+=c;
	set_text_internal(temp);
}

//!Sets the text as a string.

void ttf_representation::set_text(const std::string& c)
{
	set_text_internal(c);
}

//!Internal function to change text.

//!Triggers a change of the text property and the creation of a new texture.

void ttf_representation::set_text_internal(const std::string& c)
{
	if(c!=text)
	{
		//No se limpia la textura: se va a reutilizar.
		text=c;
		reset_calculations();
		create_texture();
	}
}

//!Internal replacement function.

//!Basically keeps this class independent from the "tools" project.

void ttf_representation::text_replace(std::string& sujeto, const std::string& busca, const std::string& reemplaza)
{
	size_t pos = 0, l=reemplaza.length();
	while ((pos = sujeto.find(busca, pos)) != std::string::npos) 
	{
		sujeto.replace(pos, busca.length(), reemplaza);
		pos += l;
	}
}
