#ifndef LIBDANSDL2_RASTER_REPRESENTATION_H
#define LIBDANSDL2_RASTER_REPRESENTATION_H

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include "../representation.h"
#include "../../texture/texture.h"
#include "../../../tools/log/log.h"

namespace ldv
{

//!Indicates vertical and horizontal flipping transformation for raster_representation

struct invert_transform
{
	bool 		horizontal,
		 	vertical;

	invert_transform():
		horizontal(false), vertical(false)
	{}

	bool is_transformed() const {return horizontal || vertical;}

	void reset()
	{
		horizontal=false;
		vertical=false;
	}
};

//!Base class for all texture-based representations.

class raster_representation:
	public representation
{
	public:
	
				raster_representation(rect={0,0,0,0}, rect={0,0,0,0}, int=representation::alpha_max);
				raster_representation(const raster_representation&);
				raster_representation& operator=(const raster_representation &);
	virtual 		~raster_representation() {}

	//!Gets the assigned texture.
	const texture * 	get_texture() const {return texture_instance;}
	//!Gets the assigned texture. 
	texture * 		get_texture() {return texture_instance;}
	int			get_w_texture_instance() const;
	int			get_h_texture_instance() const;

	//!Gets the full rect of the representation.
	const rect& 		get_location() const {return location;}
	//!Gets the full rect of the representation.
	rect 			get_location() {return location;}
	//!Gets the texture clip of the representation.
	const rect& 		get_clip() const {return clip;}
	//!Gets the texture clip of the representation.
	rect 			get_clip() {return clip;}
	//!Gets the transformation structure.
	const invert_transform& get_transformation_invert() const {return transformation;}
	//!Gets the rgb color that tints the representation. 
	//TODO: HOW DOES THIS REALLY WORK????.
	rgb_color		get_colorize() {return rgb_colorize;}
	//!Gets indicates whether there is rotation or flipping.
	virtual bool 		is_transformed() const {return representation::is_transformed() || transformation.is_transformed();}
	//!Flips the representation horizontally without changing its position.
	void 			set_invert_horizontal(bool v) {transformation.horizontal=v;}
	//!Flips the representation vertically without changing its position.
	void 			set_invert_vertical(bool v) {transformation.vertical=v;}
	//!Sets the fill brush values.
	void			set_brush(int w, int h) {brush.w=w; brush.h=h;}
	void 			set_location(rect);
	void 			set_clip(rect);
	//!Sets the texture.
	void 			set_texture(const texture& v) {texture_instance=&const_cast <texture&> (v);}
	//!Sets the rgb_color tint. 
	//TODO: How does this work????
	void			set_colorize(rgb_color col) {rgb_colorize=col;}

	//!Resets all rotation and flipping.
	virtual void 		reset_transform() {representation::reset_transform(); transformation.reset();}

	void 			clip_to_texture();

	virtual void 		go_to(point);
	virtual point		get_position() const;
	virtual rect		get_base_view_position() const;

	private:

	struct texpoint {GLfloat x, y;};

	void			calculate_points();

	invert_transform 	transformation;

	texture * 		texture_instance;	//Este es el puntero a su superficie de memoria. Es un puntero y no una referencia para permitir cambiarlo.

	struct 	{int w, h;}	brush;

	std::vector<point>	points;
	std::vector<texpoint>	tex_points;
	rgb_color		rgb_colorize;

	protected:

	rect 			location; 	//Lugar en que se muestra de la pantalla.
	rect	 		clip;		//Considerando la dimensión total de la representación, la parte que mostramos.

	void			reset_calculations();
	void 			free_texture();
	//!Removes the texture.
	void 			reset_texture() {texture_instance=nullptr;}

	virtual void 		do_draw();
};

} //Fin namespace DLibV

#endif
