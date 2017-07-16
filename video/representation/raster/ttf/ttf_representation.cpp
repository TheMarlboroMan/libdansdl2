#include "ttf_representation.h"

using namespace ldv;

ttf_representation::ttf_representation(const ttf_font& pfont, rgba_color pcolor, std::string ptext)
	:raster_representation(pcolor, rect{0,0,0,0}, rect{0,0,0,0}, sampling::complete), 
	font(&pfont),
	text(ptext),
	mode(render_mode::blended), 
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
	create_texture();

	return *this;
}

void ttf_representation::create_texture()
{	
	//Si el text tiene newlines no va a funcionar: tenemos que montar
	//nosotros el código. Empezamos por partir el text en varias líneas...

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

	//Y ahora medimos el que sería el tamaño total de la superficie del text.
	int total_h=0, h=0, w=0, tw=0;

	for(const std::string& c : lines)
	{
		TTF_SizeUTF8(const_cast<TTF_Font*>(font->get_font()), c.c_str(), &tw, &h);
		if(tw > w) w=tw;
	}

	total_h=(lines.size() * h);
	// TTF_FontLineSkip(const_cast<TTF_Font*>(font->get_font()))) + h;

	//Podemos preparar una superficie de ese tamaño... Vamos a sacar una
	//superficie primero para obtener el formato... Es una mierda pero
	//me vale.
	auto col=get_rgba();
	SDL_Color sdl_col{(Uint8)colorif(col.r), (Uint8)colorif(col.g), (Uint8)colorif(col.b), (Uint8)colorif(col.a)};

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

	//Y ahora, por cada línea, crear una superficie y pegarla en el cnv...
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
	set_location(0, 0, ref_tex->get_w(), ref_tex->get_h(), frw|frh);	//Esto debemos llamarlo aquí, de lo contrario se queda con ancho y alto 0, dando problemas con las cámaras.
}

void ttf_representation::set_font(const ttf_font& f)
{
	font=&f;
	free_texture();
	create_texture();
}

void ttf_representation::set_text(const char c)
{
	//Hay que joderse...
	std::string temp("");
	temp+=c;
	set_text_internal(temp);
}

void ttf_representation::set_text(const std::string& c)
{
	set_text_internal(c);
}

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

void ttf_representation::text_replace(std::string& sujeto, const std::string& busca, const std::string& reemplaza)
{
	size_t pos = 0, l=reemplaza.length();
	while ((pos = sujeto.find(busca, pos)) != std::string::npos) 
	{
		sujeto.replace(pos, busca.length(), reemplaza);
		pos += l;
	}
}
