#include "ttf_representation.h"

using namespace ldv;

const std::vector<int> ttf_representation::valid_sizes={2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

//!Default class constructor.

//!Line height by default is constructed as font size values.

ttf_representation::ttf_representation(const ttf_font& pfont, rgba_color pcolor, std::string ptext, double lh, align al, render_mode pmode)
	:raster_representation(rect{0,0,0,0}, rect{0,0,0,0}, colorif(pcolor.a)), 
	font(&pfont),
	text(ptext),
	mode(pmode), 
	text_color{pcolor.r, pcolor.g, pcolor.b},
	bg_shaded(rgba8(0,0,0,255)),
	line_height(calculate_line_height(lh)),
	alignment(al)
{
	create_texture();
	update_view_position();
}

//!Copy constructor.

//!Texture is recreated as a different resource from the original.

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

//!Class destructor.

ttf_representation::~ttf_representation()
{
	free_texture();
}

//!Assignment operator.

//!Texture is recreated as a different resource from the original.

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
	int total_h=0, h=0, w=0, total_w=0;

	for(const std::string& c : lines)
	{
		TTF_SizeUTF8(const_cast<TTF_Font*>(font->get_font()), c.c_str(), &total_w, &h);
		if(total_w > w) w=total_w;
	}

	//TODO: Check this calculation...
	total_h=(lines.size() * h) + (lines.size() * (line_height-h));
	std::cout<<lines.size()<<" of "<<h<<" pixels each with "<<line_height<<" is "<<total_h<<std::endl;

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

	auto get_next_power_of_2=[this](int v)->int
	{
		for(const auto& vs : valid_sizes)
		{
			if(v > vs) continue;
			return vs;
		}

		throw std::runtime_error("Invalid text texture size");
	};

	std::unique_ptr<canvas> cnv(canvas::create(
		get_next_power_of_2(w), 
		get_next_power_of_2(total_h), 
		s->format->BitsPerPixel, s->format->Rmask, s->format->Gmask, s->format->Bmask, s->format->Amask));
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
			int x=0;
			switch(alignment)
			{
				case align::left: x=0; break;
				case align::center: x=(total_w/2)-(surf->w/2); break;
				case align::right: x=total_w-surf->w; break;
			}

			SDL_Rect pos{x, y, 0, 0}; 
			SDL_BlitSurface(surf, nullptr, cnv->get_surface(), &pos);
			SDL_FreeSurface(surf);
			//TODO: And this???
			y+=h+(line_height-h);
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
	
//	const auto &ref_tex=get_texture();
	set_blend(representation::blends::alpha);
	set_clip({0,0, (unsigned)w, (unsigned)total_h});
	//This must be triggered: dimensions would be left at 0 and cameras would fail.
	set_location({0, 0, (unsigned)w, (unsigned)total_h});

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

		auto pos=get_position();
		text=c;
		reset_calculations();
		create_texture();
		go_to(pos); //Setting the text incidentally resets the position.
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

//!Sets the line height in pixels.

//!This function will trigger a recreation of the texture.

void ttf_representation::set_line_height_px(int v) 
{
	line_height=v;
	free_texture();
	create_texture();
}

//!Sets the font color.

//!Will trigger a recreation of the texture if different.
void ttf_representation::set_color(rgb_color c)
{
	if(c==text_color) return;
	text_color=c;
	free_texture();
	create_texture();
}

//!Sets the background color for the shaded mode.

//!Will trigger a recreation of the texture if the mode is shaded and changes.

void ttf_representation::set_bg_shaded_color(rgba_color c)
{
	if(c==bg_shaded) return;
	bg_shaded=c;
	if(mode!=render_mode::shaded) return;
	free_texture();
	create_texture();
}

//!Sets the render mode.

//!Triggers a texture recreation if changes.

void ttf_representation::set_render_mode(render_mode r)
{
	if(r==mode) return;
	mode=r;
	free_texture();
	create_texture();
}

//!Sets the text alignment.

//!Triggers a texture recreation if changes. Text alignment only matters when
//!there is more than a single line in the text.

void ttf_representation::set_align(align v)
{
	if(v==alignment) return;
	alignment=v;
	free_texture();
	create_texture();
}
