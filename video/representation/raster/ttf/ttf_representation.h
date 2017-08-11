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

class ttf_representation:
	public raster_representation
{
	public:

	//!Render modes. Default is blended.
	enum class 			render_mode{solid, shaded, blended};

					ttf_representation(const ttf_font&, rgba_color, std::string="");
					ttf_representation(const ttf_representation&);
	virtual				~ttf_representation();
	ttf_representation&		operator=(const ttf_representation&);

	//!Returns the assigned text.
	const std::string& 		get_text() const {return text;}

	void 				set_font(const ttf_font&);
	virtual void 			set_text(const char);
	virtual void 			set_text(const std::string&);
	//!Sets the render mode.
	void				set_render_mode(render_mode r) {mode=r;}

	private:

	void				create_texture();
	void				set_text_internal(const std::string&);
	void				text_replace(std::string&, const std::string&, const std::string&);

	ttf_font const *		font; //! <This is a pointer so it can change.
	std::string			text;

	render_mode			mode;
	rgb_color			text_color;
	rgba_color			bg_shaded;
};

}

#endif
