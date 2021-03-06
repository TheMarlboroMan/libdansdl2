#pragma once

#include "representation.h"
#include "texture.h"
#include <ldt/log.h>

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
namespace ldv
{

//!Indicates vertical and horizontal flipping transformation for raster_representation

struct invert_transform
{
	bool        horizontal=false, 	//!< Indicates that horizontal transformation will be performed.
	            vertical=false;		//!< Indicates that vertical transformation will be performed.

	//!Checks if there is any transformations.
	bool        is_transformed() const {return horizontal || vertical;}

	//!Resets all transformations to their default state.
	void        reset()
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

	//!Gets the assigned texture.
	const texture *     get_texture() const {return texture_instance;}
	//!Gets the assigned texture.
	texture *           get_texture() {return texture_instance;}
	unsigned int        get_w_texture_instance() const;
	unsigned int        get_h_texture_instance() const;

	//!Gets the full rect of the representation.
	//TODO: Should be made obsolete.
	const rect&         get_location() const {return base_view_position;}
	//!Gets the full rect of the representation.
	//TODO: Should be made obsolete.
	rect                get_location() {return base_view_position;}
	//!Gets the texture clip of the representation.
	const rect&         get_clip() const {return clip;}
	//!Gets the texture clip of the representation.
	rect                get_clip() {return clip;}
	//!Gets the transformation structure.
	const invert_transform& get_transformation_invert() const {return transformation;}
	//!Gets the rgb color that tints the representation.
	//TODO: HOW DOES THIS REALLY WORK????.
	rgb_color           get_colorize() {return rgb_colorize;}
	//!Gets indicates whether there is rotation or flipping.
	virtual bool        is_transformed() const {return representation::is_transformed() || transformation.is_transformed();}
	//!Flips the representation horizontally without changing its position.
	void                set_invert_horizontal(bool v) {transformation.horizontal=v;}
	//!Flips the representation vertically without changing its position.
	void                set_invert_vertical(bool v) {transformation.vertical=v;}
	//!Sets the fill brush values. When these are set to anything than 0, the "location" rect will be filled with "brush" sizes patches of "clip".
	void                set_brush(int w, int h);
	//!Removes the brush.
	void                reset_brush();
	//TODO: Obsolete??
	void                set_location(const rect&);
	void                set_clip(const rect&);
	//!Sets the texture.
	void                set_texture(const texture& v) {texture_instance=&const_cast <texture&> (v);}
	//!Sets the rgb_color tint.
	//TODO: How does this work????
	void                set_colorize(rgb_color col) {rgb_colorize=col;}

	//!Resets all rotation and flipping.
	virtual void        reset_transform() {representation::reset_transform(); transformation.reset();}

	void                clip_to_texture();

	virtual void        go_to(point);

	//!Gets the position.
	virtual const point& get_position() const {return base_view_position.origin;}

	//!Gets the base view position.
	private:

	using point_type=GLdouble;
	struct texpoint {point_type x, y;};

	void                calculate_points();
	void                calculate_points_brush();

	invert_transform    transformation;
	texture *           texture_instance{nullptr};	//Este es el puntero a su superficie de memoria. Es un puntero y no una referencia para permitir cambiarlo.
	struct 	{int w, h;} brush;
	std::vector<point>  points; 	//!<Space points.
	std::vector<texpoint> tex_points; //!<Texture mapping points.
	static std::vector<point>  quad_points; 	//!<Space points.
	static std::vector<texpoint> quad_tex_points; //!<Texture mapping points.

	bool                calculate{true},
	                    with_brush{false};
	rgb_color           rgb_colorize;

	protected:

	void                reset_calculations() {calculate=true;}
	void                free_texture();
	//!Removes the texture.
	void                reset_texture() {texture_instance=nullptr;}
	virtual void        do_draw();

	rect                clip;		//!< Section of the texture used.
};

} //Fin namespace DLibV

