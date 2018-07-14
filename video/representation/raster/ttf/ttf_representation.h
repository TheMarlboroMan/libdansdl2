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
//!are always in powers of two, to avoid hardware issues. Also, note that 
//!depending on the font used, the size will not always correspond to a certain
//!pixel height.

class ttf_representation:
	public raster_representation
{
	public:

	//!Render modes. Default is blended.
	enum class 			render_mode{solid, shaded, blended}; 
	//!Text alignment Default is left.
	enum class			text_align{left, center, right};

					ttf_representation(const ttf_font&, rgba_color, std::string="", double=1., text_align=text_align::left, render_mode=render_mode::blended);
					ttf_representation(const ttf_representation&);
	virtual				~ttf_representation();

	//!Assignment operator. Texture is recreated as a different resource from the original.
	ttf_representation&		operator=(const ttf_representation&);

	//!Returns the assigned text.
	const std::string& 		get_text() const {return text;}

	//!Locks the representation so calls to functions that would recreate the texture return before doing so.
	//!Must be accompanied by a call to "unlock_changes" that will perform recreation. 
	//!Trying to draw a locked ttf_representation will throw,
	void				lock_changes() {perform_changes=false;}
	void				unlock_changes();

	void				set_color(rgb_color);
	void				set_bg_shaded_color(rgba_color);

	void				set_text_align(text_align);

	void 				set_font(const ttf_font&);
	void 				set_text(const char);
	void 				set_text(const std::string&);
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

	//!Lookup table for powers of two.
	static const std::vector<int>	valid_sizes;
};

}

#endif
