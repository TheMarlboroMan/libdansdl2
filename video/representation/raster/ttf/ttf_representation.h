#ifndef LIBDANSDL2_TTF_REPRESENTATION_H
#define LIBDANSDL2_TTF_REPRESENTATION_H

#include <memory>
#include "../raster_representation.h"
#include "../../../ttf_font/ttf_font.h"
#include "../../../texture/texture.h"
#include "../../../surface/surface.h"
#include "../../../surface/canvas.h"
#include "../../../../tools/log/log.h"
#include "../../../../tools/tools/tools.h"

namespace ldv
{
class ttf_representation:
	public raster_representation
{
	public:

					ttf_representation(const ttf_font&, rgba_color, std::string="");
					ttf_representation(const ttf_representation&);
	virtual				~ttf_representation();
	ttf_representation&		operator=(const ttf_representation&);

	const std::string& 		get_text() const {return text;}

	void 				set_font(const ttf_font&);
	virtual void 			set_text(const char);
	virtual void 			set_text(const std::string&);

	private:

	void				create_texture();
	void				set_text_internal(const std::string&);
	void				text_replace(std::string&, const std::string&, const std::string&);

	//Esto es un puntero a conciencia, para poderlo cambiar.
	ttf_font const *		font;
	std::string			text;
};

}

#endif
