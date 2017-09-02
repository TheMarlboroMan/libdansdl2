#ifndef LIBDANSDL2_TTF_REPRESENTATION_H
#define LIBDANSDL2_TTF_REPRESENTATION_H

#include <memory>
#include "../raster_representation.h"
#include "../../../ttf_font/ttf_font.h"
#include "../../../texture/texture.h"
#include "../../../surface/surface.h"
#include "../../../surface/canvas.h"
#include "../../../surface/image.h"
#include "../../../../tools/log/log.h"
#include "../../../../tools/tools/tools.h"

namespace ldv
{

//!Text representation using a TTF font.

//!Generates and owns a texture using the SDL_ttf library. Textures generated 
//!are always in powers of two, to avoid hardware issues.

class ttf_representation:
	public raster_representation
{
	public:

	enum class 			render_mode{solid, shaded, blended}; 	//!< Render modes. Default is blended.
	enum class			align{left, center, right};		//!< Text alignment Default is left.

					ttf_representation(const ttf_font&, rgba_color, std::string="", double=1., align=align::left, render_mode=render_mode::blended);
					ttf_representation(const ttf_representation&);
	virtual				~ttf_representation();
	ttf_representation&		operator=(const ttf_representation&);

	//!Returns the assigned text.
	const std::string& 		get_text() const {return text;}

	void				set_color(rgb_color);
	void				set_bg_shaded_color(rgba_color);

	void				set_align(align);

	void 				set_font(const ttf_font&);
	void 				set_text(const char);
	void 				set_text(const std::string&);
	void				set_render_mode(render_mode r);
	void				set_line_height_px(int v);
	//!Sets the line height relative to font size (1.0 is equal to font size). Will trigger a recreation of the texture.
	void 				set_line_height_relative(double v) {set_line_height_px(calculate_line_height(v));}

	private:
	
	//!Gets the height value in pixels from a double value relative to font size.
	int				calculate_line_height(double v) {return (double)font->get_size()*v;}
	void				create_texture();
	void				set_text_internal(const std::string&);
	void				text_replace(std::string&, const std::string&, const std::string&);

	ttf_font const *		font; //! <This is a pointer so it can change.
	std::string			text;

	render_mode			mode;
	rgb_color			text_color;
	rgba_color			bg_shaded;
	int				line_height; //!< Expressed in pixels...
	align				alignment;

	//!Lookup table for powers of two.
	static const std::vector<int>	valid_sizes;
};

}

#endif
