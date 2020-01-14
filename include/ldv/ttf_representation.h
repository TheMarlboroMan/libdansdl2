#pragma once

#include "raster_representation.h"
#include "ttf_font.h"
#include "texture.h"
#include "surface.h"
#include "canvas.h"
#include "image.h"
#include "../tools/log.h"
#include "../tools/tools.h"

#include <memory>
namespace ldv
{

//!Text representation using a TTF font.

//!Generates and owns a texture using the SDL_ttf library. Textures generated
//!are always in powers of two, to avoid hardware issues. Also, note that
//!depending on the font used, the size will not always correspond to a certain
//!pixel height.

class ttf_representation:
	public raster_representation
{
	public:

	//Enum for set_style.
	enum 				font_faces {
		bold=TTF_STYLE_BOLD,
		italic=TTF_STYLE_ITALIC, 
		underline=TTF_STYLE_UNDERLINE, 
		striketrough=TTF_STYLE_STRIKETHROUGH};

	//!Render modes. Default is blended.
	enum class 			render_mode{solid, shaded, blended};
	//!Text alignment Default is left.
	enum class			text_align{left, center, right};

						ttf_representation(const ttf_font&, rgba_color, std::string="", double=1., text_align=text_align::left, render_mode=render_mode::blended);
						ttf_representation(const ttf_representation&);
	virtual				~ttf_representation();

	//!Assignment operator. Texture is recreated as a different resource from the original.
	ttf_representation&		operator=(const ttf_representation&);

	//!Specialization of go_to, which will move the text_position.
	virtual void			go_to(point);

	//!Returns the assigned text.
	const std::string& 		get_text() const {return text;}

	//!Returns the rectangle of the text position, which may be different
	//!from the texture size (textures are in powers of two, and are usually
	//!larger than the text portion.
	const rect&			get_text_position() const {return text_position;}

	//!Allows correct use of the "align" method. 
	virtual rect		get_base_view_position() const;

	//!Locks the representation so calls to functions that would recreate the texture return before doing so.
	//!Must be accompanied by a call to "unlock_changes" that will perform recreation.
	//!Trying to draw a locked ttf_representation will throw,
	void				lock_changes() {perform_changes=false;}
	void				unlock_changes();

	void				set_color(rgb_color);
	void				set_bg_shaded_color(rgba_color);

	void				set_text_align(text_align);

	//!Returns the horizontal text alignment.
	text_align			get_text_align() const {return alignment;}

	//!Resets the font to its default style.
	ttf_representation&	reset_style();

	//!Enables typeface styles according to the font_faces flag.
	ttf_representation&	set_style(int);

	void 				set_font(const ttf_font&);

	//!Sets the text as a string. Triggers a cleanse and recreation of the texture if unlocked.
	void 				set_text(const char);
	void 				set_text(const std::string& _c) {set_text_internal(_c);}
	void				set_render_mode(render_mode r);
	void				set_line_height_ratio(double);

	protected:

	virtual void 			do_draw();

	private:

	void				create_texture();
	void				set_text_internal(const std::string&);
	void				text_replace(std::string&, const std::string&, const std::string&);

	ttf_font const *		font; //! <This is a pointer so it can change.
	std::string			text;

	render_mode			mode;
	rgb_color			text_color;
	rgba_color			bg_shaded;
	double				line_height_ratio; //!< Expressed as a ratio of the font size.
	text_align			alignment;
	bool				perform_changes=true;
	rect				text_position; //!<The text box is usually smaller than the power of 2 texture containing it. This variable stores the real text rect.
	int					text_x_displacement;

	//!Lookup table for powers of two.
	static const std::vector<int>	valid_sizes;
};

}

