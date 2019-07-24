#include "ttf_representation.h"

using namespace ldv;

const std::vector<int> ttf_representation::valid_sizes={2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

//!Default class constructor.

//!Line height by default is constructed as font size values.

ttf_representation::ttf_representation(const ttf_font& pfont, rgba_color pcolor, std::string ptext, double lhr, text_align al, render_mode pmode)
	:raster_representation(rect{0,0,0,0}, rect{0,0,0,0}, colorif(pcolor.a)),
	font(&pfont),
	text(ptext),
	mode(pmode),
	text_color{pcolor.r, pcolor.g, pcolor.b},
	bg_shaded(rgba8(0,0,0,255)),
	line_height_ratio(lhr),
	alignment(al),
	text_position{0,0,0,0} {

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
	bg_shaded(o.bg_shaded),
	text_position{o.text_position} {

	reset_texture();
	create_texture();
}

//!Class destructor.

ttf_representation::~ttf_representation() {

	free_texture();
}

ttf_representation& ttf_representation::operator=(const ttf_representation& o) {

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

void ttf_representation::create_texture() {
	//The text is prepared line by line in different surfaces.

	auto explode=[](const std::string& _text, char delimiter) {

		std::vector<std::string> result;
		std::string temp;

		for(const char& c : _text) {
			if(c==delimiter) {
				result.push_back(temp);
				temp="";
			}
			else {
				temp+=c;
			}
		}

		result.push_back(temp);
		return result;
	};

	std::vector<std::string> lines=explode(text, '\n');

	//TODO: We should store these values where they must be..

	//Measuring the full resulting texture...
	int total_h=0, h=0, w=0, total_w=0;

	for(const std::string& c : lines) {
		TTF_SizeUTF8(const_cast<TTF_Font*>(font->get_font()), c.c_str(), &w, &h);
		if(w > total_w) total_w=w;
	}

	int line_height=(double)font->get_size()*line_height_ratio;

	total_h=(lines.size() * h) + ( (lines.size()-1) * (line_height-h)); //The last spacing is removed, of course.

	//This is going to render a surface. Alpha will be always 1. When rendering it will be applied and colorised.
	//Also, notice the hack. The shaded thing will create a BGRA, so we change the colors.

	SDL_Color sdl_col{(Uint8)colorif(text_color.r), (Uint8)colorif(text_color.g), (Uint8)colorif(text_color.b), (Uint8)colorif(1.f)};
	if(mode==render_mode::blended) {
		sdl_col=SDL_Color{(Uint8)colorif(text_color.b), (Uint8)colorif(text_color.g), (Uint8)colorif(text_color.r), (Uint8)colorif(1.f)};
	}

	SDL_Surface * s=nullptr;

	switch(mode) {
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

	auto get_next_power_of_2=[this](int v)->int {
		for(const auto& vs : valid_sizes) {
			if(v > vs) continue;
			return vs;
		}

		throw std::runtime_error("Invalid text texture size");
	};

	auto canvas_w=get_next_power_of_2(total_w);
	auto canvas_h=get_next_power_of_2(total_h);

	std::unique_ptr<canvas> cnv(canvas::create(
		canvas_w,
		canvas_h,
		s->format->BitsPerPixel, s->format->Rmask, s->format->Gmask, s->format->Bmask, s->format->Amask));
	SDL_FreeSurface(s);

	//Creating surfaces for each lines, pasting them on the canvas...
	int y=0;

	for(std::string& c : lines) {
		SDL_Surface * surf=nullptr;

		text_replace(c, "\t", "    ");
		const char * cad=c.size() ? c.c_str() : " \0";

		switch(mode) {
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

		if(!surf) {
			throw std::runtime_error("Unable to prepare ttf_representation : "+std::string(TTF_GetError())+" : "+c);
		}

		//TODO: We should not do this again, it has been done earlier.
		TTF_SizeUTF8(const_cast<TTF_Font*>(font->get_font()), cad, &w, &h);

		int x=0;

//TODO:
/*
All this is terribly hacked: some implementations of OpenGL will botch the
text if we use linear filtering and map anything else than the whole texture
to a polygon of the full texture size... To solve this, we will map the
clip and location to the full texture size (in powers of 2) rather than to the
effective size of the text. This, in time, causes problems when aligning text
to the right if we don't use the canvas_w as a measure and with the "align"
method.

Corollary: either fix that with virtual methods for the box size and such or
accept that calling align with "to the inner right" as argument makes only sense
if the text is right aligned itself (which works).
*/

		//TODO: This only makes sense if we have more than one line.
		switch(alignment) {
			case text_align::left: x=0; break;
			case text_align::center: x=(canvas_w/2)-(w/2); break;
			case text_align::right: x=canvas_w-w; break;
		}

		SDL_Rect pos{x, y, 0, 0};
		SDL_BlitSurface(surf, nullptr, cnv->get_surface(), &pos);
		SDL_FreeSurface(surf);
		y+=h+(line_height-h);
	}

	if(!get_texture())	{
		ldv::texture * tex=new ldv::texture(*cnv);
		set_texture(*tex);
	}
	else {
		get_texture()->replace(*cnv);
	}

	set_blend(representation::blends::alpha);
	set_clip({0,0, (unsigned)canvas_w, (unsigned)canvas_h});
	//This must be triggered: dimensions would be left at 0 and cameras would fail.
	set_location({0, 0, (unsigned)canvas_w, (unsigned)canvas_h});
	text_position={0, 0, (unsigned)total_w, (unsigned)total_h};
}

//!Sets a new ttf font.

//!Triggers a cleanse and recreation of the texture if unlocked.

void ttf_representation::set_font(const ttf_font& f) {

	font=&f;
	if(!perform_changes) return;
	create_texture();
}

//!Sets the text as a single character.

//!Triggers a cleanse and recreation of the texture if unlocked.

void ttf_representation::set_text(const char c) {

	//This is absurd...
	std::string temp("");
	temp+=c;
	set_text_internal(temp);
}

//!Sets the text as a string.

//!Triggers a cleanse and recreation of the texture if unlocked.

void ttf_representation::set_text(const std::string& c) {

	set_text_internal(c);
}

//!Internal function to change text.

//!Triggers a change of the text property and the creation of a new texture if unlocked.

void ttf_representation::set_text_internal(const std::string& c) {

	if(c!=text) {
		//The texture is going to be reused.
		text=c;
		if(!perform_changes) return;
		auto pos=get_position();
		reset_calculations();
		create_texture();
		go_to(pos); //Setting the text incidentally resets the position.
	}
}

//!Unlocks the representation and refreshes its values.
void ttf_representation::unlock_changes() {

	perform_changes=true;
	auto pos=get_position();
	reset_calculations();
	create_texture();
	go_to(pos);
}

//!Internal replacement function.

//!Basically keeps this class independent from the "tools" project.

void ttf_representation::text_replace(std::string& sujeto, const std::string& busca, const std::string& reemplaza) {

	size_t pos = 0, l=reemplaza.length();
	while ((pos = sujeto.find(busca, pos)) != std::string::npos) {

		sujeto.replace(pos, busca.length(), reemplaza);
		pos += l;
	}
}

//!Sets the line height as a ratio of the font size.

//!A value of 1.0 given an space equal to the font size. 1.5 gives a line
//!and a half and so on.
//!This function will trigger a recreation of the texture if unlocked.

void ttf_representation::set_line_height_ratio(double v) {

	if(v==line_height_ratio) return;
	line_height_ratio=v;
	if(!perform_changes) return;
	create_texture();
}

//!Sets the font color.

//!Will trigger a recreation of the texture if different and unlocked.
void ttf_representation::set_color(rgb_color c) {

	if(c==text_color) return;
	text_color=c;
	if(!perform_changes) return;
	create_texture();
}

//!Sets the background color for the shaded mode.

//!Will trigger a recreation of the texture if the mode is shaded and changes (if unlocked).

void ttf_representation::set_bg_shaded_color(rgba_color c) {

	if(c==bg_shaded) return;
	bg_shaded=c;
	if(mode!=render_mode::shaded) return;
	if(!perform_changes) return;
	create_texture();
}

//!Sets the render mode.

//!Triggers a texture recreation if changes and unlocked.

void ttf_representation::set_render_mode(render_mode r) {

	if(r==mode) return;
	mode=r;
	if(!perform_changes) return;
	create_texture();
}

//!Sets the text alignment.

//!Triggers a texture recreation if changes and unlocked. Text alignment only
//!matters when there is more than a single line in the text.

void ttf_representation::set_text_align(text_align v) {

	if(v==alignment) return;
	alignment=v;
	if(!perform_changes) return;
	create_texture();
}

void ttf_representation::do_draw() {

	if(!perform_changes) {
		throw std::runtime_error("ttf_representation is locked!");
	}

	raster_representation::do_draw();
}

void ttf_representation::go_to(point _p) {

	text_position.origin=_p;
	raster_representation::go_to(_p);
}
